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

#ifndef HEADER_SURF_CONVERT_HPP
#define HEADER_SURF_CONVERT_HPP

#include "color.hpp"
#include "pixel.hpp"

namespace surf {

template<typename SrcPixel, typename DstPixel>
DstPixel convert(SrcPixel src) {
  if constexpr (std::is_same<SrcPixel, SrcPixel>::value) {
    return src;
  } else {
    static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
                "convert<>() not implemented for the given types");
    return {};
  }
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
RGBPixel convert<Color, RGBPixel>(Color src) {
  return RGBPixel{src.r8(), src.g8(), src.b8()};
}

template<> inline
RGBAPixel convert<Color, RGBAPixel>(Color src) {
  return RGBAPixel{src.r8(), src.g8(), src.b8(), src.a8()};
}

template<> inline
Color convert<RGBPixel, Color>(RGBPixel src) {
  return Color::from_rgb888(src.r, src.g, src.b);
}

template<> inline
Color convert<RGBAPixel, Color>(RGBAPixel src) {
  return Color::from_rgba8888(src.r, src.g, src.b, src.a);
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

} // namespace surf

#endif

/* EOF */
