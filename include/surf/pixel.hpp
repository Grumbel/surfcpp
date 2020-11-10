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

template<typename Pixel> inline Pixel::value_type red(Pixel pixel) { return pixel.r; }
template<typename Pixel> inline Pixel::value_type green(Pixel pixel) { return pixel.g; }
template<typename Pixel> inline Pixel::value_type blue(Pixel pixel) { return pixel.b; }

template<typename Pixel> inline Pixel::value_type alpha(Pixel pixel) {
  if constexpr (Pixel::has_alpha()) {
    return pixel.a;
  } else {
    return Pixel::max();
  }
}

template<typename T>
struct tRGBPixel
{
  using value_type = T;
  static constexpr bool has_alpha() { return false; }
  static constexpr T max() {
    if constexpr (std::is_floating_point<T>::value) {
      return 1.0;
    } else {
      return std::numeric_limits<T>::max();
    }
  }

  T r;
  T g;
  T b;

  inline bool operator==(tRGBPixel<T> const& rhs) const = default;
};

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

using RGB8Pixel = tRGBPixel<uint8_t>;
using RGBA8Pixel = tRGBAPixel<uint8_t>;

using RGB16Pixel = tRGBPixel<uint16_t>;
using RGBA16Pixel = tRGBAPixel<uint16_t>;

using RGB32Pixel = tRGBPixel<uint32_t>;
using RGBA32Pixel = tRGBAPixel<uint32_t>;

using RGB32fPixel = tRGBPixel<float>;
using RGBA32fPixel = tRGBAPixel<float>;

using RGB64fPixel = tRGBPixel<double>;
using RGBA64fPixel = tRGBAPixel<double>;

using RGBPixel = RGB8Pixel;
using RGBAPixel = RGBA8Pixel;

static_assert(sizeof(RGBAPixel) == 4);
static_assert(std::is_trivial<RGBAPixel>::value);

static_assert(sizeof(RGBPixel) == 3);
static_assert(std::is_trivial<RGBPixel>::value);

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
struct PPixelFormat<RGB8Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB8;
  static constexpr int bits_per_pixel = 24;
  static constexpr int bytes_per_pixel = 3;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0x00ff0000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0x0000ff00 : 0x0000ff00; // NOLINT
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0x000000ff : 0x00ff0000;
  static constexpr uint32_t amask = 0x00000000;
};

template<>
struct PPixelFormat<RGBA8Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA8;
  static constexpr int bits_per_pixel = 32;
  static constexpr int bytes_per_pixel = 4;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0xff000000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0x00ff0000 : 0x0000ff00;
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0x0000ff00 : 0x00ff0000;
  static constexpr uint32_t amask = std::endian::native == std::endian::big ? 0x000000ff : 0xff000000;
};

template<>
struct PPixelFormat<RGB16Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB16;
  static constexpr int bits_per_pixel = 48;
  static constexpr int bytes_per_pixel = 6;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGBA16Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA16;
  static constexpr int bits_per_pixel = 64;
  static constexpr int bytes_per_pixel = 8;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGB32Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB32;
  static constexpr int bits_per_pixel = 96;
  static constexpr int bytes_per_pixel = 12;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGBA32Pixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA32;
  static constexpr int bits_per_pixel = 128;
  static constexpr int bytes_per_pixel = 16;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGB32fPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB32f;
  static constexpr int bits_per_pixel = 96;
  static constexpr int bytes_per_pixel = 12;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGBA32fPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA32f;
  static constexpr int bits_per_pixel = 128;
  static constexpr int bytes_per_pixel = 16;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGB64fPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGB64f;
  static constexpr int bits_per_pixel = 96;
  static constexpr int bytes_per_pixel = 12;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
};

template<>
struct PPixelFormat<RGBA64fPixel>
{
  static constexpr PixelFormat format = PixelFormat::RGBA64f;
  static constexpr int bits_per_pixel = 128;
  static constexpr int bytes_per_pixel = 16;
  static constexpr uint32_t rmask = 0;
  static constexpr uint32_t gmask = 0;
  static constexpr uint32_t bmask = 0;
  static constexpr uint32_t amask = 0;
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
