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

#include "Object.h"

#include <cbang/Exception.h>
#include <cbang/Math.h>
#include <cbang/parse/Scanner.h>

using namespace std;
using namespace cb;
using namespace PCB;




void Object::round(double &v, int i) {
  v = i * Math::round(v / i);
}


void Object::round(int &v, int i) {
  v = i * (int)Math::round((double)v / i);
}


void Object::parse(Tokenizer &tokenizer) {
  tokenizer.match(ID_TOKEN);

  if (tokenizer.getType() != OBRACKET_TOKEN &&
      tokenizer.getType() != OPAREN_TOKEN)
    THROW("Expected '[' or '('");

  square = tokenizer.advance().getType() == OBRACKET_TOKEN;

  while (true) {
    TokenType type = tokenizer.getType();

    if (square && type == CBRACKET_TOKEN) break;
    if (!square && type == CPAREN_TOKEN) break;

    switch (type) {
    case EOF_TOKEN: THROW("Unexpected end of file");

    case REAL_TOKEN: {
      double x = parseReal(tokenizer, square);
      header.push_back(Variant(x));
      break;
    }

    case INTEGER_TOKEN: {
      int64_t x = String::parseS64(tokenizer.advance().getValue());
      header.push_back(Variant(x));
      break;
    }

    case STRING_TOKEN:
      header.push_back(tokenizer.advance().getValue());
      break;

    case CHAR_TOKEN:
      header.push_back((uint8_t)tokenizer.advance().getValue()[0]);
      break;

    default:
      THROWS("Unexpected token " << tokenizer.peek() << " in " << getName()
             << " header");
    }
  }

  tokenizer.advance();

  init();
}


void Object::write(ostream &stream, unsigned depth) const {
  stream << name << (square ? '[' : '(');

  for (unsigned i = 0; i < header.size(); i++) {
    if (i) stream << ' ';

    switch (header[i].getType()) {
    case Variant::BYTE_TYPE:
      stream << '\'' << (char)header[i].toByte() << '\'';
      break;

    case Variant::STRING_TYPE:
      stream << '"' << header[i].toString() << '"';
      break;

    case Variant::INTEGER_TYPE:
      stream << header[i].toInteger();
      break;

    case Variant::REAL_TYPE:
      stream << formatReal(header[i].toReal());
      break;

    default: THROWS("Unexpected type " << header[i].getType() << " in header");
    }
  }

  stream << (square ? ']' : ')');
}


void Object::read(istream &stream) {
  Scanner scanner(stream);
  PCB::Tokenizer tokenizer(scanner);
  return parse(tokenizer);
}


double Object::parseReal(Tokenizer &tokenizer, bool newFormat) {
  double x = String::parseDouble(tokenizer.match(REAL_TOKEN).getValue());

  if (tokenizer.getType() == ID_TOKEN) {
    string unit = String::toLower(tokenizer.advance().getValue());

    if (unit == "mm") return x;
    else if (unit == "mil") return x * 0.0254;
    else if (unit == "in") return x * 25.4;
    else if (unit == "cm") return x * 10;
    else THROWS("Unrecognized unit '" << unit << "'");

  }

  if (newFormat) return x * 0.000254; // 1/100 mil
  return x * 0.0254; // mil
}


string Object::formatReal(double x) {
  return String::printf("%0.6fmm", x);
}
