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

#ifndef HEADER_SURF_PIXEL_DATA_HPP
#define HEADER_SURF_PIXEL_DATA_HPP

#include <stdint.h>
#include <string.h>

#include <memory>
#include <ostream>
#include <vector>

#include <geom/point.hpp>
#include <geom/rect.hpp>
#include <geom/size.hpp>

#include "color.hpp"
#include "convert.hpp"
#include "ipixel_data.hpp"
#include "pixel.hpp"
#include "pixel_format.hpp"

namespace surf {

/** A mutable low-level container for pixel data */
template<typename Pixel>
class PixelData : public IPixelData
{
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

  PixelFormat get_format() const override { return PPixelFormat<Pixel>::format; }

  geom::isize get_size() const override { return m_size; }
  int get_width() const override { return m_size.width(); }
  int get_height() const override { return m_size.height(); }
  int get_row_length() const override { return m_row_length; }
  int get_pitch() const override { return m_row_length * sizeof(Pixel); }

  bool empty() const override { return m_pixels.empty(); }

  void put_pixel(geom::ipoint const& pos, Pixel const& pixel)
  {
    assert(geom::contains(m_size, pos));
    m_pixels[pos.y() * m_row_length + pos.x()] = pixel;
  }

  Color get_pixel_color(geom::ipoint const& pos) const override
  {
    return convert<Pixel, Color>(get_pixel(pos));
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

  void* get_row_data(int y) override {
    return get_row(y);
  }

  Pixel const* get_data() const {
    return m_pixels.data();
  }

  Pixel const* get_row(int y) const {
    return m_pixels.data() + (y * m_row_length);
  }

  void const* get_row_data(int y) const override {
    return get_row(y);
  }

  template<typename DstPixel>
  PixelData<DstPixel> convert_to() const
  {
    if constexpr (std::is_same<DstPixel, Pixel>::value) {
      return *this;
    } else {
      std::vector<DstPixel> dstpixels;
      dstpixels.reserve(m_pixels.size());
      std::transform(m_pixels.begin(), m_pixels.end(), std::back_inserter(dstpixels),
                     convert<Pixel, DstPixel>);

      PixelData<DstPixel> result(m_size, std::move(dstpixels));
      return result;
    }
  }

  std::unique_ptr<IPixelData> copy() const override {
    return std::make_unique<PixelData<Pixel>>(*this);
  }

protected:
  bool is_equal(IPixelData const& rhs) const override {
    PixelData<Pixel> const* rhs_ptr = dynamic_cast<PixelData<Pixel> const*>(&rhs);
    if (rhs_ptr == nullptr) {
      return false;
    } else {
      return (m_size == rhs_ptr->m_size &&
              m_row_length == rhs_ptr->m_row_length &&
              m_pixels == rhs_ptr->m_pixels);
    }
  }

private:
  geom::isize m_size;
  int m_row_length;
  std::vector<Pixel> m_pixels;
};

} // namespace surf

#endif

/* EOF */
