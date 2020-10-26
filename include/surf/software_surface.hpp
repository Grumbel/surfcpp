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

#ifndef HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_HPP
#define HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_HPP

#include <stdint.h>
#include <memory>
#include <geom/fwd.hpp>

#include "pixel_data.hpp"

namespace surf {

class RGB;
class RGBA;

/** A non-mutable high-level SoftwareSurface with value semantics */
class SoftwareSurface
{
public:
  static SoftwareSurface from_file(std::filesystem::path const& filename);
  static SoftwareSurface from_file(std::filesystem::path const& filename, std::string_view loader);
  static SoftwareSurface create(PixelFormat format, geom::isize const& size, Color const& color = {});

public:
  SoftwareSurface();

  template<typename Pixel>
  explicit SoftwareSurface(PixelData<Pixel> data) :
    m_pixel_data(std::make_unique<PixelData<Pixel>>(std::move(data)))
  {}

  geom::isize get_size() const;
  int get_width() const;
  int get_height() const;
  int get_pitch() const;
  PixelFormat get_format() const;

  void blit_to(SoftwareSurface const& dst, geom::ipoint const& pos);
  SoftwareSurface convert_to(PixelFormat format) const;

  //SoftwareSurface to_rgb() const;
  //RGB get_average_color() const;

  Color get_pixel(geom::ipoint const& position) const;

  void* get_data();
  void* get_row_data(int y);

  void const* get_data() const;
  void const* get_row_data(int y) const;

  IPixelData const& get_pixel_data() const { return *m_pixel_data; }

  template<typename Pixel>
  PixelData<Pixel> const* as_pixeldata_ptr() const {
    return dynamic_cast<PixelData<Pixel> const*>(m_pixel_data.get());
  }

  template<typename Pixel>
  PixelData<Pixel> const& as_pixeldata() const {
    return dynamic_cast<PixelData<Pixel> const&>(*m_pixel_data);
  }

  template<typename Pixel>
  PixelData<Pixel>& as_pixeldata() {
    return dynamic_cast<PixelData<Pixel>&>(*m_pixel_data);
  }

  bool operator==(SoftwareSurface const& rhs) const {
    return *m_pixel_data == *rhs.m_pixel_data;
  }

private:
  std::unique_ptr<IPixelData> m_pixel_data;
};

inline
std::ostream& operator<<(std::ostream& os, SoftwareSurface const& surface)
{
  surface.get_pixel_data().print(os);
  return os;
}

#define SOFTWARE_SURFACE_UNWRAP(format, fail_expr, expr)        \
  switch (format)                                               \
  {                                                             \
    default:                                                    \
    case PixelFormat::NONE: {                                   \
      fail_expr;                                                \
      break;                                                    \
    }                                                           \
                                                                \
    case PixelFormat::RGB: {                                    \
      auto&& pixeldata = surface.as_pixeldata<RGBPixel>();      \
      expr;                                                     \
      break;                                                    \
    }                                                           \
                                                                \
    case PixelFormat::RGBA {:                                   \
      auto&& pixeldata = surface.as_pixeldata<RGBAPixel>();     \
      expr;                                                     \
      break;                                                    \
    }                                                           \
  }

#define SOFTWARE_SURFACE_LIFT_N(name, function)         \
  template<typename ...Args>                            \
  SoftwareSurface name(SoftwareSurface const& surface,  \
                       Args&&... args)                  \
  {                                                     \
    switch (surface.get_format()) {                     \
      case PixelFormat::NONE:                           \
        return SoftwareSurface();                       \
                                                        \
      case PixelFormat::RGB:                            \
        return SoftwareSurface(                         \
          function(surface.as_pixeldata<RGBPixel>(),    \
                   std::forward<Args>(args)...));       \
                                                        \
      case PixelFormat::RGBA:                           \
        return SoftwareSurface(                         \
          function(surface.as_pixeldata<RGBAPixel>(),   \
                   std::forward<Args>(args)...));       \
      default:                                          \
        log_unreachable();                              \
        return {};                                      \
    }                                                   \
  }

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

#define SOFTWARE_SURFACE_LIFT(function)         \
  SOFTWARE_SURFACE_LIFT_N(function, function)

} // namespace surf

#endif

/* EOF */
