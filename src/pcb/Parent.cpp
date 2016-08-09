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

#include "Parent.h"

#include "PCB.h"

#include <cbang/Exception.h>

using namespace std;
using namespace cb;
using namespace PCB;


void Parent::findChildren(const string &name,
                          vector<SmartPointer<Object> > &children) const {
  for (const_iterator it = begin(); it != end(); it++)
    if ((*it)->getName() == name) children.push_back(*it);
}


void Parent::centerMark() {
  for (iterator it = begin(); it != end(); it++)
    if (it->isInstance<Parent>()) it->cast<Parent>()->centerMark();
}


void Parent::subtractMask(int clear) {
  for (iterator it = begin(); it != end(); it++)
    if (it->isInstance<Parent>()) it->cast<Parent>()->subtractMask(clear);
}


Point Parent::findCenter() {
  Point min(numeric_limits<int>::max(), numeric_limits<int>::max());
  Point max(-numeric_limits<int>::max(), -numeric_limits<int>::max());
  bounds(min, max);
  return (max - min) / 2 + min;
}


void Parent::center() {
  translate(-findCenter());
}


void Parent::merge(const Parent &o) {
  for (const_iterator it = o.begin(); it != o.end(); it++)
    add(*it);
}


void Parent::rotate(const Point &center, double angle) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->rotate(center, angle);
}


void Parent::translate(const Point &t) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->translate(t);
}


void Parent::multiply(double m) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->multiply(m);
}


void Parent::round(int x) {
  for (const_iterator it = begin(); it != end(); it++)
    (*it)->round(x);
}


void Parent::bounds(Point &min, Point &max) const {
  for (const_iterator it = begin(); it != end(); it++)
    (*it)->bounds(min, max);
}


void Parent::flipX(double x) {
  for (const_iterator it = children.begin(); it != children.end(); it++)
    (*it)->flipX(x);
}


void Parent::flipY(double y) {
  for (const_iterator it = children.begin(); it != children.end(); it++)
    (*it)->flipY(y);
}


void Parent::lineSize(int size) {
  for (const_iterator it = children.begin(); it != children.end(); it++)
    (*it)->lineSize(size);
}


void Parent::textScale(int scale) {
  for (const_iterator it = children.begin(); it != children.end(); it++)
    (*it)->textScale(scale);
}


void Parent::parse(Tokenizer &tokenizer) {
  // Header
  Object::parse(tokenizer);

  // Body
  if (tokenizer.getType() == OPAREN_TOKEN) {
    tokenizer.advance();
    parseBody(tokenizer);
    tokenizer.match(CPAREN_TOKEN);
  }
}


void Parent::write(ostream &stream, unsigned depth) const {
  Object::write(stream, depth);

  stream << '\n' << indent(depth) << '(';

  for (const_iterator it = begin(); it != end(); it++) {
    stream << '\n' << indent(depth + 1);
    (*it)->write(stream, depth + 1);
  }

  stream << '\n' << indent(depth) << ')';
}


void Parent::parseBody(Tokenizer &tokenizer) {
  while (true)
    switch (tokenizer.getType()) {
    case EOF_TOKEN: case CPAREN_TOKEN: return;
    case COMMENT_TOKEN: tokenizer.advance(); continue; // Skip for now

    case ID_TOKEN: {
      string id = tokenizer.getValue();
      SmartPointer<Object> o;

      if (id == "Arc") o = new Arc;
      else if (id == "Element") o = new Element;
      else if (id == "ElementArc") o = new ElementArc;
      else if (id == "ElementLine") o = new ElementLine;
      else if (id == "Layout") o = new Layout;
      else if (id == "Layer") o = new Layer;
      else if (id == "NetList") o = new NetList;
      else if (id == "Symbol") o = new Symbol;
      else if (id == "Line") o = new Line;
      else if (id == "Pad") o = new Pad;
      else if (id == "Polygon") o = new Polygon;
      else if (id == "Hole") o = new Hole;
      else if (id == "Text") o = new Text;
      else if (id == "Via") o = new Via;
      else if (id == "Thermal") o = new Thermal;
      else if (id == "PolyArea") o = new PolyArea;

      if (o.isNull()) {
        o = new Object(id);
        o->parse(tokenizer);

        if (tokenizer.getType() == OPAREN_TOKEN) {
          SmartPointer<Parent> p = new Parent(*o); // Upgrade
          tokenizer.advance();
          p->parseBody(tokenizer);
          tokenizer.match(CPAREN_TOKEN);
          o = p;
        }

      } else o->parse(tokenizer);

      add(o);

      break;
    }

    default:
      THROWS("Unexpected token " << tokenizer.peek() << " in " << getName()
             << " body");
    }
}
