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

#ifndef HEADER_SURF_TRANSFORM_HPP
#define HEADER_SURF_TRANSFORM_HPP

namespace surf {

class SoftwareSurface;

enum class Transform
{
  ROTATE_0,
  ROTATE_90,
  ROTATE_180,
  ROTATE_270,
  ROTATE_0_FLIP,
  ROTATE_90_FLIP,
  ROTATE_180_FLIP,
  ROTATE_270_FLIP,

  FLIP_VERTICAL = ROTATE_0_FLIP,
  FLIP_HORIZONTAL = ROTATE_180_FLIP,
};

SoftwareSurface transform(SoftwareSurface const& surface, Transform mod);
SoftwareSurface rotate90(SoftwareSurface const& surface);
SoftwareSurface rotate180(SoftwareSurface const& surface);
SoftwareSurface rotate270(SoftwareSurface const& surface);
SoftwareSurface flip_vertical(SoftwareSurface const& surface);
SoftwareSurface flip_horizontal(SoftwareSurface const& surface);

} // namespace surf

#endif

/* EOF */
