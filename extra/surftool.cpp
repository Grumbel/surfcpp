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

#include <geom/io.hpp>
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
  std::cout
    << argv[0] << " ( [IMGFILE] [OPTION] )...\n"
    << "\n"
    << "General Options:\n"
    << " -h, --help   Display this text\n"
    << "\n"
    << "Per Image Options:\n"
    << "  --output FILE        Output filename\n"
    << "  --output-dir DIR     Output directory\n"
    << "\n"
    << "Filter:\n"
    << "  --invert             Invert the image\n"
    << "  --gamma VALUE        Apply gamma correction\n"
    << "  --brightness VALUE   Adjust brightness\n"
    << "  --contrast VALUE     Adjust contrast\n"
    << "  --scale WxH{!><}     Resize the image\n"
    << "  --crop WxH[+X+Y]     Crop the image\n"
    << "  --transform ROT      Rotate or flip the image\n"
    << "  --threshold VALUE    Apply the given threshold\n"
    << "  --grayscale          Convert to grayscale\n"
    << "  --hsv H:S:V          Apply hue/saturation/value\n"
    << "  --convert FORMAT     Convert internal format to FORMAT\n"
    << "  --blit FILE POS      Blit image\n"
    << "  --blend FILE POS     Blend image\n"
    << "  --add FILE POS       Add image\n";
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  if (argc == 1) {
    print_usage(argc, argv);
    exit(EXIT_SUCCESS);
  }

  for (int i = 1; i < argc; ++i) {
    auto next_arg = [&]{
      if (++i >= argc) {
        std::ostringstream msg;
        msg << argv[i - 1] << " needs an argument";
        throw std::runtime_error(msg.str());
      }
      return std::string_view(argv[i]);
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
      std::string_view opt = argv[i];
      if (opt == "--help" || opt == "-h") {
        print_usage(argc, argv);
        exit(EXIT_SUCCESS);
      } else if (opt == "--invert") {
        file_opts().filters.emplace_back([](SoftwareSurface& sur) {
          surf::apply_invert(sur);
        });
      } else if (opt == "--gamma") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        file_opts().filters.emplace_back([value](SoftwareSurface& sur) {
          surf::apply_gamma(sur, value);
        });
      } else if (opt == "--brightness") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        file_opts().filters.emplace_back([value](SoftwareSurface& sur) {
          surf::apply_brightness(sur, value);
        });
      } else if (opt == "--contrast") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        file_opts().filters.emplace_back([value](SoftwareSurface& sur) {
          surf::apply_contrast(sur, value);
        });
      } else if (opt == "--threshold") {
        std::string_view arg = next_arg();
        float rthreshold = 0.5f;
        float gthreshold = 0.5f;
        float bthreshold = 0.5f;
        if (sscanf(argv[i], " %f, %f, %f ", &rthreshold, &gthreshold, &bthreshold) != 3) {
          rthreshold = gthreshold = bthreshold = std::stof(std::string(arg));
        }
        file_opts().filters.emplace_back([rthreshold, gthreshold, bthreshold](SoftwareSurface& sur) {
          surf::apply_threshold(sur, surf::Color(rthreshold, gthreshold, bthreshold));
        });
      } else if (opt == "--hsv") {
        next_arg();
        float hue = 0.0f;
        float saturation = 0.5f;
        float value = 1.0f;
        if (sscanf(argv[i], " %f, %f, %f ", &hue, &saturation, &value) != 3) {
          throw std::invalid_argument("invalid argument");
        }
        file_opts().filters.emplace_back([hue, saturation, value](SoftwareSurface& sur) {
          surf::apply_hsv(sur, hue, saturation, value);
        });
      } else if (opt == "--grayscale") {
        file_opts().filters.emplace_back([](SoftwareSurface& sur) {
          surf::apply_grayscale(sur);
        });
      } else if (opt == "--convert") {
        std::string_view arg = next_arg();
        auto format = surf::pixelformat_from_string(arg);
        file_opts().filters.emplace_back([format](SoftwareSurface& sur) {
          sur = surf::convert(sur, format);
        });
      } else if (opt == "--blit") {
        std::string_view filename_str = next_arg();
        std::string_view pos_str = next_arg();

        auto img = SoftwareSurface::from_file(filename_str);
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        file_opts().filters.emplace_back([img{std::move(img)}, pos](SoftwareSurface& sur) {
          blit(img, sur, pos);
        });
      } else if (opt == "--blend") {
        std::string_view filename_str = next_arg();
        std::string_view pos_str = next_arg();

        auto img = SoftwareSurface::from_file(filename_str);
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        file_opts().filters.emplace_back([img{std::move(img)}, pos](SoftwareSurface& sur) {
          blend(img, sur, pos);
        });
      } else if (opt == "--add") {
        std::string_view filename_str = next_arg();
        std::string_view pos_str = next_arg();

        auto img = SoftwareSurface::from_file(filename_str);
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        file_opts().filters.emplace_back([img{std::move(img)}, pos](SoftwareSurface& sur) {
          blend_add(img, sur, pos);
        });
      } else if (opt == "--scale") {
        std::string_view arg = next_arg();
        char op = ' ';
        if (arg.back() == '!' || arg.back() == '<' || arg.back() == '>') {
          op = arg.back();
          arg = arg.substr(0, arg.length() - 1);
        }

        geom::isize desired_size = geom::isize_from_string(std::string(arg));
        file_opts().filters.emplace_back([desired_size, op](SoftwareSurface& sur) {
          geom::isize size = desired_size;
          if (op == '!') {
            // nop
          } else if (op == '<') {
            size = geom::grow_to_fit(sur.get_size(), desired_size);
          } else if (op == '>') {
            size = geom::shrink_to_fit(sur.get_size(), desired_size);
          } else {
            size = geom::resize_to_fit(sur.get_size(), desired_size);
          }
          sur = surf::scale(sur, size);
        });
      } else if (opt == "--crop") {
        std::string_view arg = next_arg();
        geom::irect rect = geom::irect_from_string(std::string(arg));
        file_opts().filters.emplace_back([rect](SoftwareSurface& sur) {
          sur = surf::crop(sur, rect);
        });
      } else if (opt == "--transform") {
        std::string_view arg = next_arg();
        surf::Transform const transf = transform_from_string(arg);
        file_opts().filters.emplace_back([transf](SoftwareSurface& sur) {
          sur = surf::transform(sur, transf);
        });
      } else if (opt == "--fill") {
        std::string_view arg = next_arg();
        surf::Color const color = surf::Color::from_string(arg);
        file_opts().filters.emplace_back([color](SoftwareSurface& sur) {
          surf::fill(sur, color);
        });
      } else if (opt == "--output" || opt == "-o") {
        std::string_view arg = next_arg();
        file_opts().output_filename = arg;

      } else if (opt == "--output-dir" || opt == "-O") {
        std::string_view arg = next_arg();
        std::filesystem::path dir = arg;
        file_opts().output_filename = dir / file_opts().input_filename.filename();
      } else {
        print_usage(argc, argv);
        throw std::invalid_argument(fmt::format("unknown option: {}", opt));
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
      surf::save(img, *fileopt.output_filename);
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
    exit(EXIT_FAILURE);
  }
  return 0;
}

/* EOF */
