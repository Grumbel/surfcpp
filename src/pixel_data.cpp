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

#include "pixel_data.hpp"

#include <ostream>

#include <logmich/log.hpp>
#include <geom/rect.hpp>
#include <string.h>

#include "rgb.hpp"
#include "rgba.hpp"
#include "pixel_data_factory.hpp"

namespace surf {

namespace {

PixelDataFactory g_pixeldata_fatory;

} // namespace

/*
PixelData
PixelData::from_file(std::filesystem::path const& filename)
{
  return g_pixeldata_fatory.from_file(filename);
}

PixelData
PixelData::from_file(std::filesystem::path const& filename, std::string_view loader)
{
  return g_pixeldata_fatory.from_file(filename, loader);
}

PixelData
PixelData::create(PixelFormat format, const geom::isize& size, RGBA const& rgba)
{
  PixelData pixel_data(format, size);
  pixel_data.fill(rgba);
  return pixel_data;
}
*/

} // namespace surf

/* EOF */
