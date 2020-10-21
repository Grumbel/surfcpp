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

#ifndef HEADER_GALAPIX_UTIL_PIXEL_DATA_HPP
#define HEADER_GALAPIX_UTIL_PIXEL_DATA_HPP

#include <stdint.h>
#include <vector>

#include <geom/size.hpp>
#include <geom/point.hpp>

#include "pixel_format.hpp"

namespace surf {

class RGB;
class RGBA;

/** A mutable low-level container for pixel data */
class PixelData
{
public:
  PixelData();
  PixelData(PixelFormat format, const geom::isize& size);

  PixelFormat get_format() const { return m_format; }
  geom::isize get_size() const { return m_size; }
  int get_width() const { return m_size.width(); }
  int get_height() const { return m_size.height(); }
  int get_pitch() const { return m_pitch; }
  int get_bytes_per_pixel() const;

  bool empty() const { return m_pixels.empty(); }

  void put_pixel(geom::ipoint const& position, RGB const& rgb);
  void put_pixel(geom::ipoint const& position, RGBA const& rgb);

  void get_pixel(geom::ipoint const& position, RGB& rgb) const;
  void get_pixel(geom::ipoint const& position, RGBA& rgb) const;

  uint8_t* get_data();
  uint8_t* get_row_data(int y);

  uint8_t const* get_data() const;
  uint8_t const* get_row_data(int y) const;

  /** Performs a simple copy from this to \a test, no blending is performed */
  void blit(PixelData& dst, const geom::ipoint& pos) const;

private:
  PixelFormat m_format;
  geom::isize m_size;
  int m_pitch;
  std::vector<uint8_t> m_pixels;
};

} // namespace surf

#endif

/* EOF */
