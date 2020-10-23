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

SoftwareSurface
SoftwareSurface::from_file(std::filesystem::path const& filename)
{
  PixelData pixel_data = PixelData::from_file(filename);
  return SoftwareSurface(std::move(pixel_data));
}

SoftwareSurface::SoftwareSurface() :
  m_pixel_data(std::make_shared<PixelData>())
{
}

SoftwareSurface::SoftwareSurface(PixelData data) :
  m_pixel_data(std::make_shared<PixelData>(std::move(data)))
{
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

PixelFormat
SoftwareSurface::get_format() const
{
  return m_pixel_data->get_format();
}

int
SoftwareSurface::get_bytes_per_pixel() const
{
  return m_pixel_data->get_bytes_per_pixel();
}

int
SoftwareSurface::get_bits_per_pixel() const
{
  return m_pixel_data->get_bytes_per_pixel() / 8;
}

uint8_t const*
SoftwareSurface::get_data() const
{
  return m_pixel_data->get_data();
}

uint8_t const*
SoftwareSurface::get_row_data(int y) const
{
  return m_pixel_data->get_row_data(y);
}

void
SoftwareSurface::get_pixel(geom::ipoint const& position, RGB& rgb) const
{
  m_pixel_data->get_pixel(position, rgb);
}

void
SoftwareSurface::get_pixel(geom::ipoint const& position, RGBA& rgba) const
{
  m_pixel_data->get_pixel(position, rgba);
}

} // namespace surf

/* EOF */
