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

SoftwareSurface
transform(SoftwareSurface const& surface, Transform mod)
{
  switch (mod)
  {
    case Transform::ROTATE_0:
      return surface;

    case Transform::ROTATE_90:
      return rotate90(surface);

    case Transform::ROTATE_180:
      return rotate180(surface);

    case Transform::ROTATE_270:
      return rotate270(surface);

    case Transform::ROTATE_0_FLIP:
      return flip_vertical(surface);

    case Transform::ROTATE_90_FLIP:
      // FIXME: Could be made faster
      return flip_vertical(rotate90(surface));

    case Transform::ROTATE_180_FLIP:
      return flip_horizontal(surface);

    case Transform::ROTATE_270_FLIP:
      // FIXME: Could be made faster
      return flip_vertical(rotate270(surface));

    default:
      assert(false && "never reached");
      return surface;
  }
}

SoftwareSurface
rotate90(SoftwareSurface const& surface)
{
  PixelData const& src = surface.get_pixel_data();
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

  return SoftwareSurface(std::move(dst));
}

SoftwareSurface
rotate180(SoftwareSurface const& surface)
{
  PixelData const& src = surface.get_pixel_data();
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

  return SoftwareSurface(std::move(dst));
}

SoftwareSurface
rotate270(SoftwareSurface const& surface)
{
  PixelData const& src = surface.get_pixel_data();
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

  return SoftwareSurface(std::move(dst));
}

SoftwareSurface
flip_horizontal(SoftwareSurface const& surface)
{
  PixelData const& src = surface.get_pixel_data();
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

  return SoftwareSurface(std::move(dst));
}

SoftwareSurface
flip_vertical(SoftwareSurface const& surface)
{
  PixelData const& src = surface.get_pixel_data();
  PixelData dst(src.get_format(), src.get_size());

  for(int y = 0; y < src.get_size().height(); ++y)
  {
    memcpy(dst.get_row_data(src.get_size().height() - y - 1),
           src.get_row_data(y),
           static_cast<size_t>(src.get_pitch()));
  }

  return SoftwareSurface(std::move(dst));
}

} // namespace surf

/* EOF */
