#include <string.h>
#include <algorithm>
#include <vector>
#include <benchmark/benchmark.h>

#include <surf/pixel_data.hpp>

using namespace surf;

namespace {

const geom::isize SRCSIZE(64, 64);
const geom::isize DSTSIZE(1024, 1024);

void PixelData_blit_to(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        src.blit_to(dst, geom::ipoint(x, y));
      }
    }
  }
}

void PixelData_blit_to__slow_copy(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        src.blit_to__slow_copy(dst, geom::ipoint(x, y));
      }
    }
  }
}

void PixelData_blit_to__slow(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBPixel> dst(DSTSIZE, RGBPixel{0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        src.blit_to__slow(dst, geom::ipoint(x, y));
      }
    }
  }
}

void PixelData_blit_to__convert(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBAPixel> dst(DSTSIZE, RGBAPixel{0, 0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        src.blit_to(dst, geom::ipoint(x, y));
      }
    }
  }
}

void PixelData_blit_to__slow_convert(benchmark::State& state)
{
  PixelData<RGBPixel> src(SRCSIZE, RGBPixel{255, 255, 255});
  PixelData<RGBAPixel> dst(DSTSIZE, RGBAPixel{0, 0, 0, 0});

  while (state.KeepRunning()) {
    for (int y = 0; y < 1024; y += 100) {
      for (int x = 0; x < 1024; x += 100) {
        src.blit_to__slow(dst, geom::ipoint(x, y));
      }
    }
  }
}

void PixelData_fill(benchmark::State& state)
{
  PixelData<RGBPixel> src(DSTSIZE, RGBPixel{255, 255, 255});

  while (state.KeepRunning()) {
    src.fill(RGBPixel{12, 34, 56});
  }
}

void PixelData_fill__slow(benchmark::State& state)
{
  PixelData<RGBPixel> src(DSTSIZE, RGBPixel{255, 255, 255});

  while (state.KeepRunning()) {
    src.fill__slow(RGBPixel{12, 34, 56});
  }
}

void PixelData_fill__all(benchmark::State& state)
{
  PixelData<RGBPixel> src(DSTSIZE, RGBPixel{255, 255, 255});

  while (state.KeepRunning()) {
    src.fill__all(RGBPixel{12, 34, 56});
  }
}

} // namespace

BENCHMARK(PixelData_blit_to);
BENCHMARK(PixelData_blit_to__slow);
BENCHMARK(PixelData_blit_to__slow_copy);

BENCHMARK(PixelData_blit_to__convert);
BENCHMARK(PixelData_blit_to__slow_convert);

BENCHMARK(PixelData_fill);
BENCHMARK(PixelData_fill__slow);
BENCHMARK(PixelData_fill__all);

BENCHMARK_MAIN();

/* EOF */
