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

#ifndef HEADER_SURF_UNWRAP_HPP
#define HEADER_SURF_UNWRAP_HPP

#define PIXELFORMAT_TO_TYPE(format, type, fail_expr, expr)      \
  do {                                                          \
    switch (format)                                             \
    {                                                           \
      default:                                                  \
      case PixelFormat::NONE: {                                 \
        fail_expr;                                              \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGB: {                                  \
        using type = RGBPixel;                                  \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA: {                                 \
        using type = RGBAPixel;                                 \
        expr;                                                   \
        break;                                                  \
      }                                                         \
    }                                                           \
  } while (false)

#define SOFTWARE_SURFACE_UNWRAP(surface, pixeldata, fail_expr, expr)    \
  do {                                                                  \
    switch (surface.get_format())                                       \
    {                                                                   \
      default:                                                          \
      case PixelFormat::NONE: {                                         \
        fail_expr;                                                      \
        break;                                                          \
      }                                                                 \
                                                                        \
      case PixelFormat::RGB: {                                          \
        auto&& pixeldata = surface.as_pixeldata<RGBPixel>();            \
        expr;                                                           \
        break;                                                          \
      }                                                                 \
                                                                        \
      case PixelFormat::RGBA: {                                         \
        auto&& pixeldata = surface.as_pixeldata<RGBAPixel>();           \
        expr;                                                           \
        break;                                                          \
      }                                                                 \
    }                                                                   \
  } while (false)

#define SOFTWARE_SURFACE_LIFT_N(name, function)         \
  template<typename ...Args>                            \
  SoftwareSurface name(SoftwareSurface const& surface,  \
                       Args&&... args)                  \
  {                                                     \
    SOFTWARE_SURFACE_UNWRAP(                            \
      surface,                                          \
      pixeldata,                                        \
      return {},                                        \
      return SoftwareSurface(                           \
        function(pixeldata,                             \
                 std::forward<Args>(args)...)));        \
  }

#define SOFTWARE_SURFACE_LIFT2(function)                        \
  template<typename ...Args>                                    \
  SoftwareSurface function(SoftwareSurface const& lhs,          \
                           SoftwareSurface const& rhs,          \
                           Args&&... args)                      \
  {                                                             \
    SOFTWARE_SURFACE_UNWRAP(                                    \
      lhs,                                                      \
      lhs_pixeldata,                                            \
      return {},                                                \
      SOFTWARE_SURFACE_UNWRAP(                                  \
        rhs,                                                    \
        rhs_pixelbuffer,                                        \
        log_unreachable(),                                      \
        function(lhs, rhs, std::forward<Args>(args)...))        \
      );                                                        \
  }

#if 0
#define SOFTWARE_SURFACE_LIFT2_N(name, function)                \
                                                                \
  template<typename Pixel, typename ...Args>                    \
  SoftwareSurface name##__lift_rhs(PixelData<Pixel> const& lhs, \
                                   SoftwareSurface const& rhs,  \
                                   Args&&... args)              \
  {                                                             \
    switch (rhs.get_format()) {                                 \
      case PixelFormat::NONE:                                   \
        return SoftwareSurface();                               \
                                                                \
      case PixelFormat::RGB:                                    \
        return SoftwareSurface(                                 \
          function(lhs,                                         \
                   rhs.as_pixeldata<RGBPixel>(),                \
                   std::forward<Args>(args)...));               \
                                                                \
      case PixelFormat::RGBA:                                   \
        return SoftwareSurface(                                 \
          function(lhs,                                         \
                   rhs.as_pixeldata<RGBAPixel>(),               \
                   std::forward<Args>(args)...));               \
      default:                                                  \
        log_unreachable();                                      \
        return {};                                              \
    }                                                           \
  }                                                             \
                                                                \
  template<typename ...Args>                                    \
  SoftwareSurface name(SoftwareSurface const& lhs,              \
                       SoftwareSurface const& rhs,              \
                       Args&&... args)                          \
  {                                                             \
    switch (lhs.get_format()) {                                 \
      case PixelFormat::NONE:                                   \
        return SoftwareSurface();                               \
                                                                \
      case PixelFormat::RGB:                                    \
        return SoftwareSurface(                                 \
          name##__lift_rhs(lhs.as_pixeldata<RGBPixel>(),        \
                           rhs,                                 \
                           std::forward<Args>(args)...));       \
                                                                \
      case PixelFormat::RGBA:                                   \
        return SoftwareSurface(                                 \
          name##__lift_rhs(lhs.as_pixeldata<RGBAPixel>(),       \
                           rhs,                                 \
                           std::forward<Args>(args)...));       \
      default:                                                  \
        log_unreachable();                                      \
        return {};                                              \
    }                                                           \
  }
#endif

#define SOFTWARE_SURFACE_LIFT(function)         \
  SOFTWARE_SURFACE_LIFT_N(function, function)

#endif

/* EOF */
