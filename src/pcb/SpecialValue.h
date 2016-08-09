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

#ifndef PCB_SPECIAL_VALUE_H
#define PCB_SPECIAL_VALUE_H

#include "Object.h"


namespace PCB {
  class SpecialValue : public Object {
    double value;

  public:
    SpecialValue(const std::string &name) : Object(name) {}

    double getValue() const {return value;}

    // From Object
    void parse(Tokenizer &tokenizer);
    void write(std::ostream &stream, unsigned depth = 0) const;
  };
}

#endif // PCB_SPECIAL_VALUE_H
