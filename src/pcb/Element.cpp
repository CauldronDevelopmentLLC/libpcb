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

#include "Element.h"


using namespace std;
using namespace cb;
using namespace PCB;


Element::Element(JSON::Value &data) :
  data(data), name(data.getString("name")), params(*data.get("params")),
  children(data.hasList("children") ? data.get("children") : 0) {}


bool Element::isLocked(const string &name) const {
  return hasParam(name) && getFlags(name).has("lock");
}


bool Element::isFound(const string &name) const {
  return hasParam(name) && getFlags(name).has("found");
}


Flags Element::getFlags(const string &name) const {
  return Flags(*params.get(name));
}


double Element::getNumber(const string &name) const {
  return params.getNumber(name);
}


void Element::setNumber(const string &name, double x) {params.insert(name, x);}


void Element::setThermals(const string &thermal) {
  if (!getFlags().has("thermal")) return;

  JSON::Value &flag = getFlags().get("thermal");

  for (unsigned i = 0; i < flag.size(); i++)
    flag.set(i, string(1, flag.getString(i)[0]) + thermal);
}
