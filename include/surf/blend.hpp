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
#include "convert.hpp"
#include "pixel.hpp"

namespace surf {

template<typename SrcPixel, typename DstPixel>
DstPixel pixel_blend(SrcPixel src, DstPixel dst)
{
  if constexpr (!SrcPixel::has_alpha()) {
    return convert<SrcPixel, DstPixel>(src);
  } else if constexpr (SrcPixel::has_alpha() && !DstPixel::has_alpha()) {
    return DstPixel{
      static_cast<typename DstPixel::value_type>((src.r * src.a + dst.r * (SrcPixel::max() - src.a)) / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>((src.g * src.a + dst.g * (SrcPixel::max() - src.a)) / SrcPixel::max()),
      static_cast<typename DstPixel::value_type>((src.b * src.a + dst.b * (SrcPixel::max() - src.a)) / SrcPixel::max())
    };
  } else if constexpr (SrcPixel::has_alpha() && DstPixel::has_alpha()) {
    typename DstPixel::value_type const out_a = static_cast<typename DstPixel::value_type>(src.a + dst.a * (SrcPixel::max() - src.a) / SrcPixel::max());
    if (out_a == 0) {
      return DstPixel{0, 0, 0, 0};
    } else {
      return DstPixel{
        static_cast<typename DstPixel::value_type>((src.r * src.a * DstPixel::max() / SrcPixel::max() + dst.r * dst.a * (SrcPixel::max() - src.a) / SrcPixel::max()) / out_a),
        static_cast<typename DstPixel::value_type>((src.g * src.a * DstPixel::max() / SrcPixel::max() + dst.g * dst.a * (SrcPixel::max() - src.a) / SrcPixel::max()) / out_a),
        static_cast<typename DstPixel::value_type>((src.b * src.a * DstPixel::max() / SrcPixel::max() + dst.b * dst.a * (SrcPixel::max() - src.a) / SrcPixel::max()) / out_a),
        out_a
      };
    }
  } else {
    static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
                  "blend<>() not implemented for the given types");
    return DstPixel{};
  }
}

} // namespace surf

#endif

/* EOF */
