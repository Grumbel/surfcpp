#include <iostream>
#include <gtest/gtest.h>

#include <geom/rect.hpp>

#include "surface/software_surface.hpp"
#include "surface/rgb.hpp"

using namespace surf;

TEST(SoftwareSurfaceTest, default_is_valid)
{
  SoftwareSurface surface;

  EXPECT_EQ(geom::isize(0, 0), surface.get_size());

  EXPECT_EQ(geom::isize(0, 0), surface.halve().get_size());
  EXPECT_EQ(geom::isize(32, 16), surface.scale(geom::isize(32, 16)).get_size());
  EXPECT_EQ(geom::isize(0, 0), surface.crop(geom::irect(0, 0, 16, 16)).get_size());

  surface.transform(SoftwareSurface::kRot0);
  surface.rotate90();
  surface.rotate180();
  surface.rotate270();
  surface.vflip();
  surface.hflip();
  surface.to_rgb();
  surface.get_average_color();

  surface.get_pixel_data();
}

TEST(SoftwareSurfaceTest, assignment)
{
  SoftwareSurface lhs(PixelData(PixelFormat::RGB, geom::isize(32, 16)));
  SoftwareSurface rhs(PixelData(PixelFormat::RGB, geom::isize(64, 32)));
  SoftwareSurface tmp;

  tmp = lhs;
  lhs = rhs;
  rhs = tmp;

  EXPECT_EQ(geom::isize(64, 32), lhs.get_size());
  EXPECT_EQ(geom::isize(32, 16), rhs.get_size());
  EXPECT_EQ(geom::isize(32, 16), tmp.get_size());
}

TEST(SoftwareSurfaceTest, move)
{
  SoftwareSurface lhs(PixelData(PixelFormat::RGB, geom::isize(32, 16)));
  SoftwareSurface rhs(PixelData(PixelFormat::RGB, geom::isize(64, 32)));
  SoftwareSurface tmp;

  tmp = std::move(lhs);
  lhs = std::move(rhs);
  rhs = std::move(tmp);

  EXPECT_EQ(geom::isize(64, 32), lhs.get_size());
  EXPECT_EQ(geom::isize(32, 16), rhs.get_size());

  // this will crash as SoftwareSurface::m_pixel_data goes nullptr
  // EXPECT_EQ(geom::isize(32, 16), tmp.get_size());
}

/* EOF */
