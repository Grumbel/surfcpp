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

#include <logmich/log.hpp>

#include "software_surface.hpp"

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
      case PixelFormat::RGB8: {                                 \
        using type = RGB8Pixel; /* NOLINT */                    \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA8: {                                \
        using type = RGBA8Pixel; /* NOLINT */                   \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGB16: {                                \
        using type = RGB16Pixel; /* NOLINT */                   \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA16: {                               \
        using type = RGBA16Pixel; /* NOLINT */                  \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGB32: {                                \
        using type = RGB32Pixel; /* NOLINT */                   \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA32: {                               \
        using type = RGBA32Pixel; /* NOLINT */                  \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGB32f: {                               \
        using type = RGB32fPixel; /* NOLINT */                  \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA32f: {                              \
        using type = RGBA32fPixel; /* NOLINT */                 \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGB64f: {                               \
        using type = RGB64fPixel; /* NOLINT */                  \
        expr;                                                   \
        break;                                                  \
      }                                                         \
                                                                \
      case PixelFormat::RGBA64f: {                              \
        using type = RGBA64fPixel; /* NOLINT */                 \
        expr;                                                   \
        break;                                                  \
      }                                                         \
    }                                           \
  } while (false)

#define SOFTWARE_SURFACE_UNWRAP(surface, pixelview, fail_expr, expr)    \
  PIXELFORMAT_TO_TYPE(                                                  \
    (surface).get_format(),                                             \
    pixelview##pixeltype,                                               \
    fail_expr,                                                          \
    auto&& pixelview = (surface).as_pixelview<pixelview##pixeltype>(); /* NOLINT */ \
    expr                                                                \
    )

#define SOFTWARE_SURFACE_LIFT_VOID(function)    \
  template<typename ...Args>                    \
  void function(SoftwareSurface& surface,       \
                Args&&... args)                 \
  {                                             \
    SOFTWARE_SURFACE_UNWRAP(                    \
      surface,                                  \
      pixelview,                                \
      log_unreachable(),                        \
      function(pixelview,                       \
               std::forward<Args>(args)...));   \
  }

#define SOFTWARE_SURFACE_LIFT_N(name, function)         \
  template<typename ...Args>                            \
  SoftwareSurface name(SoftwareSurface const& surface,  \
                       Args&&... args)                  \
  {                                                     \
    SOFTWARE_SURFACE_UNWRAP(                            \
      surface,                                          \
      pixelview,                                        \
      return {},                                        \
      return SoftwareSurface(                           \
        function(pixelview,                             \
                 std::forward<Args>(args)...)));        \
  }

#define SOFTWARE_SURFACE_LIFT(function)         \
  SOFTWARE_SURFACE_LIFT_N(function, function)

#define SOFTWARE_SURFACE_LIFT2(function)                        \
  template<typename ...Args>                                    \
  SoftwareSurface function(SoftwareSurface const& lhs,          \
                           SoftwareSurface const& rhs,          \
                           Args&&... args)                      \
  {                                                             \
    SOFTWARE_SURFACE_UNWRAP(                                    \
      lhs,                                                      \
      lhs_pixelview,                                            \
      return {},                                                \
      SOFTWARE_SURFACE_UNWRAP(                                  \
        rhs,                                                    \
        rhs_pixelbuffer,                                        \
        log_unreachable(),                                      \
        function(lhs, rhs, std::forward<Args>(args)...))        \
      );                                                        \
  }

#endif

/* EOF */
