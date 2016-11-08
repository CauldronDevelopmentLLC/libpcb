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

#include "FindAskew.h"

#include <math.h>

using namespace PCB;


void FindAskew::line(Element &e) {
  double x1 = e.getNumber("x1");
  double y1 = e.getNumber("y1");
  double x2 = e.getNumber("x2");
  double y2 = e.getNumber("y2");

  if (x1 != x2 && y1 != y2 && 0.00001 < fabs(fabs(x1 - x2) - fabs(y1 - y2)))
    e.getFlags().set("found");
}
