/******************************************************************************\

    libpcb is an Open-Source library for parsing Gnu EDA PCB files
    Copyright (C) 2011-2015 Joseph Coffland <joseph@cauldrondevelopment.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#ifndef PCB_FLAGS_H
#define PCB_FLAGS_H

#include <cbang/json/Value.h>


namespace PCB {
  class Flags {
    cb::JSON::Value &data;

  public:
    Flags(cb::JSON::Value &data) : data(data) {}

    void set(const std::string &name);
    cb::JSON::Value &get(const std::string &name) const;
    void clear(const std::string &name);
    bool has(const std::string &name) const;
  };
}

#endif // PCB_FLAGS_H
