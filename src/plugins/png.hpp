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

#ifndef HEADER_GALAPIX_PLUGINS_PNG_HPP
#define HEADER_GALAPIX_PLUGINS_PNG_HPP

#include <filesystem>
#include <span>
#include <string>

#include <geom/size.hpp>

#include "software_surface.hpp"

namespace surf {
namespace png {

bool get_size(void* data, int len, geom::isize& size);
bool get_size(std::filesystem::path const& filename, geom::isize& size);

bool is_png(std::filesystem::path const& filename);

SoftwareSurface load_from_file(std::filesystem::path const& filename);
SoftwareSurface load_from_mem(std::span<uint8_t const> data);

void save(SoftwareSurface const& surface, std::filesystem::path const& filename);
std::vector<uint8_t> save(SoftwareSurface const& surface);

} // namespace png
} // namespace surf

#endif

/* EOF */
