#include <gtest/gtest.h>

#include <surf/color.hpp>
#include <surf/palette.hpp>

using namespace surf;

TEST(ColorTest, construction)
{
  EXPECT_EQ(Color(1.0f, 1.0f, 1.0f), palette::white);
  EXPECT_EQ(Color(1.0f, 0.0f, 0.0f), palette::red);
  EXPECT_EQ(Color(0.0f, 1.0f, 0.0f), palette::lime); // not green!
  EXPECT_EQ(Color(0.0f, 0.0f, 1.0f), palette::blue);
  EXPECT_EQ(Color(0.0f, 0.0f, 0.0f), palette::black);
}

TEST(ColorTest, from_string)
{
  EXPECT_EQ(Color::from_string("white"), palette::white);
  EXPECT_EQ(Color::from_string("red"), palette::red);
  EXPECT_EQ(Color::from_string("green"), palette::green);
  EXPECT_EQ(Color::from_string("blue"), palette::blue);
  EXPECT_EQ(Color::from_string("black"), palette::black);
}

TEST(ColorTest, u8)
{
  EXPECT_EQ(palette::chocolate.r8(), 0xd2);
  EXPECT_EQ(palette::chocolate.g8(), 0x69);
  EXPECT_EQ(palette::chocolate.b8(), 0x1e);
  EXPECT_EQ(palette::chocolate.a8(), 255);
}

/* EOF */
