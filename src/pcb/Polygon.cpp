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

#include "Polygon.h"

#include <cbang/Exception.h>
#include <cbang/String.h>

using namespace std;
using namespace cb;
using namespace PCB;


Polygon::Polygon(const std::string &name) : Object(name) {}


void Polygon::align(double x) {
  if (getFlags(0).isLocked()) return;

  for (unsigned i = 0; i < points.size(); i++)
    points[i].align(x);
}


void Polygon::parse(Tokenizer &tokenizer) {
  Object::parse(tokenizer); // Header

  // Body
  tokenizer.match(OPAREN_TOKEN);

  while (true)
    switch (tokenizer.getType()) {
    case CPAREN_TOKEN: tokenizer.advance(); return;
    case EOF_TOKEN: THROW("Unexpected end of file in Polygon body");
    case OBRACKET_TOKEN:
    case OPAREN_TOKEN: {
      bool square = tokenizer.advance().getType() == OBRACKET_TOKEN;

      while (true) {
        if (tokenizer.getType() == EOF_TOKEN)
          THROW("Unexpected end of file in Polygon line");

        if (square && tokenizer.getType() == CBRACKET_TOKEN) break;
        if (!square && tokenizer.getType() == CPAREN_TOKEN) break;

        double x = parseReal(tokenizer, square);
        double y = parseReal(tokenizer, square);

        points.push_back(Point(x, y));
      }

      tokenizer.advance();
      break;
    }

    default:
      THROWS("Unexpected token " << tokenizer.peek() << " in Polygon body");
    }
}


void Polygon::write(std::ostream &stream, unsigned depth) const {
  Object::write(stream, depth); // Header

  // Body
  stream << '\n' << indent(depth) << '(';

  for (unsigned i = 0; i < points.size(); i++)
    // TODO output units
    stream << '\n' << indent(depth + 1)
           << '[' << formatReal(points[i].x())
           << ' ' << formatReal(points[i].y()) << ']';

  stream << '\n' << indent(depth) << ')';
}
