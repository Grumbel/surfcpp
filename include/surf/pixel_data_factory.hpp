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

#ifndef HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_FACTORY_HPP
#define HEADER_GALAPIX_UTIL_SOFTWARE_SURFACE_FACTORY_HPP

#include <filesystem>
#include <span>
#include <map>
#include <string>

#include "pixel_data.hpp"

namespace surf {

class SoftwareSurface;
class PixelDataLoader;

class PixelDataFactory
{
private:
  typedef std::map<std::string, const PixelDataLoader*> ExtensionMap;
  typedef std::map<std::string, const PixelDataLoader*> MimeTypeMap;
  typedef std::map<std::string, const PixelDataLoader*> MagicMap;

public:
  PixelDataFactory();
  ~PixelDataFactory();

  void add_loader(std::unique_ptr<PixelDataLoader> loader);
  bool has_supported_extension(std::filesystem::path const& filename);

  /** Files are handled in the order of mime-type, extension, magic,
      if one fails, the next in line is tried. Extension is the most
      important one, as it is used to filter valid files when
      generating file lists */
  void register_by_magic(PixelDataLoader const& loader, const std::string& magic);
  void register_by_mime_type(PixelDataLoader const& loader, const std::string& mime_type);
  void register_by_extension(PixelDataLoader const& loader, const std::string& extension);

  PixelDataLoader const* find_loader_by_filename(std::filesystem::path const& filename) const;
  PixelDataLoader const* find_loader_by_magic(const std::string& data) const;
  PixelDataLoader const* find_loader_by_magic(std::span<uint8_t const> data) const;

  SoftwareSurface from_mem(std::span<uint8_t const> data, std::string const& mime_type, std::filesystem::path const& filename) const;
  SoftwareSurface from_file(std::filesystem::path const& filename) const;
  SoftwareSurface from_file(std::filesystem::path const& filename, std::string_view loader) const;
  SoftwareSurface from_file(std::filesystem::path const& filename, PixelDataLoader const& loader) const;

private:
  std::vector<std::unique_ptr<PixelDataLoader> > m_loader;

  ExtensionMap m_extension_map;
  MimeTypeMap  m_mime_type_map;
  MagicMap m_magic_map;

private:
  PixelDataFactory(const PixelDataFactory&);
  PixelDataFactory& operator=(const PixelDataFactory&);
};

} // namespace surf

#endif

/* EOF */
