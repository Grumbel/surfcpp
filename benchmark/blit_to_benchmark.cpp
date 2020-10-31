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

void blit(benchmark::State& state)
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

void blit__copy(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blit__copy(src, geom::irect(src.get_size()), dst, geom::ipoint(x, y));
      }
    }
  }
}

void blit__slow(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blit__slow(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void blit__convert(benchmark::State& state)
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

void blit__slow_convert(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBAPixel> dst(DSTSIZE, RGBAPixel{0, 0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        blit__slow(src, dst, geom::ipoint(x, y));
      }
    }
  }
}

void fill(benchmark::State& state)
{
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{255, 255, 255});

  while (state.KeepRunning()) {
    fill(dst, RGBPixel{12, 34, 56});
  }
}

void fill__slow(benchmark::State& state)
{
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{255, 255, 255});

  while (state.KeepRunning()) {
    fill__slow(dst, RGBPixel{12, 34, 56});
  }
}

} // namespace

BENCHMARK(blit);
BENCHMARK(blit__copy);
BENCHMARK(blit__slow);

BENCHMARK(blit__convert);
BENCHMARK(blit__slow_convert);

BENCHMARK(fill);
BENCHMARK(fill__slow);

BENCHMARK_MAIN();

/* EOF */
