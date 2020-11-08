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

    default:
      throw std::invalid_argument("unknown PixelFormat");
  }
}

} // namespace surf

/* EOF */
