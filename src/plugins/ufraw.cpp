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

#include "plugins/ufraw.hpp"

#include <stdexcept>

#include <logmich/log.hpp>

#include "plugins/pnm.hpp"
#include "util/exec.hpp"
#include "util/filesystem.hpp"

namespace surf {
namespace ufraw {

bool is_available()
{
  try
  {
    std::string exe = Filesystem::find_exe("ufraw-batch");
    log_info("found {}", exe);
    return true;
  }
  catch(std::exception& err)
  {
    log_warn(err.what());
    return false;
  }
}

SoftwareSurface load_from_file(std::filesystem::path const& filename)
{
  Exec ufraw("ufraw-batch");
  ufraw
    .arg(filename)
    .arg("--silent")
    .arg("--out-type=ppm8")
    .arg("--output=-");

  if (ufraw.exec())
  {
    throw std::runtime_error("UFRaw::load_from_file(): " + std::string(ufraw.get_stderr().begin(), ufraw.get_stderr().end()));
  }
  else
  {
    return pnm::load_from_mem(ufraw.get_stdout());
  }
}

} // namespace ufraw
} // namespace surf

/* EOF */
