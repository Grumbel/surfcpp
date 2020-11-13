#include <string.h>
#include <algorithm>
#include <vector>
#include <benchmark/benchmark.h>

#include <surf/pixel_data.hpp>
#include <surf/blit.hpp>
#include <surf/fill.hpp>

using namespace surf;

namespace {

const geom::isize SRCSIZE(64, 64);
const geom::isize DSTSIZE(1024, 1024);

void BM_blit(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blit(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__copy(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        experimental::blit__copy(src, geom::irect(src.get_size()), dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__blend(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blend(pixel_copy<RGBPixel, RGBPixel>(), src, geom::irect(src.get_size()), dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__blend_blend(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blend(pixel_blend<RGBPixel, RGBPixel>(), src, geom::irect(src.get_size()), dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__blend_add(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blend(pixel_add<RGBPixel, RGBPixel>(), src, geom::irect(src.get_size()), dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__slow(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        experimental::blit__slow(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__convert(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBAPixel> dst(DSTSIZE, RGBAPixel{0, 0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blit(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__slow_convert(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBAPixel> dst(DSTSIZE, RGBAPixel{0, 0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        experimental::blit__slow(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void BM_blit__slow_convert_self(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        experimental::blit__slow(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

} // namespace

BENCHMARK(BM_blit);
BENCHMARK(BM_blit__blend);
BENCHMARK(BM_blit__blend_add);
BENCHMARK(BM_blit__blend_blend);
BENCHMARK(BM_blit__copy);
BENCHMARK(BM_blit__slow);

BENCHMARK(BM_blit__convert);
BENCHMARK(BM_blit__slow_convert);
BENCHMARK(BM_blit__slow_convert_self);

/* EOF */
