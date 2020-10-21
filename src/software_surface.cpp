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

#include "software_surface.hpp"

#include <algorithm>
#include <assert.h>
#include <string.h>

#include <geom/rect.hpp>

#include "rgb.hpp"
#include "rgba.hpp"

namespace surf {

SoftwareSurface::SoftwareSurface() :
  m_pixel_data(std::make_shared<PixelData>())
{
}

SoftwareSurface::SoftwareSurface(PixelData data) :
  m_pixel_data(std::make_shared<PixelData>(std::move(data)))
{
}

SoftwareSurface
SoftwareSurface::halve() const
{
  PixelData const& src = *m_pixel_data;
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

  return SoftwareSurface(std::move(dst));
}

SoftwareSurface
SoftwareSurface::scale(geom::isize const& size) const
{
  PixelData const& src = *m_pixel_data;

  if (src.get_size() == size)
  {
    return *this;
  }
  else if (src.get_size() == geom::isize(0, 0))
  {
    return SoftwareSurface(PixelData(m_pixel_data->get_format(), size));
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
        assert(false && "SoftwareSurface::scale: Unknown format");
        break;
    }

    return SoftwareSurface(std::move(dst));
  }
}

SoftwareSurface
SoftwareSurface::crop(geom::irect const& rect_in) const
{
  // FIXME: We could do a crop without copying content, sm_imply
  // reference the old SoftwareSurfaceM_Impl and have a different pitch
  // and pixel offset
  assert(rect_in);

  // Clip the rectangle to the image
  geom::irect rect(std::clamp(rect_in.left(),   0, get_width()),
                   std::clamp(rect_in.top(),    0, get_height()),
                   std::clamp(rect_in.right(),  0, get_width()),
                   std::clamp(rect_in.bottom(), 0, get_height()));

  PixelData const& src = *m_pixel_data;
  PixelData dst(src.get_format(), rect.size());

  for(int y = rect.top(); y < rect.bottom(); ++y)
  {
    memcpy(dst.get_row_data(y - rect.top()),
           src.get_row_data(y) + rect.left() * src.get_bytes_per_pixel(),
           rect.width() * src.get_bytes_per_pixel());
  }

  return SoftwareSurface(std::move(dst));
}

geom::isize
SoftwareSurface::get_size()  const
{
  return m_pixel_data->get_size();
}

int
SoftwareSurface::get_width()  const
{
  return m_pixel_data->get_width();
}

int
SoftwareSurface::get_height() const
{
  return m_pixel_data->get_height();
}

RGB
SoftwareSurface::get_average_color() const
{
  PixelData const& src = *m_pixel_data;

  if (src.empty())
  {
    return {};
  }
  else
  {
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
}

SoftwareSurface
SoftwareSurface::to_rgb() const
{
  PixelData const& src = *m_pixel_data;

  switch(src.get_format())
  {
    case PixelFormat::RGB:
      return *this;

    case PixelFormat::RGBA:
    {
      PixelData pixel_data(PixelFormat::RGB, src.get_size());

      int num_pixels = get_width() * get_height();
      uint8_t const* src_pixels = src.get_data();
      uint8_t* dst_pixels = pixel_data.get_data();

      for(int i = 0; i < num_pixels; ++i)
      {
        dst_pixels[3*i+0] = src_pixels[4*i+0];
        dst_pixels[3*i+1] = src_pixels[4*i+1];
        dst_pixels[3*i+2] = src_pixels[4*i+2];
      }

      return SoftwareSurface(std::move(pixel_data));
    }

    default:
      assert(false && "SoftwareSurface::to_rgb: Unknown format");
      return {};
  }
}

} // namespace surf

/* EOF */
