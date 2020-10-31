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

#ifndef HEADER_SURF_BLIT_HPP
#define HEADER_SURF_BLIT_HPP

#include <cassert>
#include <cstring>

#include "blend.hpp"
#include "pixel_data.hpp"

namespace surf {

// template<typename SrcPixel, typename DstPixel>
// void blit(PixelData<SrcPixel>& src, PixelData<DstPixel>& dst, const geom::ipoint& pos)
// {
//   static_assert(!std::is_same<SrcPixel, SrcPixel>::value,
//                 "blit() not implemented for the given types");
// }

template<typename SrcPixel, typename DstPixel,
         std::enable_if_t<std::is_same<SrcPixel, DstPixel>::value, int> = 0>
void blit(PixelData<SrcPixel> const& src, geom::irect const& srcrect,
          PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  assert(contains(geom::irect(src.get_size()), srcrect));

  geom::irect const cliprect(dst.get_size());
  geom::irect const region = intersection(geom::irect(srcrect.size()) + geom::ioffset(pos), cliprect);
  geom::ioffset const dst2src(-pos.x() + srcrect.left(), -pos.y() + srcrect.top());

  for (int y = region.top(); y < region.bottom(); ++y) {
    std::memcpy(dst.get_row(y) + region.left(),
                src.get_row(y + dst2src.y()) + region.left() + dst2src.x(),
                region.width() * sizeof(SrcPixel));
  }
}

template<typename SrcPixel, typename DstPixel,
         std::enable_if_t<std::is_same<SrcPixel, DstPixel>::value, int> = 0>
void blit__copy(PixelData<SrcPixel> const& src, geom::irect const& srcrect,
                PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  assert(contains(geom::irect(src.get_size()), srcrect));

  geom::irect const cliprect(dst.get_size());
  geom::irect const region = intersection(geom::irect(srcrect.size()) + geom::ioffset(pos), cliprect);
  geom::ioffset const dst2src(-pos.x() + srcrect.left(), -pos.y() + srcrect.top());

  for (int y = region.top(); y < region.bottom(); ++y) {
    std::copy_n(src.get_row(y + dst2src.y()) + region.left() + dst2src.x(),
                region.width(),
                dst.get_row(y) + region.left());
  }
}

template<typename SrcPixel, typename DstPixel,
         std::enable_if_t<!std::is_same<SrcPixel, DstPixel>::value, int> = 0>
void blit(PixelData<SrcPixel> const& src, geom::irect const& srcrect,
          PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  assert(contains(geom::irect(src.get_size()), srcrect));

  geom::irect const cliprect(dst.get_size());
  geom::irect const region = intersection(geom::irect(srcrect.size()) + geom::ioffset(pos), cliprect);
  geom::ioffset const dst2src(-pos.x() + srcrect.left(), -pos.y() + srcrect.top());

  for (int y = region.top(); y < region.bottom(); ++y) {
    std::transform(src.get_row(y + dst2src.y()) + region.left() + dst2src.x(),
                   src.get_row(y + dst2src.y()) + region.left() + dst2src.x() + region.width(),
                   dst.get_row(y) + region.left(),
                   convert<SrcPixel, DstPixel>);
  }
}

/** Performs a simple copy from this to \a test, no blending is performed */
template<typename SrcPixel, typename DstPixel>
void blit(PixelData<SrcPixel> const& src, PixelData<DstPixel>& dst, geom::ipoint const& pos)
{
  blit(src, geom::irect(src.get_size()), dst, pos);
}

template<typename SrcPixel, typename DstPixel, typename BlendFunc>
void blend_n(SrcPixel const* srcpixels, DstPixel* dstpixels,
             size_t count, BlendFunc blend_func)
{
  for (size_t i = 0; i < count; ++i) {
    dstpixels[i] = blend_func(srcpixels[i], dstpixels[i]);
  }
}

template<typename SrcPixel, typename DstPixel>
void blend(PixelData<SrcPixel> const& src, geom::irect const& srcrect,
           PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  assert(contains(geom::irect(src.get_size()), srcrect));

  geom::irect const cliprect(dst.get_size());
  geom::irect const region = intersection(geom::irect(srcrect.size()) + geom::ioffset(pos), cliprect);
  geom::ioffset const dst2src(-pos.x() + srcrect.left(), -pos.y() + srcrect.top());

  for (int y = region.top(); y < region.bottom(); ++y) {
    blend_n(src.get_row(y + dst2src.y()) + region.left() + dst2src.x(),
            dst.get_row(y) + region.left(),
            region.width(),
            pixel_blend<SrcPixel, DstPixel>);
  }
}

template<typename SrcPixel, typename DstPixel>
void blend(PixelData<SrcPixel> const& src, PixelData<DstPixel>& dst, geom::ipoint const& pos)
{
  blend(src, geom::irect(src.get_size()), dst, pos);
}

template<typename SrcPixel, typename DstPixel>
void blit__slow(PixelData<SrcPixel>& src, PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  int const start_x = std::max(0, -pos.x());
  int const start_y = std::max(0, -pos.y());

  int const end_x = std::min(src.get_size().width(), dst.get_size().width()  - pos.x());
  int const end_y = std::min(src.get_size().height(), dst.get_size().height() - pos.y());

  for(int y = start_y; y < end_y; ++y) {
    for(int x = start_x; x < end_x; ++x) {
      SrcPixel const srcpx = src.get_pixel(geom::ipoint(x, y));
      dst.put_pixel(geom::ipoint(pos.x() + x - start_x, pos.y() + y - start_y), convert<SrcPixel, DstPixel>(srcpx));
    }
  }
}

#if 0
template<typename DstPixel>
void blit(PixelData<SrcPixel>& src, geom::irect const& srcrect, PixelData<DstPixel>& dst, const geom::ipoint& pos)
{
  int const start_x = std::max(0, -pos.x()) + srcrect.x();
  int const start_y = std::max(0, -pos.y()) + srcrect.y();

  int const end_x = std::min(srcrect.right(), srcrect.x() + dst.get_size().width()  - pos.x());
  int const end_y = std::min(srcrect.bottom(), srcrect.y() + dst.get_size().height() - pos.y());

  for(int y = start_y; y < end_y; ++y) {
    for(int x = start_x; x < end_x; ++x) {
      Pixel const srcpx = get_pixel(geom::ipoint(x, y));
      dst.put_pixel(geom::ipoint(pos.x() + x - start_x,
                                 pos.y() + y - start_y),
                    convert<Pixel, DstPixel>(srcpx));
    }
  }
}
#endif

} // namespace surf

#endif

/* EOF */
