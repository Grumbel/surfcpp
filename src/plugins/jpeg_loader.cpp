/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
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

#include "plugins/jpeg_loader.hpp"

#include <sstream>
#include <stdexcept>

void
JPEGLoader::fatal_error_handler(j_common_ptr cinfo)
{
  longjmp(reinterpret_cast<ErrorMgr*>(cinfo->err)->setjmp_buffer, 1);
}

JPEGLoader::JPEGLoader()
{
  jpeg_std_error(&m_err.pub);

  m_err.pub.error_exit = &JPEGLoader::fatal_error_handler;

  m_cinfo.err = &m_err.pub;

  jpeg_create_decompress(&m_cinfo);
}

JPEGLoader::~JPEGLoader()
{
  jpeg_destroy_decompress(&m_cinfo);
}

Size
JPEGLoader::get_size() const
{
  return Size(m_cinfo.image_width,
              m_cinfo.image_height);
}

void
JPEGLoader::read_header()
{
  if (setjmp(m_err.setjmp_buffer))
  {
    char buffer[JMSG_LENGTH_MAX];
    (m_cinfo.err->format_message)(reinterpret_cast<jpeg_common_struct*>(&m_cinfo), buffer);

    std::ostringstream out;
    out << "Error: JPEG::read_header: " /*<< filename << ": "*/ << buffer;

    throw std::runtime_error(out.str());
  }
  else
  {
    jpeg_read_header(&m_cinfo, /*require_image*/ FALSE);
  }
}

SoftwareSurfaceHandle
JPEGLoader::read_image(int scale)
{
  if (setjmp(m_err.setjmp_buffer))
  {
    char buffer[JMSG_LENGTH_MAX];
    (m_cinfo.err->format_message)(reinterpret_cast<jpeg_common_struct*>(&m_cinfo), buffer);

    std::ostringstream out;
    out << "Error: JPEG::read_image: " /*<< filename << ": "*/ << buffer;
    throw std::runtime_error(out.str());
  }
  else
  {
    jpeg_read_header(&m_cinfo, /*require_image*/ FALSE);

    if (scale != 1) // scale the image down by \a scale
    { 
      // by default all those values below are on 1
      m_cinfo.scale_num           = 1;
      m_cinfo.scale_denom         = scale;
   
      m_cinfo.do_fancy_upsampling = FALSE; /* TRUE=apply fancy upsampling */
      m_cinfo.do_block_smoothing  = FALSE; /* TRUE=apply interblock smoothing */
    }

    jpeg_start_decompress(&m_cinfo);

    SoftwareSurfaceHandle surface = SoftwareSurface::create(SoftwareSurface::RGB_FORMAT, Size(m_cinfo.output_width,
                                                                                              m_cinfo.output_height));
 
    if (m_cinfo.output_components == 3) // RGB Image
    { 
      boost::scoped_array<JSAMPLE*> scanlines(new JSAMPLE*[m_cinfo.output_height]);

      for(JDIMENSION y = 0; y < m_cinfo.output_height; ++y)
        scanlines[y] = surface->get_row_data(y);

      while (m_cinfo.output_scanline < m_cinfo.output_height) 
      {
        jpeg_read_scanlines(&m_cinfo, &scanlines[m_cinfo.output_scanline], 
                            m_cinfo.output_height - m_cinfo.output_scanline);
      }
    }
    else if (m_cinfo.output_components == 1)  // Greyscale Image
    {
      boost::scoped_array<JSAMPLE*> scanlines(new JSAMPLE*[m_cinfo.output_height]);

      for(JDIMENSION y = 0; y < m_cinfo.output_height; ++y)
        scanlines[y] = surface->get_row_data(y);

      while (m_cinfo.output_scanline < m_cinfo.output_height) 
      {
        jpeg_read_scanlines(&m_cinfo, &scanlines[m_cinfo.output_scanline], 
                            m_cinfo.output_height - m_cinfo.output_scanline);
      }

      // Expand the greyscale data to RGB
      // FIXME: Could be made faster if SoftwareSurface would support
      // other color formats
      for(int y = 0; y < surface->get_height(); ++y)
      {
        uint8_t* rowptr = surface->get_row_data(y);
        for(int x = surface->get_width()-1; x >= 0; --x)
        {
          rowptr[3*x+0] = rowptr[x];
          rowptr[3*x+1] = rowptr[x];
          rowptr[3*x+2] = rowptr[x];
        }
      }
    }
    else
    {
      std::ostringstream str;
      str << "JPEG: Unsupported color depth: " << m_cinfo.output_components;
      throw std::runtime_error(str.str());
    }

    return surface;
  }
}

/* EOF */