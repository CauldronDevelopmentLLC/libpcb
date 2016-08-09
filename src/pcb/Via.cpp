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

#include "Via.h"

#include <algorithm>

using namespace std;
using namespace PCB;


Via::Via() : Object("Via"), thickness(0), clearance(0), mask(0), drill(0) {}


Via::Via(int drill, int pad, int clearance, int mask) :
  Object("Via"), thickness(drill + pad), clearance(clearance),
  mask(drill + pad + mask), drill(drill) {
  if (pad == 0) flags = "hole";
}


void Via::rotate(const Point &center, double angle) {
  p.rotate(center, angle);
}


void Via::translate(const Point &t) {
  p += t;
}


void Via::multiply(double m) {
  p.multiply(m);
  thickness *= m;
  clearance *= m;
  mask *= m;
  drill *= m;
}


void Via::round(int i) {
  p.round(i);
  Object::round(thickness, i);
  Object::round(clearance, i);
  Object::round(mask, i);
  Object::round(drill, i);
}


void Via::bounds(Point &min, Point &max) const {
  int x = std::max(thickness, std::max(mask, drill)) / 2;
  Point offset(x, x);

  Point(p - offset).bounds(min, max);
  Point(p + offset).bounds(min, max);
}


void Via::flipX(double x) {
  p.flipX(x);
}


void Via::flipY(double y) {
  p.flipY(y);
}
