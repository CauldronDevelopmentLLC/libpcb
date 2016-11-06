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

#include "Line.h"

#include <math.h>

using namespace std;
using namespace cb;
using namespace PCB;


Segment2D Line::toSegment() const {
  return Segment2D(Vector2D(getDouble(0), getDouble(1)),
                   Vector2D(getDouble(2), getDouble(3)));
}


void Line::setSegment(const Segment2D &seg) {
  setDouble(0, seg[0].x());
  setDouble(1, seg[0].y());
  setDouble(2, seg[1].x());
  setDouble(3, seg[1].y());
}


void Line::align(double i) {
  if (getFlags(6).isLocked()) return;
  Object::align(0, i);
  Object::align(1, i);
  Object::align(2, i);
  Object::align(3, i);
}


void Line::findAskew() {
  double x1 = getDouble(0);
  double y1 = getDouble(1);
  double x2 = getDouble(2);
  double y2 = getDouble(3);

  if (x1 != x2 && y1 != y2 && 0.00001 < fabs(fabs(x1 - x2) - fabs(y1 - y2)))
    setFlag(6, "found");
}


void Line::findShort(double length) {
  if (toSegment().length() <= length) setFlag(6, "found");
}
