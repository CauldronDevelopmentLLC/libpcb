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

#ifndef PCB_POLYGON_H
#define PCB_POLYGON_H

#include "Object.h"
#include "Point.h"

#include <vector>


namespace PCB {
  class Polygon : public Object {
    std::vector<Point> points;
    std::string flags;

  public:
    Polygon(const std::string &name = "Polygon");

    // From Object
    void align(double i);
    void parse(Tokenizer &tokenizer);
    void write(std::ostream &stream, unsigned depth) const;
  };
}

#endif // PCB_POLYGON_H
