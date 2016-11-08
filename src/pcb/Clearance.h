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

#ifndef PCB_CLEARANCE_H
#define PCB_CLEARANCE_H

#include "Visitor.h"


namespace PCB {
  class Clearance : public Visitor {
    double clearance;

  public:
    Clearance(double clearance) : clearance(clearance) {}

    void setClearance(Element &e);

    // From Visitor
    void line(Element &e) {setClearance(e);}
    void pad(Element &e) {setClearance(e);}
    void pin(Element &e) {setClearance(e);}
    void via(Element &e) {setClearance(e);}
  };
}

#endif // PCB_CLEARANCE_H
