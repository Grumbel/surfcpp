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

#ifndef HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_HPP
#define HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_HPP

#include <stdint.h>
#include <memory>
#include <geom/fwd.hpp>

#include "pixel_data.hpp"
#include "transform.hpp"

namespace surf {

class RGB;
class RGBA;

/** A non-mutable high-level SoftwareSurface with value semantics */
class SoftwareSurface
{
public:
  static SoftwareSurface from_file(std::filesystem::path const& filename);

public:
  SoftwareSurface();
  SoftwareSurface(PixelData data);

  geom::isize get_size() const;
  int get_width() const;
  int get_height() const;
  PixelFormat get_format() const;

  SoftwareSurface to_rgb() const;
  RGB get_average_color() const;

  void get_pixel(geom::ipoint const& position, RGB& rgb) const;
  void get_pixel(geom::ipoint const& position, RGBA& rgba) const;

  int get_bytes_per_pixel() const;
  int get_bits_per_pixel() const;

  uint8_t const* get_data() const;
  uint8_t const* get_row_data(int y) const;

  PixelData const& get_pixel_data() const { return *m_pixel_data; }

private:
  std::shared_ptr<const PixelData> m_pixel_data;
};

} // namespace surf

#endif

/* EOF */
