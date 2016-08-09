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

#ifndef PCB_PARSER_H
#define PCB_PARSER_H

#include "Tokenizer.h"
#include "Layout.h"

#include <cbang/SmartPointer.h>
#include <cbang/io/InputSource.h>


namespace PCB {
  class Parser {
  public:
    cb::SmartPointer<Layout> parse(Tokenizer &tokenizer) const;
    cb::SmartPointer<Layout> parse(const cb::InputSource &source) const;
  };
}

#endif // PCB_PARSER_H
