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

#ifndef HEADER_SURF_SDL_HPP
#define HEADER_SURF_SDL_HPP

#include <SDL.h>
#include <assert.h>
#include <string.h>
#include <memory>

namespace surf {

class SDLSurfacePtr
{
public:
  SDLSurfacePtr(SDL_Surface* surf) : m_surf(surf) { assert(m_surf != nullptr); }
  SDLSurfacePtr(SDLSurfacePtr&& other) : m_surf(std::exchange(other.m_surf, nullptr)) {}
  SDLSurfacePtr(const SDLSurfacePtr&) = delete;
  SDLSurfacePtr& operator=(const SDLSurfacePtr&) = delete;
  ~SDLSurfacePtr() { destroy(); }

  SDLSurfacePtr& operator=(SDLSurfacePtr&& other) {
    if (this == &other) { return *this; }
    destroy();
    m_surf = std::exchange(other.m_surf, nullptr);
    return *this;
  }

  void reset(SDL_Surface* surf) { destroy(); m_surf = surf; }
  SDL_Surface& operator*() const { return *m_surf; }
  SDL_Surface* get() const { return m_surf; }
  SDL_Surface* operator->() const { return m_surf; }

private:
  void destroy() {
    if (m_surf == nullptr) { return; }

    SDL_FreeSurface(m_surf);
    m_surf = nullptr;
  }

private:
  SDL_Surface* m_surf;
};

/** Create an SDL_Surface from PixelData without copying it */
inline
SDLSurfacePtr create_sdl_surface_view(PixelData& pixeldata)
{
  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixeldata.get_data(),
                                                  pixeldata.get_width(),
                                                  pixeldata.get_height(),
                                                  pixeldata.get_format().bits_per_pixel(),
                                                  pixeldata.get_pitch(),
                                                  pixeldata.get_format().rmask(),
                                                  pixeldata.get_format().gmask(),
                                                  pixeldata.get_format().bmask(),
                                                  pixeldata.get_format().amask());
  if (surface == nullptr) {
    std::ostringstream oss;
    oss << "SDL_Surface creation failed: " << SDL_GetError();
    throw std::runtime_error(oss.str());
  }

  return SDLSurfacePtr(surface);
}

/** Create an SDL_Surface from PixelData without copying it. */
inline
SDLSurfacePtr create_sdl_surface_view(PixelData const& pixeldata)
{
  return create_sdl_surface_view(const_cast<PixelData&>(pixeldata));
}

/** Create an SDL_Surface and copy PixelData into it. */
inline
SDLSurfacePtr create_sdl_surface(PixelData const& pixeldata)
{
  SDL_Surface* surface = SDL_CreateRGBSurface(0,
                                              pixeldata.get_width(),
                                              pixeldata.get_height(),
                                              pixeldata.get_format().bits_per_pixel(),
                                              pixeldata.get_format().rmask(),
                                              pixeldata.get_format().gmask(),
                                              pixeldata.get_format().bmask(),
                                              pixeldata.get_format().amask());
  if (surface == nullptr) {
    std::ostringstream oss;
    oss << "SDL_Surface creation failed: " << SDL_GetError();
    throw std::runtime_error(oss.str());
  }

  SDL_LockSurface(surface);
  for (int y = 0; y < pixeldata.get_height(); ++y) {
    memcpy(&static_cast<uint8_t*>(surface->pixels)[y * surface->pitch], pixeldata.get_row_data(y),
           pixeldata.get_width() * pixeldata.get_format().bytes_per_pixel());
  }
  SDL_UnlockSurface(surface);

  return surface;
}

inline
PixelData from_sdl_surface(SDL_Surface& surface)
{
  PixelFormat format;
  if (surface.format->BitsPerPixel == 32) {
    format = PixelFormat::RGBA;
  } else if (surface.format->BitsPerPixel == 24) {
    format = PixelFormat::RGB;
  } else {
    throw std::runtime_error("unhandled SDL_Surface format");
  }

  PixelData pixeldata(format, {surface.w, surface.h});
  SDLSurfacePtr dst(create_sdl_surface_view(pixeldata));
  if (SDL_BlitSurface(&surface, nullptr, dst.get(), nullptr) != 0) {
    std::ostringstream oss;
    oss << "from_sdl_surface() failed:: " << SDL_GetError();
    throw std::runtime_error(oss.str());
  }

  return pixeldata;
}

} // namespace surf

#endif

/* EOF */
