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

#include "Visitor.h"

using namespace std;
using namespace cb;
using namespace PCB;


void Visitor::operator()(JSON::Value &e) {
  for (unsigned i = 0; i < e.size(); i++) {
    JSON::Value &child = *e.get(i);

    if (child.isDict() && child.hasString("name")) {
      Element e(child);
      visit(e);

      if (e.hasChildren()) (*this)(e.getChildren());
    }
  }
}


void Visitor::visit(Element &e) {
  const string &name = e.getName();

  if (name == "Arc") arc(e);
  else if (name == "Attribute") attribute(e);
  else if (name == "Connect") connect(e);
  else if (name == "DRC") drc(e);
  else if (name == "Element") element(e);
  else if (name == "ElementArc") elementArc(e);
  else if (name == "ElementLine") elementLine(e);
  else if (name == "FileVersion") fileVersion(e);
  else if (name == "Flags") flags(e);
  else if (name == "Grid") grid(e);
  else if (name == "Groups") groups(e);
  else if (name == "Layer") layer(e);
  else if (name == "Line") line(e);
  else if (name == "Mark") mark(e);
  else if (name == "Net") net(e);
  else if (name == "NetList") netList(e);
  else if (name == "Pad") pad(e);
  else if (name == "PCB") pcb(e);
  else if (name == "Pin") pin(e);
  else if (name == "Point") point(e);
  else if (name == "PolyArea") polyArea(e);
  else if (name == "Polygon") polygon(e);
  else if (name == "Rat") rat(e);
  else if (name == "Styles") styles(e);
  else if (name == "Symbol") symbol(e);
  else if (name == "SymbolLine") symbolLine(e);
  else if (name == "Text") text(e);
  else if (name == "Thermal") thermal(e);
  else if (name == "Via") via(e);
}
