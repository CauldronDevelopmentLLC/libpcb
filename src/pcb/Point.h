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

#ifndef PCB_POINT_H
#define PCB_POINT_H

#include "Object.h"

#include <cbang/geom/Vector.h>
#include <cbang/Math.h>


namespace PCB {
  struct Point : public cb::Vector2D {
    Point(double x = 0, double y = 0) : cb::Vector2D(x, y) {validate();}
    Point(const cb::Vector2D &p) : cb::Vector2D(p) {validate();}


    void align(double grid) {
      x() = Object::alignValue(x(), grid);
      y() = Object::alignValue(y(), grid);
    }


    void validate() const {
      if (!isReal()) THROWS("Invalid Point(" << x() << ", " << y() << ")");
    }
  };
}

#endif // PCB_POINT_H
