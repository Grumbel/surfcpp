#include <iostream>
#include <gtest/gtest.h>

#include <geom/rect.hpp>

#include <surf/color.hpp>
#include <surf/pixel_data.hpp>
#include <surf/sdl.hpp>
#include <surf/transform.hpp>

using namespace surf;

TEST(PixelDataTest, creation)
{
  PixelData pixeldata;
  EXPECT_TRUE(pixeldata.empty());

  pixeldata = PixelData::create(PixelFormat::RGB, geom::isize(64, 32));
  EXPECT_FALSE(pixeldata.empty());
  EXPECT_EQ(pixeldata.get_size(), geom::isize(64, 32));
}

TEST(PixelDataTest, equality)
{
  PixelData black = PixelData::create(PixelFormat::RGB, geom::isize(64, 32), Color::black.to_rgba());
  PixelData white = PixelData::create(PixelFormat::RGB, geom::isize(64, 32), Color::white.to_rgba());

  PixelData pixeldata = PixelData::create(PixelFormat::RGB, geom::isize(64, 32), Color::white.to_rgba());
  EXPECT_FALSE(pixeldata.empty());

  EXPECT_EQ(pixeldata.get_size(), geom::isize(64, 32));
  EXPECT_EQ(pixeldata, pixeldata);
  EXPECT_EQ(pixeldata, white);
  EXPECT_NE(pixeldata, black);
}

TEST(PixelDataTest, empty)
{
  PixelData pixeldata;
  EXPECT_TRUE(pixeldata.empty());

  pixeldata = PixelData::create(PixelFormat::RGB, geom::isize(64, 32));
  EXPECT_FALSE(pixeldata.empty());

  pixeldata = PixelData::create(PixelFormat::RGB, geom::isize(0, 0));
  EXPECT_TRUE(pixeldata.empty());
}

/* EOF */
