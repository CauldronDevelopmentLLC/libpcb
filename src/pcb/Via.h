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

#ifndef PCB_VIA_H
#define PCB_VIA_H

#include "Object.h"
#include "Point.h"

namespace PCB {
  class Via : public Object {
    Point p;
    int thickness;
    int clearance;
    int mask;
    int drill;
    std::string name;
    std::string flags;

  public:
    Via();
    Via(int drill, int pad, int clearance, int mask);

    // From Object
    void rotate(const Point &center, double angle);
    void translate(const Point &t);
    void multiply(double m);
    void round(int i);
    void bounds(Point &min, Point &max) const;
    void flipX(double x);
    void flipY(double x);
  };
}

#endif // PCB_VIA_H
