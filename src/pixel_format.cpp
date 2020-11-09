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

#include "pixel_format.hpp"

#include <stdexcept>

#include <fmt/format.h>

namespace surf {

std::string to_string(PixelFormat format)
{
  switch (format)
  {
    case PixelFormat::NONE:
      return "NONE";

    case PixelFormat::RGB8:
      return "RGB8";

    case PixelFormat::RGBA8:
      return "RGBA8";

    case PixelFormat::RGB16:
      return "RGB16";

    case PixelFormat::RGBA16:
      return "RGBA16";

    case PixelFormat::RGB32:
      return "RGB32";

    case PixelFormat::RGBA32:
      return "RGBA32";

    case PixelFormat::RGB32f:
      return "RGB32f";

    case PixelFormat::RGBA32f:
      return "RGBA32f";

    case PixelFormat::RGB64f:
      return "RGB64f";

    case PixelFormat::RGBA64f:
      return "RGBA64f";

    default:
      throw std::invalid_argument("unknown PixelFormat");
  }
}

PixelFormat pixelformat_from_string(std::string_view text)
{
  if (text == "rgb8") {
    return PixelFormat::RGB8;
  } else if (text == "rgba8") {
    return PixelFormat::RGBA8;
  } else if (text == "rgb16") {
    return PixelFormat::RGB16;
  } else if (text == "rgba16") {
    return PixelFormat::RGBA16;
  } else if (text == "rgb32") {
    return PixelFormat::RGB32;
  } else if (text == "rgba32") {
    return PixelFormat::RGBA32;
  } else if (text == "rgb32f") {
    return PixelFormat::RGB32f;
  } else if (text == "rgba32f") {
    return PixelFormat::RGBA32f;
  } else if (text == "rgb64f") {
    return PixelFormat::RGB64f;
  } else if (text == "rgba64f") {
    return PixelFormat::RGBA64f;
  } else {
    throw std::invalid_argument(fmt::format("unknown PixelFormat: '{}'", text));
  }
}

} // namespace surf

/* EOF */
