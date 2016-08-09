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

using namespace std;
using namespace PCB;


void Line::rotate(const Point &center, double angle) {
  p1.rotate(center, angle);
  p2.rotate(center, angle);
}


void Line::translate(const Point &t) {
  p1 += t;
  p2 += t;
}


void Line::multiply(double m) {
  p1.multiply(m);
  p2.multiply(m);
  thickness *= m;
  clearance *= m;
}


void Line::round(int i) {
  p1.round(i);
  p2.round(i);
  Object::round(thickness, i);
  Object::round(clearance, i);
}


void Line::bounds(Point &min, Point &max) const {
  p1.bounds(min, max);
  p2.bounds(min, max);
}


void Line::flipX(double x) {
  p1.flipX(x);
  p2.flipX(x);
}


void Line::flipY(double y) {
  p1.flipY(y);
  p2.flipY(y);
}
