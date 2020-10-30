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

#ifndef HEADER_SURF_BLEND_HPP
#define HEADER_SURF_BLEND_HPP

#include "color.hpp"
#include "pixel.hpp"

namespace surf {

template<typename SrcPixel, typename DstPixel>
DstPixel blend(SrcPixel src, DstPixel dst) {
  static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
                "blend<>() not implemented for the given types");
  return DstPixel{};
}

template<> inline
RGBPixel blend<RGBPixel, RGBPixel>(RGBPixel src, RGBPixel dst)
{
  return src;
}

template<> inline
RGBAPixel blend<RGBPixel, RGBAPixel>(RGBPixel src, RGBAPixel dst)
{
  return RGBAPixel{src.r, src.g, src.b, 255};
}

template<> inline
RGBPixel blend<RGBAPixel, RGBPixel>(RGBAPixel src, RGBPixel dst)
{
  return RGBPixel{
    static_cast<uint8_t>((src.r * src.a + dst.r * (255 - src.a)) / 255),
    static_cast<uint8_t>((src.g * src.a + dst.g * (255 - src.a)) / 255),
    static_cast<uint8_t>((src.b * src.a + dst.b * (255 - src.a)) / 255)
  };
}

template<> inline
RGBAPixel blend<RGBAPixel, RGBAPixel>(RGBAPixel src, RGBAPixel dst)
{
  uint8_t const out_a = static_cast<uint8_t>(src.a + dst.a * (255 - src.a) / 255);
  if (out_a == 0) {
    return RGBAPixel{0, 0, 0, 0};
  } else {
    return RGBAPixel{
      static_cast<uint8_t>((src.r * src.a + dst.r * dst.a * (255 - src.a) / 255) / out_a),
      static_cast<uint8_t>((src.g * src.a + dst.g * dst.a * (255 - src.a) / 255) / out_a),
      static_cast<uint8_t>((src.b * src.a + dst.b * dst.a * (255 - src.a) / 255) / out_a),
      out_a
    };
  }
}

} // namespace surf

#endif

/* EOF */
