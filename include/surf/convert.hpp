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
  if constexpr (std::is_same<SrcPixel, DstPixel>::value) {
    return src;
  } else if constexpr (std::is_same<DstPixel, Color>::value) {
    if constexpr (SrcPixel::has_alpha()) {
      return Color(static_cast<float>(src.r) / static_cast<float>(SrcPixel::max()),
                   static_cast<float>(src.g) / static_cast<float>(SrcPixel::max()),
                   static_cast<float>(src.b) / static_cast<float>(SrcPixel::max()),
                   static_cast<float>(src.a) / static_cast<float>(SrcPixel::max()));
    } else {
      return Color(static_cast<float>(src.r) / static_cast<float>(SrcPixel::max()),
                   static_cast<float>(src.g) / static_cast<float>(SrcPixel::max()),
                   static_cast<float>(src.b) / static_cast<float>(SrcPixel::max()));
    }
  } else if constexpr (std::is_same<SrcPixel, Color>::value) {
    if constexpr (DstPixel::has_alpha()) {
      return DstPixel{
        static_cast<typename DstPixel::value_type>(src.r * static_cast<float>(DstPixel::max())),
        static_cast<typename DstPixel::value_type>(src.g * static_cast<float>(DstPixel::max())),
        static_cast<typename DstPixel::value_type>(src.b * static_cast<float>(DstPixel::max())),
        static_cast<typename DstPixel::value_type>(src.a * static_cast<float>(DstPixel::max()))
      };
    } else {
      return DstPixel{
        static_cast<typename DstPixel::value_type>(src.r * static_cast<float>(DstPixel::max())),
        static_cast<typename DstPixel::value_type>(src.g * static_cast<float>(DstPixel::max())),
        static_cast<typename DstPixel::value_type>(src.b * static_cast<float>(DstPixel::max()))
      };
    }
  } else if constexpr (!SrcPixel::has_alpha() && !DstPixel::has_alpha()) {
    return DstPixel{
      static_cast<typename DstPixel::value_type>(src.r * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.g * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.b * DstPixel::max() / SrcPixel::max())
    };
  } else if constexpr (SrcPixel::has_alpha() && DstPixel::has_alpha()) {
    return DstPixel{
      static_cast<typename DstPixel::value_type>(src.r * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.g * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.b * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.a * DstPixel::max() / SrcPixel::max())
    };
  } else if constexpr (!SrcPixel::has_alpha() && DstPixel::has_alpha()) {
    return DstPixel{
      static_cast<typename DstPixel::value_type>(src.r * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.g * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.b * DstPixel::max() / SrcPixel::max()),
      DstPixel::max()
    };
  } else if constexpr (SrcPixel::has_alpha() && !DstPixel::has_alpha()) {
    return DstPixel{
      static_cast<typename DstPixel::value_type>(src.r * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.g * DstPixel::max() / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>(src.b * DstPixel::max() / SrcPixel::max())
    };
  } else {
    static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
                  "convert<>() not implemented for the given types");
    return {};
  }
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
