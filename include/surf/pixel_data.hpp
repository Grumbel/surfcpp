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

#include <iosfwd>
#include <stdint.h>
#include <filesystem>
#include <string_view>
#include <vector>

#include <geom/point.hpp>
#include <geom/rect.hpp>
#include <geom/size.hpp>
#include <logmich/log.hpp>

#include "rgba.hpp"
#include "pixel_format.hpp"

namespace surf {

class RGB;
class RGBA;

struct RGBPixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct RGBAPixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct GreyscalePixel
{
  uint8_t value;
};

template<typename Pixel>
struct PPixelFormat
{
};

template<>
struct PPixelFormat<RGBPixel>
{
  static constexpr int bits_per_pixel = 24;
  static constexpr int bytes_per_pixel = 3;
  static constexpr uint32_t rmask = std::endian::native == std::endian::big ? 0x00ff0000 : 0x000000ff;
  static constexpr uint32_t gmask = std::endian::native == std::endian::big ? 0x0000ff00 : 0x0000ff00;
  static constexpr uint32_t bmask = std::endian::native == std::endian::big ? 0x000000ff : 0x00ff0000;
  static constexpr uint32_t amask = std::endian::native == std::endian::big ? 0x00000000 : 0x00000000;
};

template<>
struct PPixelFormat<RGBAPixel>
{
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

template<typename SrcPixel, typename DstPixel>
DstPixel convert(SrcPixel src) {
  static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
                "convert<>() not implemented for the given types");
}

template<> inline
RGBPixel convert<RGBAPixel, RGBPixel>(RGBAPixel src) {
  return RGBPixel{src.r, src.g, src.b};
}

template<> inline
RGBAPixel convert<RGBPixel, RGBAPixel>(RGBPixel src) {
  return RGBAPixel{src.r, src.g, src.b, 255};
}

template<> inline
GreyscalePixel convert<RGBPixel, GreyscalePixel>(RGBPixel src) {
  // FIXME: add proper weighting of colors here
  return {static_cast<uint8_t>((src.r + src.g + src.b) / 3)};
}

template<> inline
RGBPixel convert<GreyscalePixel, RGBPixel>(GreyscalePixel src) {
  // FIXME: add proper weighting of colors here
  return {src.value, src.value, src.value};
}

template<> inline
RGBAPixel convert<GreyscalePixel, RGBAPixel>(GreyscalePixel src) {
  // FIXME: add proper weighting of colors here
  return {src.value, src.value, src.value, 255};
}

inline
bool operator==(RGBAPixel const& lhs, RGBAPixel const& rhs)
{
  return (lhs.r == rhs.r &&
          lhs.g == rhs.g &&
          lhs.b == rhs.b &&
          lhs.a == rhs.a);
}

inline
bool operator==(RGBPixel const& lhs, RGBPixel const& rhs)
{
  return (lhs.r == rhs.r &&
          lhs.g == rhs.g &&
          lhs.b == rhs.b);
}

class IPixelData
{
public:
  virtual ~IPixelData() {}
  virtual geom::isize get_size() const = 0;
  virtual int get_width() const = 0;
  virtual int get_height() const = 0;
  virtual int get_row_length() const = 0;
  virtual bool empty() const = 0;
};

/** A mutable low-level container for pixel data */
template<typename Pixel>
class PixelData : public IPixelData
{
public:
  /*
  static PixelData<T> from_file(std::filesystem::path const& filename);
  static PixelData<T> from_file(std::filesystem::path const& filename, std::string_view loader);
  static PixelData<T> create(PixelFormat format, const geom::isize& size, RGBA const& rgba = RGBA(0, 0, 0, 0));
  */
public:
  using value_type = Pixel;

public:
  PixelData() :
    m_size(0, 0),
    m_row_length(0),
    m_pixels()
  {}

  PixelData(geom::isize const& size, Pixel const& pixel = {}) :
    m_size(size),
    m_row_length(m_size.width()),
    m_pixels(m_size.width() * m_size.height(), pixel)
  {}

  PixelData(geom::isize const& size, std::vector<Pixel> pixels) :
    m_size(size),
    m_row_length(m_size.width()),
    m_pixels(std::move(pixels))
  {}

  geom::isize get_size() const override { return m_size; }
  int get_width() const override { return m_size.width(); }
  int get_height() const override { return m_size.height(); }
  int get_row_length() const override { return m_row_length; }

  bool empty() const override { return m_pixels.empty(); }

  void put_pixel(geom::ipoint const& pos, Pixel const& pixel)
  {
    assert(geom::contains(m_size, pos));
    m_pixels[pos.y() * m_row_length + pos.x()] = pixel;
  }

  Pixel get_pixel(geom::ipoint const& pos) const
  {
    assert(geom::contains(m_size, pos));
    return m_pixels[pos.y() * m_row_length + pos.x()];
  }

  Pixel* get_data() {
    return m_pixels.data();
  }

  Pixel* get_row(int y) {
    return m_pixels.data() + (y * m_row_length);
  }

  Pixel const* get_data() const {
    return m_pixels.data();
  }

  Pixel const* get_row(int y) const {
    return m_pixels.data() + (y * m_row_length);
  }

  /** Performs a simple copy from this to \a test, no blending is performed */
  void blit_to(PixelData<Pixel>& dst, const geom::ipoint& pos) const
  {
    int const start_x = std::max(0, -pos.x());
    int const start_y = std::max(0, -pos.y());

    int const end_x = std::min(m_size.width(),  dst.m_size.width() - pos.x());
    int const end_y = std::min(m_size.height(), dst.m_size.height() - pos.y());

    for(int y = start_y; y < end_y; ++y)
    {
      memcpy(dst.get_row(pos.y() + y - start_y) + pos.x(),
             get_row(y),
             (end_x - start_x) * sizeof(Pixel));
    }
  }

  template<typename DstPixel>
  void blit_to(PixelData<DstPixel>& dst, const geom::ipoint& pos) const
  {
    int const start_x = std::max(0, -pos.x());
    int const start_y = std::max(0, -pos.y());

    int const end_x = std::min(m_size.width(), dst.get_size().width()  - pos.x());
    int const end_y = std::min(m_size.height(), dst.get_size().height() - pos.y());

    for(int y = start_y; y < end_y; ++y) {
      for(int x = start_x; x < end_x; ++x) {
        Pixel const srcpx = get_pixel(geom::ipoint(x, y));
        dst.put_pixel(geom::ipoint(pos.x() + x - start_x, pos.y() + y - start_y), convert<Pixel, DstPixel>(srcpx));
      }
    }
  }

  void blit_to(geom::irect& srcrect, PixelData<Pixel>& dst, const geom::ipoint& pos) const
  {
    log_not_implemented();
  }

  void fill(Pixel const& pixel)
  {
    for (int y = 0; y < m_size.height(); ++y) {
      Pixel* row = get_row(y);
      for (int x = 0; x < m_size.width(); ++x) {
        row[x] = pixel;
      }
    }
  }

  void fill_rect(geom::irect const& rect, Pixel const& pixel)
  {
    geom::irect const rect_clipped = geom::intersection(geom::irect(m_size), rect);

    for (int y = rect_clipped.top(); y < rect_clipped.bottom(); ++y) {
      Pixel* const row = get_row(y) + rect_clipped.left() * 4;
      for (int x = 0; x < rect_clipped.width(); ++x) {
        row[x + 0] = pixel;
      }
    }
  }

  bool operator==(PixelData<Pixel> const& data) const {
    return (m_size == data.m_size &&
            m_row_length == data.m_row_length &&
            m_pixels == data.m_pixels);
  }

private:
  geom::isize m_size;
  int m_row_length;
  std::vector<Pixel> m_pixels;
};

inline
std::ostream& operator<<(std::ostream& os, RGBPixel const& pixel)
{
  return os << fmt::format("({:02x} {:02x} {:02x})",
                           static_cast<int>(pixel.r),
                           static_cast<int>(pixel.g),
                           static_cast<int>(pixel.b));
}

inline
std::ostream& operator<<(std::ostream& os, RGBAPixel const& pixel)
{
  return os << fmt::format("({:02x} {:02x} {:02x} {:02x})",
                           static_cast<int>(pixel.r),
                           static_cast<int>(pixel.g),
                           static_cast<int>(pixel.b),
                           static_cast<int>(pixel.a));
}

template<typename Pixel>
std::ostream& operator<<(std::ostream& os, PixelData<Pixel> const& pixeldata)
{
  os << "\n{";
  for (int y = 0; y < pixeldata.get_height(); ++y) {
    if (y != 0) {
      os << " ";
    }
    os << " { ";
    for (int x = 0; x < pixeldata.get_width(); ++x) {
      Pixel pixel = pixeldata.get_pixel(geom::ipoint(x, y));
      os << pixel;
      if (x != pixeldata.get_width() - 1) {
        os << ' ';
      }
    }
    if (y == pixeldata.get_height() - 1) {
      os << " }";
    }
    os << " }\n";
  }
  return os;
}

} // namespace surf

#endif

/* EOF */
