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

#ifndef HEADER_GALAPIX_MATH_RGBAF_HPP
#define HEADER_GALAPIX_MATH_RGBAF_HPP

#include "rgba.hpp"
#include "rgb.hpp"

namespace surf {

class RGBAf
{
public:
  RGBAf() :
    r(0.0f), g(0.0f), b(0.0f), a(0.0f)
  {}

  RGBAf(RGB const& rgb) :
    r(static_cast<float>(rgb.r)/255.0f),
    g(static_cast<float>(rgb.g)/255.0f),
    b(static_cast<float>(rgb.b)/255.0f),
    a(1.0f)
  {}

  RGBAf(RGBA const& rgba) :
    r(static_cast<float>(rgba.r)/255.0f),
    g(static_cast<float>(rgba.g)/255.0f),
    b(static_cast<float>(rgba.b)/255.0f),
    a(static_cast<float>(rgba.a)/255.0f)
  {}

  RGBAf(float r_,
        float g_,
        float b_,
        float a_) :
    r(r_), g(g_), b(b_), a(a_)
  {}

  inline uint8_t r8() const { return static_cast<uint8_t>(255.0f * r); }
  inline uint8_t g8() const { return static_cast<uint8_t>(255.0f * g); }
  inline uint8_t b8() const { return static_cast<uint8_t>(255.0f * b); }
  inline uint8_t a8() const { return static_cast<uint8_t>(255.0f * a); }

public:
  float r;
  float g;
  float b;
  float a;
};

inline
RGBAf clamp(RGBAf const& color)
{
  return RGBAf(std::clamp(color.r, 0.0f, 1.0f),
               std::clamp(color.g, 0.0f, 1.0f),
               std::clamp(color.b, 0.0f, 1.0f),
               std::clamp(color.a, 0.0f, 1.0f));
}

} // namespace surf

#endif

/* EOF */
