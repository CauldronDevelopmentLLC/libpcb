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

#ifndef PCB_ALIGN_H
#define PCB_ALIGN_H

#include "Visitor.h"

#include <cbang/json/Number.h>


namespace PCB {
  class Align : public Visitor {
    double grid;

  public:
    Align(double grid);

    void align(Element &e, const std::string &key) const;

    // From Visitor
    void element(Element &e);
    void line(Element &e);
    void point(Element &e);
    void via(Element &e);
  };
}

#endif // PCB_ALIGN_H
