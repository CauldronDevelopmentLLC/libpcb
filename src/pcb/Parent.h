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

#ifndef PCB_PARENT_H
#define PCB_PARENT_H

#include "Object.h"

#include <list>


namespace PCB {
  class Parent : public Object {
  protected:
    typedef std::list<cb::SmartPointer<Object> > children_t;
    children_t children;

  public:
    Parent(const std::string &name) : Object(name) {}
    Parent(const Object &o) : Object(o) {}

    bool hasChildren() const {return !children.empty();}
    unsigned numChildren() const {return children.size();}

    void findChildren(const std::string &name,
                      std::vector<cb::SmartPointer<Object> > &children) const;

    typedef children_t::iterator iterator;
    typedef children_t::const_iterator const_iterator;
    iterator begin() {return children.begin();}
    iterator end() {return children.end();}
    const_iterator begin() const {return children.begin();}
    const_iterator end() const {return children.end();}

    void add(const cb::SmartPointer<Object> &o) {children.push_back(o);}

    virtual void centerMark();
    virtual void subtractMask(int clear);

    Point findCenter();
    void center();
    virtual void merge(const Parent &o);

    // From Object
    void rotate(const Point &center, double angle);
    void translate(const Point &t);
    void multiply(double m);
    void round(int x);
    void bounds(Point &min, Point &max) const;
    void flipX(double x);
    void flipY(double x);
    void lineSize(int size);
    void textScale(int scale);

    void parse(Tokenizer &tokenizer);
    void write(std::ostream &stream, unsigned depth) const;

    void parseBody(Tokenizer &tokenizer);
  };
}

#endif // PCB_PARENT_H
