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

#include "plugins/file_jpeg_decompressor.hpp"

#include <sstream>
#include <stdexcept>
#include <string.h>
#include <errno.h>

namespace surf {

FileJPEGDecompressor::FileJPEGDecompressor(std::filesystem::path const& filename) :
  m_filename(filename),
  m_in(fopen(filename.string().c_str(), "rb"))
{
  if (!m_in)
  {
    std::ostringstream out;
    out << "FileJPEGDecompressor(): " << filename << ": " << strerror(errno);
    throw std::runtime_error(out.str());
  }
  else
  {
    jpeg_stdio_src(&m_cinfo, m_in);
  }
}

FileJPEGDecompressor::~FileJPEGDecompressor()
{
  fclose(m_in);
}

} // namespace surf

/* EOF */
