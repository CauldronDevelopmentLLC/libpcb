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

#ifndef PCB_VISITOR_H
#define PCB_VISITOR_H

#include "Element.h"

#include <cbang/json/Value.h>


namespace PCB {
  class Visitor {
  public:
    virtual ~Visitor() {}

    virtual void operator()(cb::JSON::Value &e);
    virtual void visit(Element &e);

    virtual void arc(Element &e) {}
    virtual void attribute(Element &e) {}
    virtual void connect(Element &e) {}
    virtual void drc(Element &e) {}
    virtual void element(Element &e) {}
    virtual void elementArc(Element &e) {}
    virtual void elementLine(Element &e) {}
    virtual void fileVersion(Element &e) {}
    virtual void flags(Element &e) {}
    virtual void grid(Element &e) {}
    virtual void groups(Element &e) {}
    virtual void layer(Element &e) {}
    virtual void line(Element &e) {}
    virtual void mark(Element &e) {}
    virtual void net(Element &e) {}
    virtual void netList(Element &e) {}
    virtual void pad(Element &e) {}
    virtual void pcb(Element &e) {}
    virtual void pin(Element &e) {}
    virtual void point(Element &e) {}
    virtual void polyArea(Element &e) {}
    virtual void polygon(Element &e) {}
    virtual void rat(Element &e) {}
    virtual void styles(Element &e) {}
    virtual void symbol(Element &e) {}
    virtual void symbolLine(Element &e) {}
    virtual void text(Element &e) {}
    virtual void thermal(Element &e) {}
    virtual void via(Element &e) {}
  };
}

#endif // E_VISITOR_H
