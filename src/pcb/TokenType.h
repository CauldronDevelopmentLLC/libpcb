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

#ifndef CBANG_ENUM
#ifndef PCB_TOKEN_TYPE_H
#define PCB_TOKEN_TYPE_H

#define CBANG_ENUM_NAME TokenType
#define CBANG_ENUM_NAMESPACE PCB
#define CBANG_ENUM_PATH pcb
#include <cbang/enum/MakeEnumeration.def>

#endif // PCB_TOKEN_TYPE_H
#else // CBANG_ENUM

CBANG_ENUM(EOF_TOKEN)
CBANG_ENUM(COMMENT_TOKEN)
CBANG_ENUM(REAL_TOKEN)
CBANG_ENUM(INTEGER_TOKEN)
CBANG_ENUM(ID_TOKEN)
CBANG_ENUM(STRING_TOKEN)
CBANG_ENUM(CHAR_TOKEN)
CBANG_ENUM(OBRACKET_TOKEN)
CBANG_ENUM(CBRACKET_TOKEN)
CBANG_ENUM(OPAREN_TOKEN)
CBANG_ENUM(CPAREN_TOKEN)

#endif // PCB_TOKEN_TYPE_H
