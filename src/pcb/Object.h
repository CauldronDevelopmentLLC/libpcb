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

#ifndef PCB_OBJECT_H
#define PCB_OBJECT_H

#include "Tokenizer.h"
#include "TokenType.h"

#include <cbang/SmartPointer.h>
#include <cbang/util/Variant.h>

#include <iostream>
#include <string>


namespace PCB {
  class Point;
  class Layer;

  class Object : public TokenType {
  protected:
    std::string name;

    bool square;
    std::vector<cb::Variant> header;

  public:
    Object(const std::string &name) : name(name), square(false) {}
    Object(const Object &o) :
    name(o.name), square(o.square), header(o.header) {}
    virtual ~Object() {} // Compiler needs this

    const std::string &getName() const {return name;}

    virtual void init() {}
    virtual void rotate(const Point &center, double angle) {}
    virtual void translate(const Point &t) {}
    virtual void multiply(double m) {}
    virtual void round(int x) {}
    virtual void bounds(Point &minPt, Point &maxPt) const {}
    virtual void flipX(double x) {}
    virtual void flipY(double x) {}
    virtual void lineSize(int size) {}
    virtual void textScale(int scale) {}

    static void round(double &v, int i);
    static void round(int &v, int i);

    virtual void parse(Tokenizer &tokenizer);
    virtual void write(std::ostream &stream, unsigned depth = 0) const;
    virtual void read(std::istream &stream);

    static double parseReal(Tokenizer &tokenizer, bool newFormat);
    static std::string formatReal(double x);

    static std::string indent(unsigned depth) {
      return std::string(depth * 2, ' ');
    }
  };


  inline std::ostream &operator<<(std::ostream &stream, const Object &o) {
    o.write(stream);
    return stream;
  }


  inline std::istream &operator>>(std::istream &stream, Object &o) {
    o.read(stream);
    return stream;
  }
}

#endif // PCB_OBJECT_H
