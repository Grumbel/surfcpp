#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

#include <geom/rect.hpp>

#include <surf/pixel_data.hpp>
#include <surf/sdl.hpp>
#include <surf/transform.hpp>

using namespace surf;

TEST(SDLTest, create_sdl_surface_view)
{
  PixelData const red = PixelData::create(PixelFormat::RGB, geom::isize{8, 16}, RGBA(255, 0, 0, 255));
  PixelData const white = PixelData::create(PixelFormat::RGB, geom::isize{8, 16}, RGBA(255, 255, 255, 255));
  PixelData const pixeldata = white;

  SDLSurfacePtr sdl_surf = create_sdl_surface_view(pixeldata);

  EXPECT_NE(sdl_surf.get(), nullptr);

  EXPECT_EQ(white, pixeldata);

  SDL_FillRect(sdl_surf.get(), nullptr, SDL_MapRGB(sdl_surf->format, 255, 0, 0));

  EXPECT_EQ(red, pixeldata);
}

TEST(SDLTest, create_sdl_surface)
{
  PixelData pixeldata(PixelFormat::RGB, {256, 128});
  SDLSurfacePtr sdl_surf = create_sdl_surface_view(pixeldata);
}

TEST(SDLTest, from_sdl_surface)
{
  PixelData const red = PixelData::create(PixelFormat::RGB, geom::isize{8, 16}, RGBA(255, 0, 0, 255));
  SDLSurfacePtr sdl_red = create_sdl_surface(red);
  PixelData const red_out = from_sdl_surface(*sdl_red);
  EXPECT_EQ(red.get_size(), red_out.get_size());
  EXPECT_EQ(red, red_out);
}

/* EOF */
