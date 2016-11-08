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

#ifndef PCB_WRITER_H
#define PCB_WRITER_H

#include <cbang/io/OutputSink.h>
#include <cbang/json/Value.h>


namespace PCB {
  class Writer {
    cb::OutputSink sink;
    std::ostream &stream;

  public:
    Writer(const cb::OutputSink &sink) : sink(sink), stream(sink.getStream()) {}

    void write(const cb::JSON::Value &pcb, unsigned depth = 0) const;

    void writeFlags(const cb::JSON::Value &e) const;
    void writeHead(const cb::JSON::Value &e,
                   const cb::JSON::Value &templ) const;

    void indent(unsigned depth) const;
  };
}

#endif // PCB_WRITER_H
