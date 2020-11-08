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

#ifndef HEADER_SURF_PIXEL_FORMAT_HPP
#define HEADER_SURF_PIXEL_FORMAT_HPP

#include  <bit>
#include <stdint.h>

namespace surf {

namespace detail {

enum class PixelFormatName
{
  NONE,

  RGB8,
  RGBA8,
  RGB16,
  RGBA16,
  RGB32,
  RGBA32,

  RGB = RGB8,
  RGBA = RGBA8
};

struct PixelFormatImpl
{
  PixelFormatName const name;
  int const bits_per_pixel;
  int const bytes_per_pixel;
  uint32_t const rmask;
  uint32_t const gmask;
  uint32_t const bmask;
  uint32_t const amask;

  constexpr operator PixelFormatName() const { return name; }
};

} // namespace detail

class PixelFormat
{
public:
  static constexpr detail::PixelFormatImpl RGB8{detail::PixelFormatName::RGB8, 24, 3,
      std::endian::native == std::endian::big ? 0x00ff0000 : 0x000000ff,
      std::endian::native == std::endian::big ? 0x0000ff00 : 0x0000ff00, // NOLINT
      std::endian::native == std::endian::big ? 0x000000ff : 0x00ff0000,
      0x00000000};
  static constexpr detail::PixelFormatImpl RGBA8{detail::PixelFormatName::RGBA8, 32, 4,
    std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff,
    std::endian::native == std::endian::big ? 0x00ff0000 : 0x0000ff00,
    std::endian::native == std::endian::big ? 0x0000ff00 : 0x00ff0000,
    std::endian::native == std::endian::big ? 0x000000ff : 0xff000000};

  static constexpr detail::PixelFormatImpl RGB16{detail::PixelFormatName::RGB16, 48, 6, 0, 0, 0, 0};
  static constexpr detail::PixelFormatImpl RGBA16{detail::PixelFormatName::RGBA16, 64, 8, 0, 0, 0, 0};

  static constexpr detail::PixelFormatImpl RGB32{detail::PixelFormatName::RGB32, 96, 12, 0, 0, 0, 0};
  static constexpr detail::PixelFormatImpl RGBA32{detail::PixelFormatName::RGBA32, 128, 16, 0, 0, 0, 0};

  static constexpr detail::PixelFormatImpl NONE{detail::PixelFormatName::NONE, 0, 0, 0, 0, 0, 0};

  static constexpr detail::PixelFormatImpl RGB = RGB8;
  static constexpr detail::PixelFormatImpl RGBA = RGBA8;

public:
  constexpr PixelFormat() : m_desc(nullptr) {}
  constexpr PixelFormat(detail::PixelFormatImpl const& desc) :
    m_desc(&desc)
  {}

  constexpr int bits_per_pixel() const { return m_desc->bits_per_pixel; }
  constexpr int bytes_per_pixel() const { return m_desc->bytes_per_pixel; }

  constexpr uint32_t rmask() const { return m_desc->rmask; }
  constexpr uint32_t gmask() const { return m_desc->gmask; }
  constexpr uint32_t bmask() const { return m_desc->bmask; }
  constexpr uint32_t amask() const { return m_desc->amask; }

  constexpr operator detail::PixelFormatName() const { return m_desc->name; }

private:
  detail::PixelFormatImpl const* m_desc;
};

} // namespace surf

#endif

/* EOF */
