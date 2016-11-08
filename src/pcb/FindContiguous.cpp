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

#include "FindContiguous.h"
#include "Line.h"

using namespace std;
using namespace cb;
using namespace PCB;


void FindContiguous::layer(Element &e) {
  vector<SmartPointer<Line> > lines;
  vector<Segment2D> segs;
  JSON::Value &children = e.getChildren();

  for (unsigned i = 0; i < children.size(); i++) {
    JSON::Value &child = *children.get(i);

    if (child.isDict() && child.getString("name", "") == "Line") {
      SmartPointer<Line> line= new Line(child);
      lines.push_back(line);
      segs.push_back(line->getSegment());
    }
  }

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
          if (lines[i]->getFlags().has("clearline") ==
              lines[j]->getFlags().has("clearline")) {
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
            if (!lines[i]->getFlags().has("found")) {
              lines[i]->setSegment(seg);
              lines[j]->getFlags().set("found");

            } else if (!lines[j]->getFlags().has("found")) {
              lines[j]->setSegment(seg);
              lines[i]->getFlags().set("found");
            }
          }
        }
      }
    }
  }
}
