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

#ifndef HEADER_GALAPIX_PLUGINS_MEM_JPEG_COMPRESSOR_HPP
#define HEADER_GALAPIX_PLUGINS_MEM_JPEG_COMPRESSOR_HPP

#include <vector>
#include <stdint.h>

#include <surf/plugins/jpeg_compressor.hpp>

namespace surf {

class MemJPEGCompressor : public JPEGCompressor
{
public:
  MemJPEGCompressor(std::vector<uint8_t>& data);
  ~MemJPEGCompressor() override;

private:
  MemJPEGCompressor(const MemJPEGCompressor&);
  MemJPEGCompressor& operator=(const MemJPEGCompressor&);
};

} // namespace surf

#endif

/* EOF */
