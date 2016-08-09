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

#include "Element.h"

#include "Pad.h"
#include "ElementLine.h"

#include <cbang/Exception.h>

#include <cbang/geom/Vector.h>
#include <cbang/geom/Rectangle.h>

#include <limits>

using namespace std;
using namespace cb;
using namespace PCB;


void Element::rotate(const Point &center, double angle) {
  m.rotate(center, angle);
  t.rotate(center, angle);
}


void Element::translate(const Point &t) {
  m.translate(t);
}


void Element::multiply(double m) {
  this->m.multiply(m);
  t.multiply(m);
  Parent::multiply(m);
}


void Element::round(int x) {
  m.round(x);
  t.round(x);
  Parent::round(x);
}


void Element::bounds(Point &min, Point &max) const {
  Point localMin(numeric_limits<int>::max(), numeric_limits<int>::max());
  Point localMax(-numeric_limits<int>::max(), -numeric_limits<int>::max());
  Parent::bounds(localMin, localMax);

  localMin += m;
  localMax += m;

  localMin.bounds(min, max);
  localMax.bounds(min, max);
}


void Element::centerMark() {
  Point center = findCenter() - m;
  Parent::translate(-center);
}


void Element::subtractMask(int clear) {
  // This is incomplete but sufficient for now.

  bool changed = false;

  for (iterator it = begin(); it != end();) {
    if (it->isInstance<Pad>()) {
      SmartPointer<Pad> p = it->cast<Pad>();

      Point p1(numeric_limits<int>::max(), numeric_limits<int>::max());
      Point p2(-numeric_limits<int>::max(), -numeric_limits<int>::max());
      p->bounds(p1, p2);

      for (iterator it2 = begin(); it2 != end();) {
        if (it2->isInstance<ElementLine>()) {
          SmartPointer<ElementLine> l = it2->cast<ElementLine>();

          int x = l->getThickness() / 2 + clear;
          Segment2D s = l->getSegment();
          Rectangle2D box(p1, p2);
          Point offset(x, x);
          box.add(Vector2D(p1 - offset));
          box.add(Vector2D(p1 + offset));
          box.add(Vector2D(p2 - offset));
          box.add(Vector2D(p2 + offset));

          bool contains1 = box.contains(s[0]);
          bool contains2 = box.contains(s[1]);

          if (contains1 && contains2) {
            it2 = children.erase(it2);
            changed = true;
            continue;
          }

          Point p1, p2;
          if (box.intersects(s, p1, p2)) {
            int thickness = l->getThickness();
            it2 = children.erase(it2);

            if (contains1)
              it2 =
                children.insert(it2, new ElementLine(p1, s[1], thickness));

            else if (contains2)
              it2 = children.insert(it2, new ElementLine(p1, s[0], thickness));

            else {
              if (s[0].distance(p1) < s[0].distance(p2)) {
                it2 =
                  children.insert(it2, new ElementLine(p1, s[0], thickness));
                it2++;
                it2 =
                  children.insert(it2, new ElementLine(p2, s[1], thickness));

              } else {
                it2 =
                  children.insert(it2, new ElementLine(p1, s[1], thickness));
                it2++;
                it2 =
                  children.insert(it2, new ElementLine(p2, s[0], thickness));
              }

            }

            changed = true;
          }
        }

        it2++;
      }
    }

    it++;
    if (it == children.end() && changed) {
      changed = false;
      it = children.begin();
    }
  }
}


void Element::flipX(double x) {
  m.flipX(x);
  t.flipX(0);
  Parent::flipX(0);
}


void Element::flipY(double y) {
  m.flipY(y);
  t.flipY(0);
  Parent::flipY(0);
}


void Element::textScale(int scale) {
  tscale = scale;
  Parent::textScale(scale);
}
