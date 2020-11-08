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

#ifndef HEADER_SURF_PIXEL_HPP
#define HEADER_SURF_PIXEL_HPP

#include <stdint.h>

#include <bit>
#include <limits>

#include "pixel_format.hpp"

namespace surf {

template<typename T>
struct tRGBPixel
{
  using value_type = T;
  static constexpr bool has_alpha() { return false; }
  static constexpr T max() { return std::numeric_limits<T>::max(); }

  T r;
  T g;
  T b;

  inline bool operator==(tRGBPixel<T> const& rhs) const = default;
};

using RGBPixel = tRGBPixel<uint8_t>;

static_assert(sizeof(RGBPixel) == 3);
static_assert(std::is_trivial<RGBPixel>::value);

template<typename T>
struct tRGBAPixel
{
  using value_type = T;
  static constexpr bool has_alpha() { return true; }
  static constexpr T max() { return std::numeric_limits<T>::max(); }

  T r;
  T g;
  T b;
  T a;

  inline bool operator==(tRGBAPixel<T> const& rhs) const = default;
};

using RGBAPixel = tRGBAPixel<uint8_t>;

static_assert(sizeof(RGBAPixel) == 4);
static_assert(std::is_trivial<RGBAPixel>::value);

template<typename T>
struct tGreyscalePixel
{
  using value_type = T;
  static constexpr T max() { return std::numeric_limits<T>::max(); }

  T value;

  inline bool operator==(tGreyscalePixel<T> const& rhs) const = default;
};

using GreyscalePixel = tGreyscalePixel<uint8_t>;

static_assert(sizeof(GreyscalePixel) == 1);
static_assert(std::is_trivial<GreyscalePixel>::value);

template<typename Pixel>
struct PPixelFormat
{
};

template<>
struct PPixelFormat<RGBPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB;
  static constexpr int bits_per_pixel = 24;
  static constexpr int bytes_per_pixel = 3;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0x00ff0000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0x0000ff00 : 0x0000ff00; // NOLINT
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0x000000ff : 0x00ff0000;
  static constexpr uint32_t amask = 0x00000000;
};

template<>
struct PPixelFormat<RGBAPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA;
  static constexpr int bits_per_pixel = 32;
  static constexpr int bytes_per_pixel = 4;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0x00ff0000 : 0x0000ff00;
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0x0000ff00 : 0x00ff0000;
  static constexpr uint32_t amask = std::endian::native == std::endian::big ? 0x000000ff : 0xff000000;
};

template<>
struct PPixelFormat<GreyscalePixel>
{
  static constexpr int bits_per_pixel = 8;
  static constexpr int bytes_per_pixel = 1;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
  static constexpr uint32_t amask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
};

} // namespace surf

#endif

/* EOF */
