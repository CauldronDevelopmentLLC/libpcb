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

#include <pcb/PCB.h>

#include <cbang/Exception.h>
#include <cbang/Application.h>
#include <cbang/ApplicationMain.h>
#include <cbang/io/Reader.h>
#include <cbang/log/Logger.h>
#include <cbang/os/SystemUtilities.h>

#include <iostream>

using namespace std;
using namespace cb;
using namespace PCB;


namespace PCB {
  class PCBTool : public cb::Application, public cb::Reader {
    bool centroid;

  public:
    PCBTool() : Application("PCB Tool"), centroid(false) {
      cmdLine.addTarget("centroid", centroid, "Output centroid flie", 'c');

      // Force 'C' locale, otherwise double parsing is messed up.
      cb::SystemUtilities::setenv("LC_NUMERIC", "C");

      cb::Logger::instance().setScreenStream(cerr);
      cb::Logger::instance().setLogTime(false);

      cmdLine.setShowKeywordOpts(false);
    }


    // From Application
    void run() {
      const vector<string> &args = cmdLine.getPositionalArgs();
      if (args.empty()) read(cb::InputSource(cin, "<stdin>"));
      vector<string>::const_iterator it;
      for (it = args.begin(); it != args.end(); it++) read(*it);

      cout << flush;
    }


    // From cb::Reader
    void read(const InputSource &source) {
      SmartPointer<PCB::Layout> layout = PCB::Parser().parse(source);

      if (centroid) ;
      else layout->write(cout);
    }
  };
}


int main(int argc, char *argv[]) {
  return doApplication<PCB::PCBTool>(argc, argv);
}
