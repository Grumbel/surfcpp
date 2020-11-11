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
#include <stack>

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

class Context;

using ContextCommand = std::function<void(Context& ctx)>;

class Context
{
public:
  Context() : m_stack()
  {}

  void eval(ContextCommand const& cmd) {
    cmd(*this);
  }

  SoftwareSurface& top() {
    if (m_stack.empty()) {
      throw std::runtime_error("can't top(), stack empty");
    }
    return m_stack.top();
  }

  SoftwareSurface pop() {
    if (m_stack.empty()) {
      throw std::runtime_error("can't pop(), stack empty");
    }

    SoftwareSurface surface = std::move(m_stack.top());
    m_stack.pop();
    return surface;
  }

  void push(SoftwareSurface&& img) {
    m_stack.push(img);
  }

private:
  std::stack<SoftwareSurface> m_stack;
};

struct Options
{
  std::vector<ContextCommand> commands = {};
};

void print_usage(int argc, char** argv)
{
  std::cout
    << argv[0] << " ( [IMGFILE] [OPTION] )...\n"
    << "\n"
    << "General Options:\n"
    << " -h, --help   Display this text\n"
    << "\n"
    << "Commands:\n"
    << "  --output FILE        Output filename\n"
    //<< "  --output-dir DIR     Output directory\n"
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
    << "  --blit POS           Blit image\n"
    << "  --blend POS          Blend image\n"
    << "  --blend-add POS      Add image\n"
    << "  --multiply VALUE     Multiply the image by value\n"
    << "  --add VALUE          Add value to pixels\n";
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

    if (argv[i][0] == '-') {
      std::string_view opt = argv[i];
      if (opt == "--help" || opt == "-h") {
        print_usage(argc, argv);
        exit(EXIT_SUCCESS);
      } else if (opt == "--invert") {
        opts.commands.emplace_back([](Context& ctx) {
          surf::apply_invert(ctx.top());
        });
      } else if (opt == "--gamma") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        opts.commands.emplace_back([value](Context& ctx) {
          surf::apply_gamma(ctx.top(), value);
        });
      } else if (opt == "--multiply") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        opts.commands.emplace_back([value](Context& ctx) {
          surf::apply_multiply(ctx.top(), value);
        });
      } else if (opt == "--add") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        opts.commands.emplace_back([value](Context& ctx) {
          surf::apply_add(ctx.top(), value);
        });
      } else if (opt == "--brightness") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        opts.commands.emplace_back([value](Context& ctx) {
          surf::apply_brightness(ctx.top(), value);
        });
      } else if (opt == "--contrast") {
        std::string_view arg = next_arg();
        float value = std::stof(std::string(arg));
        opts.commands.emplace_back([value](Context& ctx) {
          surf::apply_contrast(ctx.top(), value);
        });
      } else if (opt == "--threshold") {
        std::string_view arg = next_arg();
        float rthreshold = 0.5f;
        float gthreshold = 0.5f;
        float bthreshold = 0.5f;
        if (sscanf(argv[i], " %f, %f, %f ", &rthreshold, &gthreshold, &bthreshold) != 3) {
          rthreshold = gthreshold = bthreshold = std::stof(std::string(arg));
        }
        opts.commands.emplace_back([rthreshold, gthreshold, bthreshold](Context& ctx) {
          surf::apply_threshold(ctx.top(), surf::Color(rthreshold, gthreshold, bthreshold));
        });
      } else if (opt == "--hsv") {
        next_arg();
        float hue = 0.0f;
        float saturation = 0.5f;
        float value = 1.0f;
        if (sscanf(argv[i], " %f, %f, %f ", &hue, &saturation, &value) != 3) {
          throw std::invalid_argument("invalid argument");
        }
        opts.commands.emplace_back([hue, saturation, value](Context& ctx) {
          surf::apply_hsv(ctx.top(), hue, saturation, value);
        });
      } else if (opt == "--grayscale") {
        opts.commands.emplace_back([](Context& ctx) {
          surf::apply_grayscale(ctx.top());
        });
      } else if (opt == "--convert") {
        std::string_view arg = next_arg();
        auto format = surf::pixelformat_from_string(arg);
        opts.commands.emplace_back([format](Context& ctx) {
          ctx.top() = surf::convert(ctx.top(), format);
        });
      } else if (opt == "--blit") {
        std::string_view pos_str = next_arg();
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        opts.commands.emplace_back([pos](Context& ctx) {
          auto img = ctx.pop();
          blit(img, ctx.top(), pos);
        });
      } else if (opt == "--blend") {
        std::string_view pos_str = next_arg();
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        opts.commands.emplace_back([pos](Context& ctx) {
          auto img = ctx.pop();
          blend(img, ctx.top(), pos);
        });
      } else if (opt == "--blend-add") {
        std::string_view pos_str = next_arg();
        geom::ipoint pos = geom::ipoint_from_string(std::string(pos_str));

        opts.commands.emplace_back([pos](Context& ctx) {
          auto img = ctx.pop();
          blend_add(img, ctx.top(), pos);
        });
      } else if (opt == "--scale") {
        std::string_view arg = next_arg();
        char op = ' ';
        if (arg.back() == '!' || arg.back() == '<' || arg.back() == '>') {
          op = arg.back();
          arg = arg.substr(0, arg.length() - 1);
        }

        geom::isize desired_size = geom::isize_from_string(std::string(arg));
        opts.commands.emplace_back([desired_size, op](Context& ctx) {
          geom::isize size = desired_size;
          if (op == '!') {
            // nop
          } else if (op == '<') {
            size = geom::grow_to_fit(ctx.top().get_size(), desired_size);
          } else if (op == '>') {
            size = geom::shrink_to_fit(ctx.top().get_size(), desired_size);
          } else {
            size = geom::resize_to_fit(ctx.top().get_size(), desired_size);
          }
          ctx.top() = surf::scale(ctx.top(), size);
        });
      } else if (opt == "--crop") {
        std::string_view arg = next_arg();
        geom::irect rect = geom::irect_from_string(std::string(arg));
        opts.commands.emplace_back([rect](Context& ctx) {
          ctx.top() = surf::crop(ctx.top(), rect);
        });
      } else if (opt == "--transform") {
        std::string_view arg = next_arg();
        surf::Transform const transf = transform_from_string(arg);
        opts.commands.emplace_back([transf](Context& ctx) {
          ctx.top() = surf::transform(ctx.top(), transf);
        });
      } else if (opt == "--fill") {
        std::string_view arg = next_arg();
        surf::Color const color = surf::Color::from_string(arg);
        opts.commands.emplace_back([color](Context& ctx) {
          surf::fill(ctx.top(), color);
        });
      } else if (opt == "--output" || opt == "-o") {
        std::filesystem::path output_filename = next_arg();
        opts.commands.emplace_back([output_filename](Context& ctx) {
          surf::save(ctx.top(), output_filename);
        });
      } else {
        print_usage(argc, argv);
        throw std::invalid_argument(fmt::format("unknown option: {}", opt));
      }
    } else { // rest argument
      std::filesystem::path input_filename = argv[i];
      opts.commands.emplace_back([input_filename](Context& ctx) {
        ctx.push(SoftwareSurface::from_file(input_filename));
      });
    }
  }

  return opts;
}

void run(int argc, char** argv)
{
  Options opts = parse_args(argc, argv);

  Context ctx;
  for (auto&& cmd : opts.commands) {
    ctx.eval(cmd);
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
