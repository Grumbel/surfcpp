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

#include "pixel_data_factory.hpp"

#include <sstream>
#include <stdexcept>

#include <logmich/log.hpp>

#include "util/filesystem.hpp"
#include "pixel_data_loader.hpp"

#include "plugins/jpeg.hpp"
#if 0
#include "plugins/dds.hpp"
#include "plugins/imagemagick.hpp"
#include "plugins/kra.hpp"
#include "plugins/png.hpp"
#include "plugins/rsvg.hpp"
#include "plugins/ufraw.hpp"
#include "plugins/vidthumb.hpp"
#include "plugins/xcf.hpp"
#endif

namespace surf {

PixelDataFactory::PixelDataFactory() :
  m_loader(),
  m_extension_map(),
  m_mime_type_map(),
  m_magic_map()
{
  // order matters, first come, first serve, later registrations for
  // an already registered type will be ignored
  jpeg::register_loader(*this);
#if 0
  png::register_loader(
    *this);

  if (xcf::is_available()) {
    xcf::register_loader(*this);
  }

  if (ufraw::is_available()) {
    ufraw::register_loader(*this);
  }

  if (rsvg::is_available()) {
    rsvg::register_loader(*this);
  }

  if (vidthumb::is_available()) {
    vidthumb::register_loader(*this);
  }

  if (kra::is_available()) {
    kra::register_loader(*this);
  }

  dds::register_loader(*this);

#ifdef HAVE_MAGICKXX
  imagemagick::register_loader(*this);
#endif

#endif
}

PixelDataFactory::~PixelDataFactory()
{
}

void
PixelDataFactory::add_loader(std::unique_ptr<PixelDataLoader> loader)
{
  m_loader.push_back(std::move(loader));
}

bool
PixelDataFactory::has_supported_extension(std::filesystem::path const& filename)
{
  std::string extension = Filesystem::get_extension(filename);
  ExtensionMap::iterator i = m_extension_map.find(extension);
  return (i != m_extension_map.end());
}

void
PixelDataFactory::register_by_magic(PixelDataLoader const& loader, const std::string& magic)
{
  auto it = m_magic_map.find(magic);
  if (it == m_magic_map.end())
  {
    m_magic_map[magic] = &loader;
  }
  else
  {
    // ignoring registration if something is already registered
  }
}

void
PixelDataFactory::register_by_mime_type(PixelDataLoader const& loader, const std::string& mime_type)
{
  MimeTypeMap::iterator i = m_mime_type_map.find(mime_type);
  if (i == m_mime_type_map.end())
  {
    m_mime_type_map[mime_type] = &loader;
  }
  else
  {
    // ignoring registration if something is already registered
  }
}

void
PixelDataFactory::register_by_extension(PixelDataLoader const& loader, const std::string& extension)
{
  ExtensionMap::iterator i = m_extension_map.find(extension);
  if (i == m_extension_map.end())
  {
    m_extension_map[extension] = &loader;
  }
  else
  {
    // ignoring registration if something is already registered
  }
}

PixelDataLoader const*
PixelDataFactory::find_loader_by_filename(std::filesystem::path const& filename) const
{
  std::string extension = Filesystem::get_extension(filename);
  const auto& it = m_extension_map.find(extension);
  if (it == m_extension_map.end())
  {
    return nullptr;
  }
  else
  {
    return it->second;
  }
}

PixelDataLoader const*
PixelDataFactory::find_loader_by_magic(const std::string& data) const
{
  for(const auto& it: m_magic_map)
  {
    if (data.starts_with(it.first))
    {
      return it.second;
    }
  }
  return nullptr;
}

PixelDataLoader const*
PixelDataFactory::find_loader_by_magic(std::span<uint8_t const> data) const
{
  size_t size = std::min(static_cast<size_t>(1024), data.size());
  return find_loader_by_magic(std::string(reinterpret_cast<const char*>(data.data()), size));
}

SoftwareSurface
PixelDataFactory::from_file(std::filesystem::path const& filename, PixelDataLoader const& loader) const
{
  if (loader.supports_from_file())
  {
    return loader.from_file(filename);
  }
#if 0
  else if (loader.supports_from_mem())
  {
    Blob blob = Blob::from_file(filename);
    return loader.from_mem(blob);
  }
#endif
  else
  {
    throw std::runtime_error("'" + loader.get_name() + "' loader does not support loading");
  }
}

SoftwareSurface
PixelDataFactory::from_file(std::filesystem::path const& filename, std::string_view loader) const
{
  auto const it = std::find_if(m_loader.begin(), m_loader.end(), [loader](auto&& loader_p) {
    return loader_p->get_name() == loader;
  });

  if (it == m_loader.end()) {
    throw std::runtime_error(fmt::format("non-existing loader specified: {}", loader));
  }

  return (*it)->from_file(filename);
}

SoftwareSurface
PixelDataFactory::from_file(std::filesystem::path const& filename) const
{
  const PixelDataLoader* loader = find_loader_by_filename(filename);
  if (!loader) {
    std::ostringstream out;
    out << "PixelDataFactory::from_file(): " << filename << ": unknown file type";
    throw std::runtime_error(out.str());
  }

  try
  {
    return from_file(filename, *loader);
  }
  catch (const std::exception& err)
  {
    // retry with magic
    std::string magic = Filesystem::get_magic(filename);
    const auto* new_loader = find_loader_by_magic(magic);
    if (new_loader && new_loader != loader)
    {
      log_warn("{}: file extension error, file is a {}", filename, new_loader->get_name());
      return from_file(filename, *new_loader);
    }
    else
    {
      throw;
    }
  }
}

SoftwareSurface
PixelDataFactory::from_mem(std::span<uint8_t const> data,
                                 std::string const& mime_type,
                                 std::filesystem::path const& filename) const
{
  const PixelDataLoader* loader = nullptr;

  // try to find a loader by mime-type
  if (!mime_type.empty())
  {
    MimeTypeMap::const_iterator i = m_mime_type_map.find(mime_type);
    if (i != m_mime_type_map.end())
    {
      loader = i->second;
    }
  }

  // try to find a loader by file extension
  if (!loader)
  {
    std::string extension = Filesystem::get_extension(filename);
    ExtensionMap::const_iterator i = m_extension_map.find(extension);
    if (i != m_extension_map.end())
    {
      loader = i->second;
    }
  }

  // load the image or fail if no loader is present
  if (!loader)
  {
    std::ostringstream out;
    out << "PixelDataFactory::from_url(): " << filename << ": unknown file type";
    throw std::runtime_error(out.str());
  }
  else
  {
    if (loader->supports_from_mem())
    {
      return loader->from_mem(data);
    }
    else
    {
      std::ostringstream out;
      out << "PixelDataFactory::from_url(): " << filename << ": loader doesn't support from_mem(), workaround not implemented";
      throw std::runtime_error(out.str());
    }
  }
}

} // namespace surf

/* EOF */
