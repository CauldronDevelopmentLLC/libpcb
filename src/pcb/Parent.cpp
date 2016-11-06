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

#include "SpecialValue.h"
#include "Polygon.h"
#include "Via.h"
#include "Pin.h"
#include "Element.h"
#include "Line.h"
#include "ElementLine.h"
#include "ElementArc.h"
#include "Text.h"

#include <cbang/Exception.h>

#include <math.h>

using namespace std;
using namespace cb;
using namespace PCB;


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


void Parent::align(double x) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->align(x);
}


void Parent::setViaThermals(const string &viaThermals) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->setViaThermals(viaThermals);
}


void Parent::setPinThermals(const string &pinThermals) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->setPinThermals(pinThermals);
}


void Parent::setSilkThickness(double thickness) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->setSilkThickness(thickness);
}


void Parent::setTextScale(unsigned scale) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->setTextScale(scale);
}


void Parent::findAskew() {
  for (iterator it = begin(); it != end(); it++)
    (*it)->findAskew();
}


void Parent::findShort(double length) {
  for (iterator it = begin(); it != end(); it++)
    (*it)->findShort(length);
}


void Parent::findContiguous() {
  vector<SmartPointer<Object> > lines;
  vector<Segment2D> segs;

  for (iterator it = begin(); it != end(); it++)
    if ((*it)->getName() == "Line") {
      lines.push_back(*it);
      segs.push_back(it->cast<Line>()->toSegment());

    } else (*it)->findContiguous();

  for (unsigned i = 0; i < lines.size(); i++) {
    Segment2D &segA = segs[i];

    for (unsigned j = i + 1; j < lines.size(); j++) {
      Segment2D &segB = segs[j];

      if (segA.distance(segB[0]) < 0.00001 ||
          segA.distance(segB[1]) < 0.00001) {

        // Lines are connected, now check if they have the same slope
        double rise1 = segA[0].x() - segA[1].x();
        double run1 = segA[0].y() - segA[1].y();
        double rise2 = segB[0].x() - segB[1].x();
        double run2 = segB[0].y() - segB[1].y();

        double slope1 = rise1 / run1;
        double slope2 = rise2 / run2;

        if ((!run1 && !run2) ||
            ((run1 && run2) && fabs(slope1 - slope2) < 0.00001)) {

          // Check that both lines have the same clear setting
          if (lines[i]->hasFlag(6, "clearline") ==
              lines[j]->hasFlag(6, "clearline")) {
            // Combine lines
            Segment2D seg;
            for (int k = 0; k < 2; k++)
              if (segA[0][k] < segA[1][k]) {
                seg[0][k] = min(segA[0][k], min(segB[0][k], segB[1][k]));
                seg[1][k] = max(segA[1][k], max(segB[0][k], segB[1][k]));

              } else {
                seg[0][k] = max(segA[0][k], max(segB[0][k], segB[1][k]));
                seg[1][k] = min(segA[1][k], min(segB[0][k], segB[1][k]));
              }

            // Make one line the combined line and flag the other for removal
            if (!lines[i]->hasFlag(6, "found")) {
              lines[i].cast<Line>()->setSegment(seg);
              lines[j]->setFlag(6, "found");

            } else if (!lines[j]->hasFlag(6, "found")) {
              lines[j].cast<Line>()->setSegment(seg);
              lines[i]->setFlag(6, "found");
            }
          }
        }
      }
    }
  }
}


void Parent::clearFound() {
  for (iterator it = begin(); it != end(); it++)
    (*it)->clearFound();
}


void Parent::removeFound() {
  for (iterator it = begin(); it != end();)
    if ((*it)->isFound()) it = children.erase(it);
    else {
      (*it)->removeFound();
      it++;
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

      if (id == "Polygon") o = new Polygon;
      else if (id == "Via") o = new Via;
      else if (id == "Pin") o = new Pin;
      else if (id == "Element") o = new Element;
      else if (id == "Line") o = new Line;
      else if (id == "ElementLine") o = new ElementLine;
      else if (id == "ElementArc") o = new ElementArc;
      else if (id == "Text") o = new Text;
      else if (id == "Thermal") o = new SpecialValue("Thermal");
      else if (id == "PolyArea") o = new SpecialValue("PolyArea");

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
