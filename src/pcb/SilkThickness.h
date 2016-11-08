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

#ifndef PCB_SILK_THICKNESS_H
#define PCB_SILK_THICKNESS_H

#include "Visitor.h"


namespace PCB {
  class SilkThickness : public Visitor {
    double thickness;

  public:
    SilkThickness(double thickness) : thickness(thickness) {}

    void setThickness(Element &e);

    // From Visitor
    void elementArc(Element &e) {setThickness(e);}
    void elementLine(Element &e) {setThickness(e);}
  };
}

#endif // PCB_SILK_THICKNESS_H
