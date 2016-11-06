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

#include <map>
#include <string>


namespace PCB {
  class Flags {
    typedef std::map<std::string, std::string> flags_t;
    flags_t flags;

  public:
    Flags(const std::string &flags);

    std::string toString() const;

    void set(const std::string &flag, const std::string &value);
    std::string get(const std::string &flag) const;
    void clear(const std::string &flag);
    bool has(const std::string &flag) const;
    bool isLocked() const {return has("lock");}
  };
}

#endif // PCB_FLAGS_H
