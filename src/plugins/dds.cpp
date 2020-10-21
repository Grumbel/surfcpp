// Galapix - an image viewer for large image collections
// Copyright (C) 2008-2019 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "plugins/dds.hpp"

#include <errno.h>
#include <fstream>
#include <string.h>
#include <sstream>

#include "pixel_format.hpp"
#include "plugins/dds_surface.hpp"

namespace surf {
namespace dds {

bool get_size(std::filesystem::path const& filename, geom::isize& size)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
  {
    // raise_runtime_error(strerror(errno));
    return false;
  }
  else
  {
    // FIXME: not very fast as we decode the complete surface
    DDSSurface surface(in);
    size = geom::isize(surface.get_width(), surface.get_height());
    return true;
  }
}

SoftwareSurface load_from_file(std::filesystem::path const& filename)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
  {
    // raise_runtime_error(strerror(errno));
    return {};
  }
  else
  {
    DDSSurface dds(in);

    PixelData dst(PixelFormat::RGBA, geom::isize(dds.get_width(), dds.get_height()));

    if (static_cast<int>(dds.get_length()) != dst.get_width() * dst.get_height() * 4)
    {
      std::ostringstream out;
      out << "DDS::load_from_file(): length missmatch " << dds.get_length()
          << " - " << dst.get_width() << "x" << dst.get_height();
      throw std::runtime_error(out.str());
    }

    memcpy(dst.get_data(), dds.get_data(), dds.get_length());

    return SoftwareSurface(std::move(dst));
  }
}

} // namespace dds
} // namespace surf

/* EOF */
