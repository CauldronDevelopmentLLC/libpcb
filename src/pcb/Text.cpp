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

#include "Text.h"

using namespace std;
using namespace PCB;


Text::Text() : Object("Text"), direction(0), scale(0) {}


Text::Text(int direction, int scale, const string &text) :
  Object("Text"), direction(direction), scale(0), text(text) {
}


void Text::rotate(const Point &center, double angle) {
  p.rotate(center, angle);
}


void Text::translate(const Point &t) {
  p += t;
}


void Text::multiply(double m) {
  p.multiply(m);
}


void Text::round(int i) {
  p.round(i);
}


void Text::bounds(Point &min, Point &max) const {
  // TODO do better than this
  p.bounds(min, max);
}

void Text::flipX(double x) {
  p.flipX(x);
}


void Text::flipY(double y) {
  p.flipY(y);
}


void Text::textScale(int scale) {
  this->scale = scale;
}
