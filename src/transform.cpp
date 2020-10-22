// surf - Software surface library
// Copyright (C) 2008-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "transform.hpp"

#include <string.h>

#include <geom/rect.hpp>

#include "rgb.hpp"
#include "rgba.hpp"
#include "software_surface.hpp"

namespace surf {

namespace {

inline
void copy_pixel_rgb(PixelData& dst, int dst_x, int dst_y,
                    PixelData const& src, int src_x, int src_y)
{
  uint8_t* const d = dst.get_row_data(dst_y) + 3*dst_x;
  uint8_t const* const s = src.get_row_data(src_y) + 3*src_x;
  d[0] = s[0];
  d[1] = s[1];
  d[2] = s[2];
}

inline
void copy_pixel_rgba(PixelData& dst, int dst_x, int dst_y,
                     PixelData const& src, int src_x, int src_y)
{
  uint32_t* const d = reinterpret_cast<uint32_t*>(dst.get_row_data(dst_y) + 4*dst_x);
  uint32_t const* const s = reinterpret_cast<uint32_t const*>(src.get_row_data(src_y) + 4*src_x);
  *d = *s;
}

} // namespace

PixelData
transform(PixelData const& src, Transform mod)
{
  switch (mod)
  {
    case Transform::ROTATE_0:
      return src;

    case Transform::ROTATE_90:
      return rotate90(src);

    case Transform::ROTATE_180:
      return rotate180(src);

    case Transform::ROTATE_270:
      return rotate270(src);

    case Transform::ROTATE_0_FLIP:
      return flip_vertical(src);

    case Transform::ROTATE_90_FLIP:
      // FIXME: Could be made faster
      return flip_vertical(rotate90(src));

    case Transform::ROTATE_180_FLIP:
      return flip_horizontal(src);

    case Transform::ROTATE_270_FLIP:
      // FIXME: Could be made faster
      return flip_vertical(rotate270(src));

    default:
      assert(false && "never reached");
      return src;
  }
}

PixelData rotate90(PixelData const& src)
{
  PixelData dst(src.get_format(), geom::isize(src.get_size().height(), src.get_size().width()));

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      for(int y = 0; y < src.get_size().height(); ++y)
      {
        for(int x = 0; x < src.get_size().width(); ++x)
        {
          copy_pixel_rgb(dst, src.get_size().height() - y - 1, x,
                         src, x, y);
        }
      }
      break;

    case PixelFormat::RGBA:
      for(int y = 0; y < src.get_size().height(); ++y)
      {
        for(int x = 0; x < src.get_size().width(); ++x)
        {
          copy_pixel_rgba(dst, src.get_size().height() - y - 1, x,
                          src, x, y);
        }
      }
      break;
  }

  return dst;
}

PixelData rotate180(PixelData const& src)
{
  PixelData dst(src.get_format(), src.get_size());

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      for(int y = 0; y < src.get_size().height(); ++y)
      {
        for(int x = 0; x < src.get_size().width(); ++x)
        {
          copy_pixel_rgb(dst, src.get_size().width() - x - 1, src.get_size().height() - 1 - y,
                         src, x, y);
        }
      }
      break;

    case PixelFormat::RGBA:
      for(int y = 0; y < src.get_size().height(); ++y)
      {
        for(int x = 0; x < src.get_size().width(); ++x)
        {
          copy_pixel_rgba(dst, src.get_size().width() - x - 1, src.get_size().height() - 1 - y,
                          src, x, y);
        }
      }
      break;
  }

  return dst;
}

PixelData rotate270(PixelData const& src)
{
  PixelData dst(src.get_format(), geom::isize(src.get_height(), src.get_width()));

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      for(int y = 0; y < src.get_size().height(); ++y) {
        for(int x = 0; x < src.get_size().width(); ++x) {
          copy_pixel_rgb(dst, y, src.get_size().width() - 1 - x,
                         src, x, y);
        }
      }
      break;

    case PixelFormat::RGBA:
      for(int y = 0; y < src.get_size().height(); ++y) {
        for(int x = 0; x < src.get_size().width(); ++x) {
          copy_pixel_rgba(dst, y, src.get_size().width() - 1 - x,
                          src, x, y);
        }
      }
      break;
  }

  return dst;
}

PixelData flip_horizontal(PixelData const& src)
{
  PixelData dst(src.get_format(), src.get_size());

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      for(int y = 0; y < src.get_size().height(); ++y) {
        for(int x = 0; x < src.get_size().width(); ++x) {
          copy_pixel_rgb(dst, src.get_size().width() - 1 - x, y,
                         src, x, y);
        }
      }
      break;

    case PixelFormat::RGBA:
      for(int y = 0; y < src.get_size().height(); ++y) {
        for(int x = 0; x < src.get_size().width(); ++x) {
          copy_pixel_rgba(dst, src.get_size().width() - 1 - x, y,
                          src, x, y);
        }
      }
      break;
  }

  return dst;
}

PixelData flip_vertical(PixelData const& src)
{
  PixelData dst(src.get_format(), src.get_size());

  for(int y = 0; y < src.get_size().height(); ++y)
  {
    memcpy(dst.get_row_data(src.get_size().height() - y - 1),
           src.get_row_data(y),
           static_cast<size_t>(src.get_pitch()));
  }

  return dst;
}

PixelData halve(PixelData const& src)
{
  PixelData dst(src.get_format(), src.get_size() / 2);

  int src_p = src.get_pitch();

  int dst_w = dst.get_width();
  int dst_h = dst.get_height();
  int dst_p = dst.get_pitch();

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      for(int y = 0; y < dst_h; ++y)
      {
        for(int x = 0; x < dst_w; ++x)
        {
          uint8_t* d = dst.get_data() + (y*dst_p + 3*x);
          uint8_t const* s = src.get_data() + (y*src_p + 3*x)*2;

          d[0] = static_cast<uint8_t>((s[0] + s[0+3] + s[0+src_p] + s[0+src_p+3])/4);
          d[1] = static_cast<uint8_t>((s[1] + s[1+3] + s[1+src_p] + s[1+src_p+3])/4);
          d[2] = static_cast<uint8_t>((s[2] + s[2+3] + s[2+src_p] + s[2+src_p+3])/4);
        }
      }
      break;

    case PixelFormat::RGBA:
      for(int y = 0; y < dst_h; ++y)
      {
        for(int x = 0; x < dst_w; ++x)
        {
          uint8_t* d = dst.get_data() + (y*dst_p + 4*x);
          uint8_t const* s = src.get_data() + (y*src_p + 4*x)*2;

          d[0] = static_cast<uint8_t>((s[0] + s[0+4] + s[0+src_p] + s[0+src_p+4])/4);
          d[1] = static_cast<uint8_t>((s[1] + s[1+4] + s[1+src_p] + s[1+src_p+4])/4);
          d[2] = static_cast<uint8_t>((s[2] + s[2+4] + s[2+src_p] + s[2+src_p+4])/4);
          d[3] = static_cast<uint8_t>((s[3] + s[3+4] + s[3+src_p] + s[3+src_p+4])/4);
        }
      }
      break;

    default:
      assert(false && "Not reachable");
      break;
  }

  return dst;
}

PixelData scale(PixelData const& src, geom::isize const& size)
{
  if (src.get_size() == size)
  {
    return src;
  }
  else if (src.get_size() == geom::isize(0, 0))
  {
    return PixelData(src.get_format(), size);
  }
  else
  {
    PixelData dst(src.get_format(), size);

    // FIXME: very much non-fast
    switch(src.get_format())
    {
      case PixelFormat::RGB:
      {
        RGB rgb;
        for(int y = 0; y < dst.get_height(); ++y)
        {
          for(int x = 0; x < dst.get_width(); ++x)
          {
            src.get_pixel({x * src.get_size().width()  / dst.get_size().width(),
                           y * src.get_size().height() / dst.get_size().height()},
                          rgb);

            dst.put_pixel({x, y}, rgb);
          }
        }
      }
      break;

      case PixelFormat::RGBA:
      {
        RGBA rgba;
        for(int y = 0; y < dst.get_height(); ++y)
        {
          for(int x = 0; x < dst.get_width(); ++x)
          {
            src.get_pixel({x * src.get_size().width()  / dst.get_size().width(),
                           y * src.get_size().height() / dst.get_size().height()},
                          rgba);

            dst.put_pixel({x, y}, rgba);
          }
        }
      }
      break;

      default:
        assert(false && "PixelData::scale: Unknown format");
        break;
    }

    return dst;
  }
}

PixelData crop(PixelData const& src, geom::irect const& rect)
{
  // Clip the rectangle to the image
  geom::irect clipped(std::clamp(rect.left(), 0, src.get_width()),
                      std::clamp(rect.top(), 0, src.get_height()),
                      std::clamp(rect.right(), 0, src.get_width()),
                      std::clamp(rect.bottom(), 0, src.get_height()));

  PixelData dst(src.get_format(), clipped.size());

  for(int y = clipped.top(); y < clipped.bottom(); ++y)
  {
    memcpy(dst.get_row_data(y - clipped.top()),
           src.get_row_data(y) + clipped.left() * src.get_bytes_per_pixel(),
           clipped.width() * src.get_bytes_per_pixel());
  }

  return dst;
}

PixelData to_rgb(PixelData const& src)
{
  switch(src.get_format())
  {
    case PixelFormat::RGB:
      return src;

    case PixelFormat::RGBA: {
      PixelData dst(PixelFormat::RGB, src.get_size());

      int num_pixels = src.get_width() * src.get_height();
      uint8_t const* src_pixels = src.get_data();
      uint8_t* dst_pixels = dst.get_data();

      for(int i = 0; i < num_pixels; ++i)
      {
        dst_pixels[3*i+0] = src_pixels[4*i+0];
        dst_pixels[3*i+1] = src_pixels[4*i+1];
        dst_pixels[3*i+2] = src_pixels[4*i+2];
      }

      return dst;
    }

    default:
      assert(false && "PixelData::to_rgb: Unknown format");
      return {};
  }
}

RGB average_color(PixelData const& src)
{
  if (src.empty()) {
    return {};
  }

  unsigned int total_r = 0;
  unsigned int total_g = 0;
  unsigned int total_b = 0;

  for(int y = 0; y < src.get_height(); ++y)
  {
    unsigned int row_r = 0;
    unsigned int row_g = 0;
    unsigned int row_b = 0;

    for(int x = 0; x < src.get_width(); ++x)
    {
      RGB rgb;
      src.get_pixel({x, y}, rgb);

      row_r += rgb.r;
      row_g += rgb.g;
      row_b += rgb.b;
    }

    total_r += row_r / src.get_width();
    total_g += row_g / src.get_width();
    total_b += row_b / src.get_width();
  }

  unsigned int num_rows = static_cast<unsigned int>(src.get_height());
  return RGB(static_cast<uint8_t>(total_r / num_rows),
             static_cast<uint8_t>(total_g / num_rows),
             static_cast<uint8_t>(total_b / num_rows));
}

} // namespace surf

/* EOF */
