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

#include <sstream>
#include <setjmp.h>

#include <geom/size.hpp>

#include "util/filesystem.hpp"
#include "plugins/exif.hpp"
#include "plugins/file_jpeg_compressor.hpp"
#include "plugins/file_jpeg_decompressor.hpp"
#include "plugins/jpeg.hpp"
#include "plugins/mem_jpeg_compressor.hpp"
#include "plugins/mem_jpeg_decompressor.hpp"

namespace surf {

namespace {

geom::isize apply_orientation(Transform modifier, const geom::isize& size)
{
  switch(modifier)
  {
    case Transform::ROTATE_90:
    case Transform::ROTATE_90_FLIP:
    case Transform::ROTATE_270:
    case Transform::ROTATE_270_FLIP:
      return geom::transpose(size);

    case Transform::ROTATE_0:
    case Transform::ROTATE_0_FLIP:
    case Transform::ROTATE_180:
    case Transform::ROTATE_180_FLIP:
    default:
      return size;
  }
}

} // namespace

bool
JPEG::filename_is_jpeg(std::filesystem::path const& filename)
{
  // FIXME: Merge this with util/jpeg_software_surface_loader, maybe
  // store the fileformat/SoftwareSurfaceLoader in the database intead
  // of figuring out the format each time anew
  return (Filesystem::get_extension(filename) == "jpg" ||
          Filesystem::get_extension(filename) == "jpeg");
}

geom::isize
JPEG::get_size(std::filesystem::path const& filename)
{
  FileJPEGDecompressor loader(filename);
  geom::isize size = loader.read_size();
  return apply_orientation(EXIF::get_orientation(filename), size);
}


geom::isize
JPEG::get_size(std::span<uint8_t const> data)
{
  MemJPEGDecompressor loader(data);
  geom::isize size = loader.read_size();
  return apply_orientation(EXIF::get_orientation(data), size);
}


SoftwareSurface
JPEG::load_from_file(std::filesystem::path const& filename, int scale, geom::isize* image_size)
{
  FileJPEGDecompressor loader(filename);
  SoftwareSurface surface = loader.read_image(scale, image_size);

  Transform modifier = EXIF::get_orientation(filename);

  if (image_size) {
    *image_size = apply_orientation(modifier, *image_size);
  }

  if (modifier == Transform::ROTATE_0) {
    return surface;
  } else {
    return transform(surface, modifier);
  }
}


SoftwareSurface
JPEG::load_from_mem(std::span<uint8_t const> data, int scale, geom::isize* image_size)
{
  MemJPEGDecompressor loader(data);
  SoftwareSurface surface = loader.read_image(scale, image_size);

  Transform modifier = EXIF::get_orientation(data);

  if (image_size) {
    *image_size = apply_orientation(modifier, *image_size);
  }

  if (modifier == Transform::ROTATE_0) {
    return surface;
  } else {
    return transform(surface, modifier);
  }
}


void
JPEG::save(SoftwareSurface const& surface, int quality, std::filesystem::path const& filename)
{
  FileJPEGCompressor compressor(filename);
  compressor.save(surface, quality);
}

std::vector<uint8_t>
JPEG::save(SoftwareSurface const& surface, int quality)
{
  std::vector<uint8_t> data;
  MemJPEGCompressor compressor(data);
  compressor.save(surface, quality);
  return data;
}

} // namespace surf

/* EOF */
