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

#include "Writer.h"
#include "Template.h"

#include <cbang/json/List.h>
#include <cbang/json/Dict.h>

using namespace std;
using namespace cb;
using namespace PCB;


void Writer::write(const JSON::Value &pcb, unsigned depth) const {
  for (unsigned i = 0; i < pcb.size(); i++) {
    const JSON::Value &e = *pcb.get(i);

    switch (e.getType()) {
    case JSON::ValueType::JSON_STRING: // Comment
      stream << '#' << e.getString() << '\n';
      break;

    case JSON::ValueType::JSON_DICT: { // Element
      if (e.getBoolean("remove", false)) continue;

      string name = e.getString("name");
      const JSON::Value &templ = Template::get(name);

      indent(depth);
      if (name != "Point") stream << name;

      writeHead(e, templ);
      stream << '\n';

      if (templ.hasList("children")) {
        indent(depth);
        stream << "(\n";

        write(e.getList("children"), depth + 1);

        indent(depth);
        stream << ")\n";
      }
      break;
    }

    default: break; // Ignore all others
    }
  }
}


void Writer::writeFlags(const JSON::Value &flags) const {
  stream << '"';

  bool first = true;
  for (unsigned i = 0; i < flags.size(); i++) {
    string key = flags.keyAt(i);
    const JSON::Value &flag = *flags.get(key);

    // Skip false flags
    if (flag.isBoolean() && !flag.getBoolean()) continue;

    if (!first) stream << ',';
    first = false;
    stream << key;

    if (flag.isList()) {
      stream << '(';
      for (unsigned j = 0; j < flag.size(); j++) {
        if (j) stream << ',';
        stream << flag.getString(j);
      }
      stream << ')';
    }
  }

  stream << '"';
}


void Writer::writeHead(const JSON::Value &e, const JSON::Value &templ) const {
  bool square = templ.getBoolean("square", true);
  stream << (square ? '[' : '(');

  const JSON::Value &params = templ.getList("params");
  const JSON::Value &values = e.getDict("params");

  for (unsigned i = 0; i < params.size(); i++) {
    const JSON::Value &param = params.getList(i);
    string name = param.getString(0);
    string type = param.getString(1);

    if (i) stream << ' ';

    if (type == "unit")
      stream << String::printf("%0.6fmm", values.getNumber(name));

    else if (type == "int")
      stream << String::printf("%d", values.getS64(name));

    else if (type == "real")
      stream << String::printf("%0.6f", values.getNumber(name));

    else if (type == "char")
      stream << '\'' << values.getString(name)[0] << '\'';

    else if (type == "string")
      stream << '"' << values.getString(name) << '"';

    else if (type == "flags")
      writeFlags(values.getDict(name));

    else THROWS("Unrecognized param type in template '" << type << "'");
  }

  stream << (square ? ']' : ')');
}


void Writer::indent(unsigned depth) const {
  for (unsigned i = 0; i < depth; i++) stream << "  ";
}
