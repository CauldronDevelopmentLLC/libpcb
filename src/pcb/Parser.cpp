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

#include "Parser.h"
#include "Template.h"

#include <cbang/String.h>
#include <cbang/parse/Scanner.h>
#include <cbang/json/Builder.h>
#include <cbang/json/Dict.h>
#include <cbang/json/List.h>

#include <sstream>


using namespace std;
using namespace cb;
using namespace PCB;


void Parser::parse(PCB::Tokenizer &tokenizer, JSON::Sink &sink) const {
  try {
    parseBody(tokenizer, sink);

  } catch (const Exception &e) {
    throw Exception("Parse error", tokenizer.getLocation().getStart(), e);
  }
}


JSON::ValuePtr Parser::parse(const InputSource &source) const {
  PCB::Tokenizer tokenizer(new Scanner(source));
  JSON::Builder builder;

  parse(tokenizer, builder);

  return builder.getRoot();
}


void Parser::parseFlagValues(string::const_iterator &it,
                             const string::const_iterator &end,
                             JSON::Sink &sink) const {
  sink.beginList();

  if (it == end || *it++ != '(') THROW("Expected '('");

  string flag;
  while (true) {
    if (it == end) THROW("Unexpected end of flag values");
    char c = *it++;

    if (c == ')') {
      if (!flag.empty()) sink.append(flag);
      break;
    }

    switch (c) {
    case ',':
      sink.append(flag);
      flag.clear();
      break;

    case '(': THROW("Unexpected '(' in flag values");

    default: flag.append(1, c); break;
    }
  }

  sink.endList();
}


void Parser::parseFlags(const string &flags, JSON::Sink &sink) const {
  sink.beginDict();

  string::const_iterator it = flags.begin();
  const string::const_iterator end = flags.end();
  string name;

  while (it != end) {
    switch (*it) {
    case ')': THROW("Unexpected ')' in flags");
    case '(':
      if (name.empty()) THROW("Empty name in flags");
      sink.beginInsert(name);
      name.clear();
      parseFlagValues(it, end, sink);
      break;

    case ',':
      if (name.empty()) THROW("Empty name in flags");
      sink.insertBoolean(name, true);
      name.clear();
      it++;
      break;

    default: name.append(1, *it++); break;
    }
  }

  // Final flag
  if (!name.empty()) sink.insertBoolean(name, true);

  sink.endDict();
}


void Parser::parseHead(Tokenizer &tokenizer, JSON::Sink &sink,
                       const JSON::Value &templ) const {
  bool square = templ.getBoolean("square", true);
  tokenizer.match(square ? OBRACKET_TOKEN : OPAREN_TOKEN);

  sink.insertDict("params");

  const JSON::Value &params = templ.getList("params");

  for (unsigned i = 0; i < params.size(); i++) {
    const JSON::Value &param = params.getList(i);
    string name = param.getString(0);
    string type = param.getString(1);

    sink.beginInsert(name);

    if (type == "unit") {
      double value =
        String::parseDouble(tokenizer.match(REAL_TOKEN).getValue());

      if (tokenizer.getType() == ID_TOKEN) {
        string unit = String::toLower(tokenizer.advance().getValue());

        if (unit == "mil") value *= 0.0254;
        else if (unit == "in") value *= 25.4;
        else if (unit == "cm") value *= 10;
        else if (unit != "mm") THROW("Unrecognized unit '" << unit << "'");

      } else if (square) value *= 0.000254; // 1/100 mil
      else value *= 0.0254; // mil

      sink.write(value);

    } else if (type == "int") {
      sink.write(String::parseS64(tokenizer.advance().getValue()));

    } else if (type == "real") {
      sink.write(String::parseDouble(tokenizer.match(REAL_TOKEN).getValue()));

    } else if (type == "char" || type == "string") {
      sink.write(tokenizer.advance().getValue());

    } else if (type == "flags") {
      parseFlags(tokenizer.advance().getValue(), sink);

    } else THROW("Unrecognized param type in template '" << type << "'");
  }

  sink.endDict();

  tokenizer.match(square ? CBRACKET_TOKEN : CPAREN_TOKEN);
}


void Parser::parseElement(Tokenizer &tokenizer, const string &name,
                          JSON::Sink &sink) const {
  const JSON::Value &templ = Template::get(name);

  sink.appendDict();
  sink.insert("name", name);

  parseHead(tokenizer, sink, templ);

  if (tokenizer.getType() == OPAREN_TOKEN) {
    if (!templ.hasList("children"))
      THROW("Did not expect child elements with " << name);

    tokenizer.advance();
    sink.beginInsert("children");
    parseBody(tokenizer, sink);
    tokenizer.match(CPAREN_TOKEN);
  }

  sink.endDict();
}


void Parser::parseBody(PCB::Tokenizer &tokenizer, JSON::Sink &sink) const {
  sink.beginList();

  while (true)
    switch (tokenizer.getType()) {
    case EOF_TOKEN: case CPAREN_TOKEN: sink.endList(); return;
    case COMMENT_TOKEN: sink.append(tokenizer.advance().getValue()); break;

    case ID_TOKEN:
      parseElement(tokenizer, tokenizer.advance().getValue(), sink);
      break;

    case OBRACKET_TOKEN: // Special case for points in Polygon
      parseElement(tokenizer, "Point", sink);
      break;

    default:
      THROW("Unexpected token " << tokenizer.peek() << " in " << getName()
             << " body");
    }
}
