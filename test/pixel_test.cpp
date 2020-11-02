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

/* EOF */
