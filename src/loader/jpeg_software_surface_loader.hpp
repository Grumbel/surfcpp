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

#ifndef HEADER_GALAPIX_UTIL_JPEG_SOFTWARE_SURFACE_LOADER_HPP
#define HEADER_GALAPIX_UTIL_JPEG_SOFTWARE_SURFACE_LOADER_HPP

#include "plugins/jpeg.hpp"

namespace surf {

class JPEGSoftwareSurfaceLoader : public SoftwareSurfaceLoader
{
public:
  JPEGSoftwareSurfaceLoader()
  {}

  std::string get_name() const override
  {
    return "jpeg";
  }

  void register_loader(SoftwareSurfaceFactory& factory) const override
  {
    factory.register_by_magic(this, "\xff\xd8");
    factory.register_by_mime_type(this, "image/jpeg");
    factory.register_by_extension(this, "jpeg");
    factory.register_by_extension(this, "jpg");
  }

  bool supports_from_file() const override { return true; }
  bool supports_from_mem() const override { return true; }

  SoftwareSurface from_file(std::filesystem::path const& filename) const override
  {
    return jpeg::load_from_file(filename);
  }

  SoftwareSurface from_mem(std::span<uint8_t const> data) const override
  {
    return jpeg::load_from_mem(data);
  }

private:
  JPEGSoftwareSurfaceLoader(const JPEGSoftwareSurfaceLoader&);
  JPEGSoftwareSurfaceLoader& operator=(const JPEGSoftwareSurfaceLoader&);
};

} // namespace surf

#endif

/* EOF */
