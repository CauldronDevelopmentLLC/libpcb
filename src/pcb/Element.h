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

#ifndef PCB_ELEMENT_H
#define PCB_ELEMENT_H

#include "Flags.h"

#include <cbang/json/Value.h>


namespace PCB {
  class Element {
    cb::JSON::Value &data;
    const std::string name;
    cb::JSON::Value &params;
    cb::JSON::ValuePtr children;

  public:
    Element(cb::JSON::Value &data);

    cb::JSON::Value &getData() const {return data;}
    const std::string &getName() const {return name;}

    bool hasParam(const std::string &name) const {return params.has(name);}

    bool isLocked(const std::string &name = "flags") const;
    bool isFound(const std::string &name = "flags") const;

    Flags getFlags(const std::string &name = "flags") const;

    double getNumber(const std::string &name) const;
    void setNumber(const std::string &name, double x);

    void setThermals(const std::string &thermal);

    bool hasChildren() const {return !children.isNull();}
    cb::JSON::Value &getChildren() {return *children;}

    void remove() {data.insertBoolean("remove", true);}
  };
}

#endif // PCB_ELEMENT_H
