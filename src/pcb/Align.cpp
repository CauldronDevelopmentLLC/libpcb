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

#include "Align.h"
#include "Flags.h"

#include <cbang/Math.h>

using namespace std;
using namespace cb;
using namespace PCB;


Align::Align(double grid) : grid(grid) {
  if (!grid) THROW("Cannot align to zero");
}


void Align::align(Element &e, const std::string &key) const {
  e.setNumber(key, grid * Math::round(e.getNumber(key) / grid));
}


void Align::element(Element &e) {
  if (e.isLocked()) return;

  align(e, "mx");
  align(e, "my");
}


void Align::line(Element &e) {
  if (e.isLocked()) return;

  align(e, "x1");
  align(e, "y1");
  align(e, "x2");
  align(e, "y2");
}


void Align::point(Element &e) {
  align(e, "x");
  align(e, "y");
}


void Align::via(Element &e) {
  if (e.isLocked()) return;

  align(e, "x");
  align(e, "y");
}
