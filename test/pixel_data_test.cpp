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
  PixelData<RGBPixel> pixeldata;
  EXPECT_TRUE(pixeldata.empty());

  pixeldata = PixelData<RGBPixel>(geom::isize(64, 32));
  EXPECT_FALSE(pixeldata.empty());
  EXPECT_EQ(pixeldata.get_size(), geom::isize(64, 32));
}

TEST(PixelDataTest, equality)
{
  PixelData<RGBPixel> black(geom::isize(64, 32), RGBPixel{0, 0, 0});
  PixelData<RGBPixel> white(geom::isize(64, 32), RGBPixel{255, 128, 64});

  PixelData<RGBPixel> pixeldata(geom::isize(64, 32), RGBPixel{255, 128, 64});
  EXPECT_FALSE(pixeldata.empty());

  EXPECT_EQ(pixeldata.get_size(), geom::isize(64, 32));
  EXPECT_EQ(pixeldata, pixeldata);
  EXPECT_EQ(pixeldata, white);
  EXPECT_NE(pixeldata, black);

  PixelData<RGBAPixel> pixeldata_rgba(geom::isize(64, 32), RGBAPixel{255, 128, 64, 255});
  EXPECT_NE(pixeldata, pixeldata_rgba);
}

TEST(PixelDataTest, blit_to)
{
  PixelData<RGBPixel> white(geom::isize(4, 3), RGBPixel{255, 255, 255});

  PixelData<RGBPixel> pixeldata(geom::isize(8, 6), RGBPixel{255, 0, 0});
  PixelData<RGBPixel> pixeldata_expected(geom::isize(8, 6), RGBPixel{255, 255, 255});

  white.blit_to(pixeldata, geom::ipoint(0, 0));
  white.blit_to(pixeldata, geom::ipoint(4, 0));
  white.blit_to(pixeldata, geom::ipoint(0, 3));
  white.blit_to(pixeldata, geom::ipoint(4, 3));

  EXPECT_EQ(pixeldata, pixeldata_expected);
}

TEST(PixelDataTest, blit_to_convert)
{
  PixelData<RGBAPixel> white(geom::isize(4, 3), RGBAPixel{255, 255, 255, 255});

  PixelData<RGBPixel> pixeldata(geom::isize(8, 6), RGBPixel{255, 0, 0});
  PixelData<RGBPixel> pixeldata_expected(geom::isize(8, 6), RGBPixel{255, 255, 255});

  white.blit_to(pixeldata, geom::ipoint(0, 0));
  white.blit_to(pixeldata, geom::ipoint(4, 0));
  white.blit_to(pixeldata, geom::ipoint(0, 3));
  white.blit_to(pixeldata, geom::ipoint(4, 3));

  EXPECT_EQ(pixeldata, pixeldata_expected);
}

TEST(PixelDataTest, empty)
{
  PixelData<RGBPixel> pixeldata;
  EXPECT_TRUE(pixeldata.empty());

  pixeldata = PixelData<RGBPixel>(geom::isize(64, 32));
  EXPECT_FALSE(pixeldata.empty());

  pixeldata = PixelData<RGBPixel>(geom::isize(0, 0));
  EXPECT_TRUE(pixeldata.empty());
}

/* EOF */
