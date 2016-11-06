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
#include "Flags.h"

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

    Flags getFlags(unsigned index) const;
    bool hasFlag(unsigned index, const std::string &flag) const;
    std::string getFlag(unsigned index, const std::string &flag) const;
    void setFlag(unsigned index, const std::string &flag,
                 const std::string &value = "");
    void clearFlag(unsigned index, const std::string &flag);
    void setThermals(unsigned index, const std::string &thermals);

    double getDouble(unsigned index) const;
    void setDouble(unsigned index, double x);

    int64_t getInteger(unsigned index) const;
    void setInteger(unsigned index, int64_t x);

    void align(unsigned index, double x);
    static double alignValue(double value, double x);

    virtual void align(double x) {}
    virtual void setViaThermals(const std::string &thermal) {}
    virtual void setPinThermals(const std::string &thermal) {}
    virtual void setSilkThickness(double thickness) {}
    virtual void setTextScale(unsigned scale) {}
    virtual void findAskew() {}
    virtual void findShort(double length) {}
    virtual void findContiguous() {}
    virtual void clearFound() {}
    virtual bool isFound() {return false;}
    virtual void removeFound() {}

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
