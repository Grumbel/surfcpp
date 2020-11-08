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
#include <logmich/log.hpp>

#include "blit.hpp"
#include "fill.hpp"
#include "convert.hpp"
#include "pixel.hpp"
#include "software_surface_factory.hpp"

namespace surf {

namespace {

SoftwareSurfaceFactory g_pixeldata_fatory;

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
    case PixelFormat::RGB8:
      return SoftwareSurface(PixelData<RGB8Pixel>(size, convert<Color, RGB8Pixel>(color)));

    case PixelFormat::RGBA8:
      return SoftwareSurface(PixelData<RGBA8Pixel>(size, convert<Color, RGBA8Pixel>(color)));

    case PixelFormat::RGB16:
      return SoftwareSurface(PixelData<RGB16Pixel>(size, convert<Color, RGB16Pixel>(color)));

    case PixelFormat::RGBA16:
      return SoftwareSurface(PixelData<RGBA16Pixel>(size, convert<Color, RGBA16Pixel>(color)));

    case PixelFormat::RGB32:
      return SoftwareSurface(PixelData<RGB32Pixel>(size, convert<Color, RGB32Pixel>(color)));

    case PixelFormat::RGBA32:
      return SoftwareSurface(PixelData<RGBA32Pixel>(size, convert<Color, RGBA32Pixel>(color)));

    default:
      throw std::runtime_error("unsupported PixelFormat");
  }
}

SoftwareSurface
SoftwareSurface::create_view(PixelFormat format, geom::isize const& size, void* ptr, int pitch)
{
  PIXELFORMAT_TO_TYPE(
    format,
    pixeltype,
    log_unreachable(); return {},
    return SoftwareSurface(std::make_unique<PixelView<pixeltype>>(
                             size, static_cast<pixeltype*>(ptr), pitch / sizeof(pixeltype))));
}

SoftwareSurface
SoftwareSurface::create_view(PixelFormat format, geom::isize const& size, void const* ptr, int pitch)
{
  PIXELFORMAT_TO_TYPE(
    format,
    pixeltype,
    log_unreachable(); return {},
    return SoftwareSurface(std::make_unique<PixelView<pixeltype>>(
                             size, static_cast<pixeltype const*>(ptr), pitch / sizeof(pixeltype))));
}

SoftwareSurface::SoftwareSurface() :
  m_pixel_data()
{
}

SoftwareSurface::SoftwareSurface(SoftwareSurface const& other) :
  m_pixel_data(other.m_pixel_data->copy())
{
}

SoftwareSurface&
SoftwareSurface::operator=(SoftwareSurface const& other)
{
  m_pixel_data = other.m_pixel_data->copy();
  return *this;
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

int
SoftwareSurface::get_pitch() const
{
  if (!m_pixel_data) { return 0; }

  return m_pixel_data->get_pitch();
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

SoftwareSurface
SoftwareSurface::get_view(geom::irect const& rect) const
{
  return SoftwareSurface(m_pixel_data->create_view(rect));
}

void blit(SoftwareSurface const& src, SoftwareSurface& dst, geom::ipoint const& pos)
{
  SOFTWARE_SURFACE_UNWRAP(
    src,
    src_as_pixeldata,
    log_unreachable(),
    SOFTWARE_SURFACE_UNWRAP(
      dst,
      dst_as_pixeldata,
      log_unreachable(),
      blit(src_as_pixeldata, dst_as_pixeldata, pos)));
}

void blit(SoftwareSurface const& src, geom::irect const& srcrect,
          SoftwareSurface& dst, geom::ipoint const& pos)
{
  SOFTWARE_SURFACE_UNWRAP(
    src,
    src_as_pixeldata,
    log_unreachable(),
    SOFTWARE_SURFACE_UNWRAP(
      dst,
      dst_as_pixeldata,
      log_unreachable(),
      blit(src_as_pixeldata, srcrect, dst_as_pixeldata, pos)));
}

void fill(SoftwareSurface& dst, Color const& color)
{
  PIXELFORMAT_TO_TYPE(
    dst.get_format(),
    dsttype,
    log_unreachable(),
    fill(dst.as_pixelview<dsttype>(), convert<Color, dsttype>(color)));
}

void fill_rect(SoftwareSurface& dst, geom::irect const& rect, Color const& color)
{
  PIXELFORMAT_TO_TYPE(
    dst.get_format(),
    dsttype,
    log_unreachable(),
    fill_rect(dst.as_pixelview<dsttype>(), rect, convert<Color, dsttype>(color)));
}

SoftwareSurface convert(SoftwareSurface const& src, PixelFormat format)
{
  SOFTWARE_SURFACE_UNWRAP(
    src,
    src_as_pixelview,
    return {},
    PIXELFORMAT_TO_TYPE(
      src.get_format(),
      pixel_type,
      return {},
      return SoftwareSurface(src_as_pixelview.template convert_to<pixel_type>())));
}

} // namespace surf

/* EOF */
