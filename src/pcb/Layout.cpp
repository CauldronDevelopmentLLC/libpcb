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

#include "Layout.h"

#include "Layer.h"
#include "NetList.h"

#include <cbang/Exception.h>
#include <cbang/log/Logger.h>

#include <set>
#include <limits>

using namespace std;
using namespace cb;
using namespace PCB;


SmartPointer<Layer> Layout::findLayer(int number) const {
  for (const_iterator it = begin(); it != end(); it++) {
    if ((*it)->getName() != "Layer") continue;

    SmartPointer<Layer> layer = it->cast<Layer>();
    if (layer->getNumber() == number) return layer;
  }

  return 0;
}


void Layout::parse(Tokenizer &tokenizer) {
  parseBody(tokenizer);
}


void Layout::write(ostream &stream, unsigned depth) const {
  for (const_iterator it = begin(); it != end(); it++) {
    (*it)->write(stream, depth);
    stream << '\n';
  }
}


void Layout::merge(const Parent &o) {
  const Layout *l = dynamic_cast<const Layout *>(&o);
  if (!l) THROWS("Cannot merge Layout with " << o.getName());
  const Layout &layout = *l;

  string last;
  set<int> foundLayers;
  bool foundNetList = false;

  for (iterator it = begin(); it != end(); it++) {
    SmartPointer<Object> o = *it;

    if (o->getName() != "Via" && last == "Via") {
      vector<SmartPointer<Object> > objs;
      layout.findChildren("Via", objs);
      children.insert(it, objs.begin(), objs.end());
    }

    if (o->getName() != "Element" && last == "Element") {
      vector<SmartPointer<Object> > objs;
      layout.findChildren("Element", objs);
      children.insert(it, objs.begin(), objs.end());
    }

    if (o->getName() == "Layer") {
      SmartPointer<Layer> layer1 = o.cast<Layer>();
      SmartPointer<Layer> layer2 = layout.findLayer(layer1->getNumber());
      if (!layer2.isNull()) layer1->merge(*layer2);

      foundLayers.insert(layer1->getNumber());
    }

    if (o->getName() != "Layer" && last == "Layer") {
      vector<SmartPointer<Object> > layers;
      vector<SmartPointer<Object> > objs;
      layout.findChildren("Layer", layers);

      for (unsigned int i = 0; i < layers.size(); i++) {
        SmartPointer<Layer> layer = layers[i].cast<Layer>();
        if (foundLayers.find(layer->getNumber()) == foundLayers.end())
          objs.push_back(layer);
      }

      children.insert(it, objs.begin(), objs.end());
    }

    if (o->getName() == "NetList") {
      SmartPointer<NetList> netList1 = o.cast<NetList>();
      vector<SmartPointer<Object> > objs;
      layout.findChildren("NetList", objs);

      if (objs.size()) {
        SmartPointer<NetList> netList2 = objs[0].cast<NetList>();
        netList1->merge(*netList2);
        foundNetList = true;
      }
    }

    last = o->getName();
  }

  if (!foundNetList) {
    vector<SmartPointer<Object> > objs;
    layout.findChildren("NetList", objs);
    children.insert(children.end(), objs.begin(), objs.end());
  }
}
