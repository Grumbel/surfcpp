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

#ifndef HEADER_SURF_FILL_HPP
#define HEADER_SURF_FILL_HPP

#include "pixel.hpp"
#include "pixel_data.hpp"

namespace surf {

template<typename Pixel>
void fill(PixelData<Pixel>& dst, Pixel const& pixel)
{
  for (int y = 0; y < dst.get_height(); ++y) {
    std::fill_n(dst.get_row(y), dst.get_width(), pixel);
  }
}

template<typename Pixel>
void fill__slow(PixelData<Pixel>& dst, Pixel const& pixel)
{
  for (int y = 0; y < dst.get_height(); ++y) {
    Pixel* const row = dst.get_row(y);
    for (int x = 0; x < dst.get_width(); ++x) {
      row[x] = pixel;
    }
  }
}

template<typename Pixel>
void fill_rect(PixelData<Pixel>& dst, geom::irect const& rect, Pixel const& pixel)
{
  geom::irect const region = geom::intersection(geom::irect(dst.get_size()), rect);

  for (int y = region.top(); y < region.bottom(); ++y) {
    Pixel* const row = dst.get_row(y) + region.left();
    std::fill_n(row, region.width(), pixel);
  }
}

} // namespace surf

#endif

/* EOF */
