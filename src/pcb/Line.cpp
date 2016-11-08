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

using namespace cb;
using namespace PCB;


Segment2D Line::getSegment() const {
  return Segment2D(Vector2D(getNumber("x1"), getNumber("y1")),
                   Vector2D(getNumber("x2"), getNumber("y2")));
}


void Line::setSegment(const Segment2D &seg) {
  setNumber("x1", seg[0].x());
  setNumber("y1", seg[0].y());
  setNumber("x2", seg[1].x());
  setNumber("y2", seg[1].y());
}
