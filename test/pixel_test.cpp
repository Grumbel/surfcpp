#include <gtest/gtest.h>

#include <surf/pixel.hpp>

TEST(PixelTest, validate_sizeof)
{
  EXPECT_EQ(1, sizeof(surf::GreyscalePixel));
  EXPECT_EQ(3, sizeof(surf::RGBPixel));
  EXPECT_EQ(4, sizeof(surf::RGBAPixel));
}

TEST(PixelTest, equality)
{
  EXPECT_EQ((surf::RGBPixel{1, 2, 3}), (surf::RGBPixel{1, 2, 3}));
  EXPECT_NE((surf::RGBPixel{1, 2, 3}), (surf::RGBPixel{3, 2, 1}));

  EXPECT_EQ((surf::RGBAPixel{1, 2, 3, 4}), (surf::RGBAPixel{1, 2, 3, 4}));
  EXPECT_NE((surf::RGBAPixel{1, 2, 3, 4}), (surf::RGBAPixel{4, 3, 2, 1}));

  EXPECT_EQ((surf::GreyscalePixel{1}), (surf::GreyscalePixel{1}));
  EXPECT_NE((surf::GreyscalePixel{1}), (surf::GreyscalePixel{4}));
}

TEST(PixelTest, max)
{
  EXPECT_EQ(surf::RGB8Pixel::max(), 255);
  EXPECT_EQ(surf::RGB16Pixel::max(), 65535);
  EXPECT_EQ(surf::RGB32Pixel::max(), 4294967295);

  EXPECT_EQ(surf::RGB32fPixel::max(), 1.0f);
  EXPECT_EQ(surf::RGB64fPixel::max(), 1.0);
}

TEST(PixelTest, alpha)
{
  EXPECT_EQ(surf::alpha(surf::RGB8Pixel{}), 255);
  EXPECT_EQ(surf::alpha(surf::RGB16Pixel{}), 65535);
  EXPECT_EQ(surf::alpha(surf::RGB32Pixel{}), 4294967295);
}

TEST(PixelTest, alpha_f)
{
  EXPECT_EQ(surf::alpha_f(surf::RGB8Pixel{}), 1.0f);
  EXPECT_EQ(surf::alpha_f(surf::RGB16Pixel{}), 1.0f);
  EXPECT_EQ(surf::alpha_f(surf::RGB32Pixel{}), 1.0f);
}

TEST(PixelTest, f2value)
{
  EXPECT_EQ(surf::f2value<surf::RGB8Pixel>(1.0f), 255);
  EXPECT_EQ(surf::f2value<surf::RGB16Pixel>(1.0f), 65535);
  EXPECT_EQ(surf::f2value<surf::RGB32Pixel>(1.0f), 4294967295);

  EXPECT_EQ(surf::f2value<surf::RGB8Pixel>(0.0f), 0);
  EXPECT_EQ(surf::f2value<surf::RGB16Pixel>(0.0f), 0);
  EXPECT_EQ(surf::f2value<surf::RGB32Pixel>(0.0f), 0);

  EXPECT_EQ(surf::f2value<surf::RGB32fPixel>(1.0f), 1.0f);
  EXPECT_EQ(surf::f2value<surf::RGB64fPixel>(1.0f), 1.0);

  EXPECT_EQ(surf::f2value<surf::RGB32fPixel>(0.0f), 0.0f);
  EXPECT_EQ(surf::f2value<surf::RGB64fPixel>(0.0f), 0.0);
}

/* EOF */
