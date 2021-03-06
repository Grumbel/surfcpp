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

#ifndef HEADER_GALAPIX_PLUGINS_PNM_MEM_READER_HPP
#define HEADER_GALAPIX_PLUGINS_PNM_MEM_READER_HPP

#include <geom/size.hpp>

namespace surf {

class PNMMemReader
{
public:
  PNMMemReader(std::span<uint8_t const> data) :
    m_data(data),
    m_ptr(data.data()),
    m_token_count(),
    m_magic(),
    m_size(),
    m_maxval(),
    m_pixel_data()
  {
    parse();
  }

  std::string get_magic() const { return m_magic; }
  uint8_t const* get_pixel_data() const { return m_pixel_data; }
  geom::isize get_size() const { return m_size; }
  int  get_maxval() const { return m_maxval; }

private:
  bool eof() const { return m_ptr >= (m_data.data() + m_data.size()); }
  void forward() { m_ptr += 1; }

  void parse()
  {
    m_token_count = 0;

    while(!eof())
    {
      if (*m_ptr == '#')
      {
        // skip comment
        forward();
        while(!eof() && *m_ptr != '\n') {
          forward();
        }
      }
      else if (isspace(*m_ptr))
      {
        // skip whitespace
        forward();
        while(!eof() && isspace(*m_ptr)) {
          forward();
        }
      }
      else
      {
        uint8_t const* start = m_ptr;
        forward();
        while(!eof() && !isspace(*m_ptr) && *m_ptr != '#')
        {
          forward();
        }

        std::string token(start, m_ptr);
        switch(m_token_count)
        {
          case 0:
            m_magic = token;
            break;

          case 1:
            m_size = geom::isize(atoi(token.c_str()), m_size.height());
            break;

          case 2:
            m_size = geom::isize(m_size.width(), atoi(token.c_str()));
            break;

          case 3:
            m_maxval = atoi(token.c_str());
            forward();
            m_pixel_data = m_ptr;
            m_ptr = m_data.data() + m_data.size(); // set ptr to EOF
            break;
        }

        m_token_count += 1;
      }
    }
  }

private:
  std::span<uint8_t const> const m_data;
  uint8_t const* m_ptr;
  int m_token_count;

  std::string m_magic;
  geom::isize m_size;
  int m_maxval;
  uint8_t const* m_pixel_data;

private:
  PNMMemReader(const PNMMemReader&);
  PNMMemReader& operator=(const PNMMemReader&);
};

} // namespace surf

#endif

/* EOF */
