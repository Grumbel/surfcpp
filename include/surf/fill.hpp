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

namespace detail {

template<typename Pixel>
void fill__slow(PixelData<Pixel>& dst, Pixel const& pixel) // const& gives 2x speedup?!
{
  int const h = dst.get_height();
  int const w = dst.get_width(); // 5x speedup

  for (int y = 0; y < h; ++y) {
    Pixel* const row = dst.get_row(y);

    for (int x = 0; x < w; ++x) {
      row[x] = pixel;
    }
  }
}

template<typename Pixel>
void fill__filln(PixelData<Pixel>& dst, Pixel const& pixel)
{
  for (int y = 0; y < dst.get_height(); ++y) {
    std::fill_n(dst.get_row(y), dst.get_width(), pixel);
  }
}

template<typename Pixel>
void fill__fast(PixelData<Pixel>& dst, Pixel const& pixel)
{
  for (int y = 0; y < dst.get_height(); ++y) {
    Pixel* const row = dst.get_row(y);
    int const w = dst.get_width();
    int x = 0;
    for (; x < w; x += 8) {
      row[x + 0] = pixel;
      row[x + 1] = pixel;

      row[x + 2] = pixel;
      row[x + 3] = pixel;

      row[x + 4] = pixel;
      row[x + 5] = pixel;

      row[x + 6] = pixel;
      row[x + 7] = pixel;
    }
    x -= 8;
    for (; x < w; x += 1) {
      row[x] = pixel;
    }
  }
}

template<typename Pixel>
void fill__memset(PixelData<Pixel>& dst)
{
  for (int y = 0; y < dst.get_height(); ++y) {
    std::memset(dst.get_row(y), 0, dst.get_width() * sizeof(Pixel));
  }
}

template<typename Pixel>
void fill_rect__filln(PixelData<Pixel>& dst, geom::irect const& rect, Pixel const& pixel)
{
  geom::irect const region = geom::intersection(geom::irect(dst.get_size()), rect);

  for (int y = region.top(); y < region.bottom(); ++y) {
    Pixel* const row = dst.get_row(y) + region.left();
    std::fill_n(row, region.width(), pixel);
  }
}

template<typename Pixel>
void fill_rect__slow(PixelData<Pixel>& dst, geom::irect const& rect, Pixel const& pixel)
{
  geom::irect const region = geom::intersection(geom::irect(dst.get_size()), rect);

  for (int y = region.top(); y < region.bottom(); ++y) {
    Pixel* const row = dst.get_row(y);

    for (int x = region.left(); x < region.right(); ++x) {
      row[x] = pixel;
    }
  }
}

} // namespace detail

template<typename Pixel>
void fill(PixelData<Pixel>& dst, Pixel const& pixel)
{
  surf::detail::fill__slow<Pixel>(dst, pixel);
}

template<typename Pixel>
void fill_rect(PixelData<Pixel>& dst, geom::irect const& rect, Pixel const& pixel)
{
  surf::detail::fill_rect__slow<Pixel>(dst, rect, pixel);
}

} // namespace surf

#endif

/* EOF */
