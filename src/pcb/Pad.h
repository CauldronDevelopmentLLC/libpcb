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

#ifndef PCB_PAD_H
#define PCB_PAD_H

#include "Object.h"
#include "Point.h"

namespace PCB {
  class Pad : public Object {
    Point p1;
    Point p2;
    int thickness;
    int clearance;
    int mask;
    std::string name;
    std::string number;
    std::string flags;

  public:
    Pad();
    Pad(unsigned int length, unsigned int width, int clearance, int mask);

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

#endif // PCB_PAD_H
