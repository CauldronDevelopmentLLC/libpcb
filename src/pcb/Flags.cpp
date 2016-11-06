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

#include "Flags.h"

#include <cbang/Exception.h>
#include <cbang/String.h>


using namespace PCB;
using namespace cb;
using namespace std;


Flags::Flags(const string &flags) {
  string name;
  string value;
  bool inValue = false;

  for (unsigned i = 0; i < flags.size(); i++)
    switch (flags[i]) {
    case '(': inValue = true; break;
    case ')': inValue = false; break;

    case ',':
      if (!inValue) {
        if (name.empty()) THROWS("Empty flag in flags '" << flags << "'");
        set(name, value);
        name.clear();
        value.clear();
        break;
      }

    default:
      if (inValue) value.append(1, flags[i]);
      else name.append(1, flags[i]);
      break;
    }

  if (inValue) THROWS("Unterminated value in flags '" << flags << "'");
  if (!name.empty()) set(name, value);
}


string Flags::toString() const {
  string s;

  for (flags_t::const_iterator it = flags.begin(); it != flags.end(); it++) {
    if (it != flags.begin()) s += ",";
    s += it->first;
    if (!it->second.empty()) s += "(" + it->second + ")";
  }

  return s;
}


void Flags::set(const string &flag, const string &value) {
  pair<flags_t::iterator, bool> result =
    flags.insert(flags_t::value_type(flag, value));

  if (!result.second) result.first->second = value;
}


string Flags::get(const string &flag) const {
  flags_t::const_iterator it = flags.find(flag);
  if (it == flags.end()) THROWS("Flag '" << flag << "' not set");

  return it->second;
}


void Flags::clear(const string &flag) {flags.erase(flag);}


bool Flags::has(const string &flag) const {
  return flags.find(flag) != flags.end();
}
