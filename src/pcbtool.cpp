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
#include <pcb/Align.h>
#include <pcb/ClearFlag.h>
#include <pcb/Mask.h>
#include <pcb/Clearance.h>
#include <pcb/TextScale.h>
#include <pcb/SilkThickness.h>
#include <pcb/ViaThermals.h>
#include <pcb/PinThermals.h>
#include <pcb/FindShort.h>
#include <pcb/FindAskew.h>
#include <pcb/FindContiguous.h>
#include <pcb/RemoveFound.h>

#include <cbang/Exception.h>
#include <cbang/Application.h>
#include <cbang/ApplicationMain.h>
#include <cbang/io/Reader.h>
#include <cbang/log/Logger.h>
#include <cbang/os/SystemUtilities.h>
#include <cbang/json/Writer.h>

#include <iostream>

using namespace std;
using namespace cb;
using namespace PCB;


namespace PCB {
  class PCBTool : public cb::Application, public cb::Reader {
    bool inplace;
    bool json;
    double align;
    bool findAskew;
    double findShort;
    bool removeContiguous;
    bool remove;
    string viaThermals;
    string pinThermals;
    double silkThickness;
    unsigned textScale;
    double mask;
    double clearance;

  public:
    PCBTool() :
      Application("PCB Tool", &PCBTool::_hasFeature), inplace(false),
      json(false), align(0), findAskew(false), findShort(0),
      removeContiguous(false), remove(false), silkThickness(0), textScale(0),
      mask(0), clearance(0) {
      cmdLine.addTarget("inplace", inplace, "Do file changes inplace instead "
                        "of outputing to standard out.", 'i');
      cmdLine.addTarget("json", json, "Output in JSON format.", 'j');
      cmdLine.addTarget("align", align, "Align to grid");
      cmdLine.addTarget("find-askew", findAskew, "Find all lines which are "
                        "not at a multiple of 45 degrees.");
      cmdLine.addTarget("find-short", findShort, "Find all lines which are less"
                        " than length.");
      cmdLine.addTarget("remove-contiguous", removeContiguous, "Find lines "
                        "which either overlap or are connected in a straight "
                        "line and replace them with a single line.");
      cmdLine.addTarget("remove", remove, "Remove found items.", 'r');
      cmdLine.addTarget("via-thermals", viaThermals, "Set all Via thermals.  "
                        "Valid values are 'S', 'X', '+' or ''");
      cmdLine.addTarget("pin-thermals", pinThermals, "Set all Pin thermals.  "
                        "Valid values are 'S', 'X', '+' or ''");
      cmdLine.addTarget("silk-thickness", silkThickness, "Set all silk line "
                        "thicknesses in mm.");
      cmdLine.addTarget("mask", mask, "Set solder mask opening on all "
                        "pads, pins and vias in mm.");
      cmdLine.addTarget("clearance", clearance, "Set clearance on all lines, "
                        "pads, pins and vias in mm where current clearance is "
                        "non-zero.");
      cmdLine.addTarget("text-scale", textScale, "Set all text scales.");

      // Force 'C' locale, otherwise double parsing is messed up.
      cb::SystemUtilities::setenv("LC_NUMERIC", "C");

      cb::Logger::instance().setScreenStream(cerr);
      cb::Logger::instance().setLogTime(false);

      cmdLine.setShowKeywordOpts(false);
    }


    static bool _hasFeature(int feature) {
      switch (feature) {
      case FEATURE_SIGNAL_HANDLER:
        return false;

      default: return Application::_hasFeature(feature);
      }
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
      // Input
      SmartPointer<JSON::Value> layout = PCB::Parser().parse(source);

      // Process
      (ClearFlag("found"))(*layout);

      if (removeContiguous) {
        ((FindContiguous()))(*layout);
        ((RemoveFound()))(*layout);
      }

      if (align) (Align(align))(*layout);
      if (mask) (Mask(mask))(*layout);
      if (clearance) (Clearance(clearance))(*layout);
      if (textScale) (TextScale(textScale))(*layout);
      if (silkThickness) (SilkThickness(silkThickness))(*layout);
      if (cmdLine["--via-thermals"].isSet())
        (ViaThermals(viaThermals))(*layout);
      if (cmdLine["--pin-thermals"].isSet())
        (PinThermals(pinThermals))(*layout);

      if (findShort) (FindShort(findShort))(*layout);
      if (findAskew) ((FindAskew()))(*layout);

      if (remove) ((RemoveFound()))(*layout);

      // Output
      SmartPointer<ostream> stream;
      if (inplace) stream = SystemUtilities::oopen(source.getName());
      else stream = SmartPointer<ostream>::Phony(&cout);

      if (json) {
        JSON::Writer writer(*stream);
        layout->write(writer);

      } else Writer(*stream).write(*layout);
    }
  };
}


int main(int argc, char *argv[]) {
  return doApplication<PCB::PCBTool>(argc, argv);
}
