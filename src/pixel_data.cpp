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

#include "pixel_data.hpp"

#include <string.h>

#include "rgb.hpp"
#include "rgba.hpp"

namespace surf {

PixelData
PixelData::from_file(std::filesystem::path const& filename)
{
  return PixelData(); //g_pixel_factory.from_file(filename);
}

// FIXME: Stuff in this file is currently written to just work, not to
// be fast

PixelData::PixelData() :
  m_format(PixelFormat::RGB),
  m_size(0, 0),
  m_pitch(0),
  m_pixels()
{
}

PixelData::PixelData(PixelFormat format, geom::isize const& size) :
  m_format(format),
  m_size(size),
  m_pitch(),
  m_pixels()
{
  switch(format)
  {
    case PixelFormat::RGB:
      m_pitch  = m_size.width() * 3;
      m_pixels.resize(m_pitch * m_size.height());
      break;

    case PixelFormat::RGBA:
      m_pitch  = size.width() * 4;
      m_pixels.resize(m_pitch * m_size.height());
      break;

    default:
      assert(false && "PixelData: Unknown color format");
  }
}

int
PixelData::get_bytes_per_pixel() const
{
  switch(m_format)
  {
    case PixelFormat::RGB:
      return 3;

    case PixelFormat::RGBA:
      return 4;

    default:
      assert(false && "SoftwareSurface::get_bytes_per_pixel(): Unknown format");
      return 0;
  }
}

void
PixelData::put_pixel(geom::ipoint const& pos, RGBA const& rgba)
{
  assert(geom::contains(m_size, pos));

  int const x = pos.x();
  int const y = pos.y();

  switch(m_format)
  {
    case PixelFormat::RGBA:
      m_pixels[y * m_pitch + x*4 + 0] = rgba.r;
      m_pixels[y * m_pitch + x*4 + 1] = rgba.g;
      m_pixels[y * m_pitch + x*4 + 2] = rgba.b;
      m_pixels[y * m_pitch + x*4 + 3] = rgba.a;
      break;

    case PixelFormat::RGB:
      m_pixels[y * m_pitch + x*3 + 0] = rgba.r;
      m_pixels[y * m_pitch + x*3 + 1] = rgba.g;
      m_pixels[y * m_pitch + x*3 + 2] = rgba.b;
      break;
  }
}

void
PixelData::PixelData::put_pixel(geom::ipoint const& pos, RGB const& rgb)
{
  assert(m_format == PixelFormat::RGB);
  assert(geom::contains(m_size, pos));

  int const x = pos.x();
  int const y = pos.y();

  m_pixels[y * m_pitch + x*3 + 0] = rgb.r;
  m_pixels[y * m_pitch + x*3 + 1] = rgb.g;
  m_pixels[y * m_pitch + x*3 + 2] = rgb.b;
}

void
PixelData::get_pixel(geom::ipoint const& pos, RGBA& rgb) const
{
  assert(geom::contains(m_size, pos));

  int const x = pos.x();
  int const y = pos.y();

  switch(m_format)
  {
    case PixelFormat::RGBA:
      rgb.r = m_pixels[y * m_pitch + x*4 + 0];
      rgb.g = m_pixels[y * m_pitch + x*4 + 1];
      rgb.b = m_pixels[y * m_pitch + x*4 + 2];
      rgb.a = m_pixels[y * m_pitch + x*4 + 3];
      break;

    case PixelFormat::RGB:
      rgb.r = m_pixels[y * m_pitch + x*3 + 0];
      rgb.g = m_pixels[y * m_pitch + x*3 + 1];
      rgb.b = m_pixels[y * m_pitch + x*3 + 2];
      rgb.a = 255;
      break;
  }
}

void
PixelData::get_pixel(geom::ipoint const& pos, RGB& rgb) const
{
  assert(m_format == PixelFormat::RGB);
  assert(geom::contains(m_size, pos));

  int const x = pos.x();
  int const y = pos.y();

  rgb.r = m_pixels[y * m_pitch + x*3 + 0];
  rgb.g = m_pixels[y * m_pitch + x*3 + 1];
  rgb.b = m_pixels[y * m_pitch + x*3 + 2];
}

uint8_t*
PixelData::get_data()
{
  return m_pixels.data();
}

uint8_t*
PixelData::get_row_data(int y)
{
  return m_pixels.data() + (y * m_pitch);
}

uint8_t const*
PixelData::get_data() const
{
  return m_pixels.data();
}

uint8_t const*
PixelData::get_row_data(int y) const
{
  return m_pixels.data() + (y * m_pitch);
}

void
PixelData::blit_to(PixelData& dst, const geom::ipoint& pos) const
{
  int start_x = std::max(0, -pos.x());
  int start_y = std::max(0, -pos.y());

  int end_x = std::min(m_size.width(),  dst.m_size.width()  - pos.x());
  int end_y = std::min(m_size.height(), dst.m_size.height() - pos.y());

  if (dst.m_format == PixelFormat::RGB && m_format == PixelFormat::RGB)
  {
    for(int y = start_y; y < end_y; ++y)
    {
      memcpy(dst.get_row_data(y + pos.y()) + (pos.x() + start_x)*3,
             get_row_data(y) + start_x*3,
             (end_x - start_x) * 3);
    }
  }
  else if (dst.m_format == PixelFormat::RGBA && m_format == PixelFormat::RGBA)
  {
    for(int y = start_y; y < end_y; ++y)
    {
      memcpy(dst.get_row_data(y + pos.y()) + (pos.x() + start_x)*4,
             get_row_data(y) + start_x*4,
             (end_x - start_x) * 4);
    }
  }
  else if (dst.m_format == PixelFormat::RGBA && m_format == PixelFormat::RGB)
  {
    for(int y = start_y; y < end_y; ++y)
    {
      uint8_t* dstpx = dst.get_row_data(y + pos.y()) + (pos.x() + start_x)*4;
      uint8_t const* srcpx = get_row_data(y) + start_x*3;

      for(int x = 0; x < (end_x - start_x); ++x)
      {
        dstpx[4*x+0] = srcpx[3*x+0];
        dstpx[4*x+1] = srcpx[3*x+1];
        dstpx[4*x+2] = srcpx[3*x+2];
        dstpx[4*x+3] = 255;
      }
    }
  }
  else if (dst.m_format == PixelFormat::RGB && m_format == PixelFormat::RGBA)
  {
    for(int y = start_y; y < end_y; ++y)
    {
      uint8_t* dstpx = dst.get_row_data(y + pos.y()) + (pos.x() + start_x)*3;
      uint8_t const* srcpx = get_row_data(y) + start_x*4;

      for(int x = 0; x < (end_x - start_x); ++x)
      {
        uint8_t alpha = srcpx[4*x+3];
        dstpx[3*x+0] = static_cast<uint8_t>(srcpx[4*x+0] * alpha / 255);
        dstpx[3*x+1] = static_cast<uint8_t>(srcpx[4*x+1] * alpha / 255);
        dstpx[3*x+2] = static_cast<uint8_t>(srcpx[4*x+2] * alpha / 255);
      }
    }
  }
  else
  {
    assert(false && "Not m_implemented");
  }
}

void
PixelData::fill(geom::irect const& rect, RGBA& rgba)
{
  // FIXME: implement me
}

} // namespace surf

/* EOF */