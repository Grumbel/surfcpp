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

#include <filesystem>
#include <iostream>

#include <fmt/format.h>

#include <surf/surf.hpp>
#include <surf/io.hpp>

using surf::SoftwareSurface;

namespace {

surf::Transform transform_from_string(std::string_view text)
{
  if (text == "0") {
    return surf::Transform::ROTATE_0;
  } else if (text == "90") {
    return surf::Transform::ROTATE_90;
  } else if (text == "180") {
    return surf::Transform::ROTATE_180;
  } else if (text == "270") {
    return surf::Transform::ROTATE_270;
  } else if (text == "0flip") {
    return surf::Transform::ROTATE_0_FLIP;
  } else if (text == "90flip") {
    return surf::Transform::ROTATE_90_FLIP;
  } else if (text == "180flip") {
    return surf::Transform::ROTATE_180_FLIP;
  } else if (text == "270flip") {
    return surf::Transform::ROTATE_270_FLIP;
  } else if (text == "flip") {
    return surf::Transform::ROTATE_0_FLIP;
  } else if (text == "vflip") {
    return surf::Transform::FLIP_VERTICAL;
  } else if (text == "hflip") {
    return surf::Transform::FLIP_HORIZONTAL;
  } else {
    throw std::invalid_argument(fmt::format("not a valid transform: {}", text));
  }
}

struct FileOptions
{
  std::filesystem::path input_filename = {};
  std::vector<std::function<void(SoftwareSurface&)>> filters = {};
  std::optional<std::filesystem::path> output_filename = {};
};

struct Options
{
  std::vector<FileOptions> files = {};
};

void print_usage(int argc, char** argv)
{
  std::cout << argv[0] << " ( [IMGFILE] [OPTION] )...\n";
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  for (int i = 1; i < argc; ++i) {
    auto next_arg = [&]{
      if (++i >= argc) {
        std::ostringstream msg;
        msg << argv[i - 1] << " needs an argument";
        throw std::runtime_error(msg.str());
      }
    };

    auto file_opts = [&]() -> FileOptions& {
      if (opts.files.empty()) {
        std::ostringstream os;
        os << argv[i - 1] << " option must follow filename, not precede it";
        throw std::runtime_error(os.str());
      }
      return opts.files.back();
    };

    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--help") == 0 ||
          strcmp(argv[i], "-h") == 0) {
        print_usage(argc, argv);
        exit(EXIT_SUCCESS);
      } else if (strcmp(argv[i], "--invert") == 0) {
        file_opts().filters.emplace_back([](SoftwareSurface& sur) {
          surf::apply_invert(sur);
        });
      } else if (strcmp(argv[i], "--gamma") == 0) {
        next_arg();
        float value = std::stof(argv[i]);
        file_opts().filters.emplace_back([value](SoftwareSurface& sur) {
          surf::apply_gamma(sur, value);
        });
      } else if (strcmp(argv[i], "--brightness") == 0) {
        next_arg();
        float value = std::stof(argv[i]);
        file_opts().filters.emplace_back([value](SoftwareSurface& sur) {
          surf::apply_brightness(sur, value);
        });
      } else if (strcmp(argv[i], "--scale") == 0) {
        next_arg();
        int width;
        int height;
        char op = ' ';
        if (sscanf(argv[i], "%dx%d%c", &width, &height, &op) == 3) {
          // ok
        } else if (sscanf(argv[i], "%dx%d", &width, &height) == 2) {
          //ok
        } else {
          throw std::invalid_argument(fmt::format("expected WxH argument for options {}", argv[i - 1]));
        }
        file_opts().filters.emplace_back([width, height, op](SoftwareSurface& sur) {
          geom::isize size(width, height);
          if (op == '!') {
            // nop
          } else {
            size = geom::resize_to_fit(sur.get_size(), geom::isize(width, height));
          }
          sur = surf::scale(sur, size);
        });
      } else if (strcmp(argv[i], "--crop") == 0) {
        next_arg();
        int x = 0;
        int y = 0;
        int width;
        int height;
        if (sscanf(argv[i], "%dx%d+%d+%d", &width, &height, &x, &y) == 4) {
          // ok
        } else if (sscanf(argv[i], "%dx%d", &width, &height) == 2) {
          // ok
        } else {
          throw std::invalid_argument(fmt::format("expected WxH argument for options {}", argv[i - 1]));
        }

        file_opts().filters.emplace_back([x, y, width, height](SoftwareSurface& sur) {
          sur = surf::crop(sur, geom::irect(geom::ipoint(x, y), geom::isize(width, height)));
        });
      } else if (strcmp(argv[i], "--transform") == 0) {
        next_arg();
        surf::Transform const transf = transform_from_string(argv[i]);
        file_opts().filters.emplace_back([transf](SoftwareSurface& sur) {
          sur = surf::transform(sur, transf);
        });
      } else if (strcmp(argv[i], "--output") == 0 ||
                 strcmp(argv[i], "-o") == 0) {
        next_arg();
        file_opts().output_filename = argv[i];
      } else {
        print_usage(argc, argv);
        throw std::invalid_argument(fmt::format("unknown option: {}", argv[i]));
      }
    } else {
      opts.files.emplace_back(FileOptions{.input_filename = argv[i]});
    }
  }

  return opts;
}

void run(int argc, char** argv)
{
  Options opts = parse_args(argc, argv);

  for (auto&& fileopt : opts.files) {
    std::cout << fileopt.input_filename << std::endl;
    auto img = SoftwareSurface::from_file(fileopt.input_filename);
    for (auto&& filter : fileopt.filters) {
      filter(img);
    }
    if (fileopt.output_filename) {
      surf::save(img, *fileopt.output_filename, "png");
    } else {
      std::cerr << "no output file given, discarding output" << std::endl;
    }
  }
}

} // namespace

int main(int argc, char** argv)
{
  try {
    run(argc, argv);
  } catch (std::exception const& err) {
    std::cerr << "error: " << err.what() << std::endl;
  }
  return 0;
}

/* EOF */
