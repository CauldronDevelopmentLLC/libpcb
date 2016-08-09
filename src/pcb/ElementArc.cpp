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

#include "ElementArc.h"

using namespace std;
using namespace PCB;


void ElementArc::rotate(const Point &center, double angle) {
  p.rotate(center, angle);
}


void ElementArc::translate(const Point &t) {
  p += t;
}


void ElementArc::multiply(double m) {
  p.multiply(m);
  width *= m;
  height *= m;
  thickness *= m;
}


void ElementArc::round(int i) {
  p.round(i);
  Object::round(width, i);
  Object::round(height, i);
  Object::round(thickness, i);
}


void ElementArc::bounds(Point &min, Point &max) const {
  // TODO compute this
}


void ElementArc::flipX(double x) {
  p.flipX(x);
  startAngle = (startAngle + deltaAngle + 180) % 360;
}


void ElementArc::flipY(double y) {
  p.flipY(y);
  startAngle = (startAngle + deltaAngle + 180) % 360;
}


void ElementArc::lineSize(int size) {
  thickness = size;
}
