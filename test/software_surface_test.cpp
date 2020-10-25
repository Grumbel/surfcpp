#include <iostream>
#include <gtest/gtest.h>

#include <geom/rect.hpp>
#include <geom/io.hpp>

#include <surf/software_surface.hpp>
#include <surf/transform.hpp>
#include <surf/rgb.hpp>

using namespace surf;

TEST(SoftwareSurfaceTest, default_is_valid)
{
  SoftwareSurface pixel_data;

  EXPECT_EQ(geom::isize(0, 0), pixel_data.get_size());
  EXPECT_EQ(geom::isize(0, 0), halve(pixel_data).get_size());
  EXPECT_EQ(geom::isize(0, 0), scale(pixel_data, geom::isize(32, 16)).get_size());
  EXPECT_EQ(geom::isize(0, 0), crop(pixel_data, geom::irect(0, 0, 16, 16)).get_size());

  transform(pixel_data, Transform::ROTATE_0);
  rotate90(pixel_data);
  rotate180(pixel_data);
  rotate270(pixel_data);
  flip_horizontal(pixel_data);
  flip_vertical(pixel_data);
  SUCCEED();
#if 0
  to_rgb(pixel_data);
  average_color(pixel_data);
#endif
}

TEST(SoftwareSurfaceTest, assignment)
{
  SoftwareSurface lhs(PixelData<RGBPixel>(geom::isize(32, 16)));
  SoftwareSurface rhs(PixelData<RGBPixel>(geom::isize(64, 32)));

  lhs = std::exchange(rhs, std::move(lhs));

  EXPECT_EQ(geom::isize(64, 32), lhs.get_size());
  EXPECT_EQ(geom::isize(32, 16), rhs.get_size());
}

TEST(SoftwareSurfaceTest, move)
{
  SoftwareSurface lhs(PixelData<RGBPixel>(geom::isize(32, 16)));
  SoftwareSurface rhs(PixelData<RGBPixel>(geom::isize(64, 32)));
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
