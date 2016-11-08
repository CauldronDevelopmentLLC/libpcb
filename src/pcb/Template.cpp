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

#include "Template.h"

#include <cbang/util/Resource.h>
#include <cbang/json/Reader.h>
#include <cbang/json/Dict.h>

using namespace std;
using namespace cb;
using namespace PCB;

namespace PCB {
  extern const DirectoryResource resource0;
}


SmartPointer<JSON::Value> Template::templ;


const JSON::Value &Template::get(const string &keyword) {
  if (templ.isNull()) {
    stringstream str;
    str << resource0.get("template.json");
    templ = JSON::Reader::parse(str);
  }

  if (!templ->hasDict(keyword))
    THROWS("Unrecognized keyword '" << keyword << "'");

  return templ->getDict(keyword);
}
