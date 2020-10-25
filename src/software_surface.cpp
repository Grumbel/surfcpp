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
#include "pixel_data_factory.hpp"

namespace surf {

namespace {

PixelDataFactory g_pixeldata_fatory;

} // namespace

SoftwareSurface
SoftwareSurface::from_file(std::filesystem::path const& filename)
{
  return g_pixeldata_fatory.from_file(filename);
}

SoftwareSurface
SoftwareSurface::from_file(std::filesystem::path const& filename, std::string_view loader)
{
  return g_pixeldata_fatory.from_file(filename, loader);
}

SoftwareSurface
SoftwareSurface::create(PixelFormat format, geom::isize const& size, Color const& color)
{
  switch (format)
  {
    case PixelFormat::RGB:
      return SoftwareSurface(PixelData<RGBPixel>(size, convert<Color, RGBPixel>(color)));

    case PixelFormat::RGBA:
      return SoftwareSurface(PixelData<RGBAPixel>(size, convert<Color, RGBAPixel>(color)));

    default:
      throw std::runtime_error("unsupported PixelFormat");
  }
}

SoftwareSurface::SoftwareSurface() :
  m_pixel_data()
{
}

geom::isize
SoftwareSurface::get_size()  const
{
  if (!m_pixel_data) { return {0, 0}; }

  return m_pixel_data->get_size();
}

int
SoftwareSurface::get_width()  const
{
  if (!m_pixel_data) { return 0; }

  return m_pixel_data->get_width();
}

int
SoftwareSurface::get_height() const
{
  if (!m_pixel_data) { return 0; }

  return m_pixel_data->get_height();
}

PixelFormat
SoftwareSurface::get_format() const
{
  if (!m_pixel_data) { return PixelFormat::NONE; }

  return m_pixel_data->get_format();
}

void*
SoftwareSurface::get_data()
{
  if (!m_pixel_data) { return nullptr; }

  return m_pixel_data->get_row_data(0);
}

void*
SoftwareSurface::get_row_data(int y)
{
  if (!m_pixel_data) { return nullptr; }

  return m_pixel_data->get_row_data(y);
}

void const*
SoftwareSurface::get_data() const
{
  if (!m_pixel_data) { return nullptr; }

  return m_pixel_data->get_row_data(0);
}

void const*
SoftwareSurface::get_row_data(int y) const
{
  if (!m_pixel_data) { return nullptr; }

  return m_pixel_data->get_row_data(y);
}

Color
SoftwareSurface::get_pixel(geom::ipoint const& position) const
{
  return m_pixel_data->get_pixel_color(position);
}

} // namespace surf

/* EOF */
