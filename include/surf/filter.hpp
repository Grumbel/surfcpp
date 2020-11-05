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

#ifndef HEADER_SURF_FILTER_HPP
#define HEADER_SURF_FILTER_HPP

#include "color.hpp"
#include "pixel_view.hpp"
#include "unwrap.hpp"

namespace surf {

template<typename Pixel, typename PixelFunc>
void pixel_filter(PixelView<Pixel> src, PixelFunc func)
{
  for(int y = 0; y < src.get_height(); ++y) {
    Pixel* const row = src.get_row(y);
    for(int x = 0; x < src.get_width(); ++x) {
      row[x] = func(row[x]);
    }
  }
}

template<typename Pixel>
void apply_gamma(PixelView<Pixel> src, float gamma)
{
  // FIXME: Slow
  for(int y = 0; y < src.get_height(); ++y) {
    for(int x = 0; x < src.get_width(); ++x) {
      Color rgba = src.get_pixel_color({x, y});
      rgba.r = powf(rgba.r, 1.0f / gamma);
      rgba.g = powf(rgba.g, 1.0f / gamma);
      rgba.b = powf(rgba.b, 1.0f / gamma);
      src.put_pixel_color({x, y}, rgba);
    }
  }
}

template<typename Pixel>
void apply_brightness(PixelView<Pixel> src, float brightness)
{
  // FIXME: Slow
  for(int y = 0; y < src.get_height(); ++y) {
    for(int x = 0; x < src.get_width(); ++x) {
      Color rgba = src.get_pixel_color({x, y});
      rgba.r += brightness;
      rgba.g += brightness;
      rgba.b += brightness;
      src.put_pixel_color({x, y}, rgba);
    }
  }
}

template<typename Pixel>
void apply_invert(PixelView<Pixel> src)
{
  // FIXME: Slow
  for(int y = 0; y < src.get_height(); ++y) {
    for(int x = 0; x < src.get_width(); ++x) {
      Color rgba = src.get_pixel_color({x, y});
      rgba.r = 1.0f - rgba.r;
      rgba.g = 1.0f - rgba.g;
      rgba.b = 1.0f - rgba.b;
      src.put_pixel_color({x, y}, clamp(rgba));
    }
  }
}

SOFTWARE_SURFACE_LIFT_VOID(apply_gamma)
SOFTWARE_SURFACE_LIFT_VOID(apply_brightness)
SOFTWARE_SURFACE_LIFT_VOID(apply_invert)

} // namespace surf

#endif

/* EOF */
