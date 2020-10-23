/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdexcept>

#include "pixel_data.hpp"
#include "plugins/jpeg.hpp"
#include "plugins/png.hpp"
#include "save.hpp"

namespace surf {

void save(PixelData const& pixeldata, std::filesystem::path const& path, std::string_view format)
{
  if (format == "png") {
    surf::png::save(pixeldata, path);
  } else if (format == "jpeg") {
    surf::jpeg::save(pixeldata, path, 70);
  } else {
    throw std::runtime_error("unsupported format");
  }
}

} // namespace surf

/* EOF */
