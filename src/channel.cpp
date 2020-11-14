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

#include "channel.hpp"

#include "software_surface.hpp"
#include "unwrap.hpp"

namespace surf {

namespace {

template<typename T>
std::vector<SoftwareSurface>
split_channel_to_software_surface(PixelView<tLPixel<T>> const& src)
{
  return {SoftwareSurface(src)};
}

template<typename T>
std::vector<SoftwareSurface>
split_channel_to_software_surface(PixelView<tLAPixel<T>> const& src)
{
  return {SoftwareSurface(src)};
}

template<typename T>
std::vector<SoftwareSurface>
split_channel_to_software_surface(PixelView<tRGBPixel<T>> const& src)
{
  auto res = split_channel(src);
  return {
    SoftwareSurface(std::move(res[0])),
    SoftwareSurface(std::move(res[1])),
    SoftwareSurface(std::move(res[2]))
  };
}

template<typename T>
std::vector<SoftwareSurface>
split_channel_to_software_surface(PixelView<tRGBAPixel<T>> const& src)
{
  auto res = split_channel(src);
  return {
    SoftwareSurface(std::move(res[0])),
    SoftwareSurface(std::move(res[1])),
    SoftwareSurface(std::move(res[2])),
    SoftwareSurface(std::move(res[3]))
  };
}

} // namespace

std::vector<SoftwareSurface>
split_channel(SoftwareSurface const& src)
{
  PIXELFORMAT_TO_TYPE(
    src.get_format(), srctype,
    return split_channel_to_software_surface(src.as_pixelview<srctype>()););
}

} // namespace surf

/* EOF */
