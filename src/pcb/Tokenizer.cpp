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

#include "Tokenizer.h"

#include <cbang/Exception.h>
#include <cbang/String.h>

#include <cctype>

using namespace std;
using namespace PCB;


namespace {
  int whiteSpace[] = {' ', '\t', '\r', '\n', 0xfeff, 0};
}


bool Tokenizer::isID(const string &id) const {
  return isType(ID_TOKEN) &&
    cb::String::toUpper(id) == cb::String::toUpper(getValue());
}


void Tokenizer::parseComment() {
  scanner.match('#');
  current.setType(COMMENT_TOKEN);

  string value;
  while (scanner.hasMore() && scanner.peek() != '\n') {
    if (scanner.peek() != '\r') value.push_back(scanner.peek());
    scanner.advance();
  }

  current.setValue(value);
}


void Tokenizer::parseNumber() {
  string value;
  bool foundDot = false;
  char c = scanner.peek();

  if (c == '-') {
    value.push_back('-');
    scanner.advance();
    c = scanner.peek();
  }

  do {
    if (c == '.') foundDot = true;

    value.push_back(c);
    scanner.advance();

    if (!scanner.hasMore()) break;
    c = scanner.peek();

  } while (isdigit(c) || (!foundDot && c == '.'));


  if (foundDot && value.length() == 1)
    THROWS("Invalid decimal point, expected number");

  current.set(foundDot ? REAL_TOKEN : INTEGER_TOKEN, value);
}


void Tokenizer::parseID() {
  string value;

  char c = scanner.peek();
  while (isalpha(c) || c == '_') {
    value.push_back(c);

    scanner.advance();
    if (!scanner.hasMore()) break;
    c = scanner.peek();
  }

  current.set(ID_TOKEN, value);
}


void Tokenizer::parseString() {
  scanner.match('"');

  bool escape = false;
  string value;

  while (true) {
    char c = scanner.peek();
    scanner.advance();

    if (escape) value.push_back(c);
    escape = false;

    if (c == '\\') escape = true;
    else if (c == '"') break;
    else value.push_back(c);
  }

  current.set(STRING_TOKEN, value);
}


void Tokenizer::parseChar() {
  scanner.match('\'');
  if (!scanner.hasMore()) THROWS("' at end of file");

  string value(1, scanner.peek());

  scanner.advance();
  scanner.match('\'');

  current.set(CHAR_TOKEN, value);
}


void Tokenizer::next() {
  scanner.skipWhiteSpace(whiteSpace);

  cb::FileLocation start = scanner.getLocation();

  if (!scanner.hasMore()) {
    current.set(EOF_TOKEN, "");
    return;
  }

  bool needAdvance = true;
  int c = scanner.peek();
  switch (c) {
  case 0: current.set(EOF_TOKEN, ""); return;

  case '#': parseComment(); needAdvance = false; break;

  case '-': case '.':
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    parseNumber();
    needAdvance = false;
    break;

  case '"': parseString(); needAdvance = false; break;
  case '\'': parseChar(); needAdvance = false; break;

  case '[': current.set(OBRACKET_TOKEN, c); break;
  case ']': current.set(CBRACKET_TOKEN, c); break;
  case '(': current.set(OPAREN_TOKEN, c); break;
  case ')': current.set(CPAREN_TOKEN, c); break;

  default:
    if (isalpha(c) || c == '_') {
      parseID();
      needAdvance = false;

    } else
      THROWS("Invalid character: '" << cb::String::escapeC(c) << "'");
  }

  if (needAdvance) scanner.advance();

  current.getLocation() = cb::LocationRange(start, scanner.getLocation());
}
