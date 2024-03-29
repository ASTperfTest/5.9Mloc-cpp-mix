/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Module:       FGElectric.cpp
 Author:       David Culp
 Date started: 04/07/2004
 Purpose:      This module models an electric motor

 --------- Copyright (C) 2004  David Culp (davidculp2@comcast.net) -------------

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place - Suite 330, Boston, MA  02111-1307, USA.

 Further information about the GNU Lesser General Public License can also be found on
 the world wide web at http://www.gnu.org.

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------

This class descends from the FGEngine class and models an electric motor based on
parameters given in the engine config file for this class

HISTORY
--------------------------------------------------------------------------------
04/07/2004  DPC  Created
01/06/2005  DPC  Converted to new XML format

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "FGElectric.h"
#include <models/FGPropulsion.h>

namespace JSBSim {

static const char *IdSrc = "$Id: FGElectric.cpp,v 1.1.2.1 2007-01-06 10:49:25 ehofman Exp $";
static const char *IdHdr = ID_ELECTRIC;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FGElectric::FGElectric(FGFDMExec* exec, Element *el, int engine_number)
  : FGEngine(exec, el, engine_number)
{
  string token;

  Type = etElectric;
  PowerWatts = 745.7;
  hptowatts = 745.7;

  dt = State->Getdt();

  if (el->FindElement("power"))
    PowerWatts = el->FindElementValueAsNumberConvertTo("power","WATTS");

  Debug(0); // Call Debug() routine from constructor if needed
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

FGElectric::~FGElectric()
{
  Debug(1); // Call Debug() routine from constructor if needed
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double FGElectric::Calculate(void)
{
  Throttle = FCS->GetThrottlePos(EngineNumber);

  RPM = Thruster->GetRPM() * Thruster->GetGearRatio();

  HP = PowerWatts * Throttle / hptowatts;

  PowerAvailable = (HP * hptoftlbssec) - Thruster->GetPowerRequired();

  return Thruster->Calculate(PowerAvailable);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

string FGElectric::GetEngineLabels(string delimeter)
{
  return ""; // currently no labels are returned for this engine
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

string FGElectric::GetEngineValues(string delimeter)
{
  return ""; // currently no values are returned for this engine
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//    The bitmasked value choices are as follows:
//    unset: In this case (the default) JSBSim would only print
//       out the normally expected messages, essentially echoing
//       the config files as they are read. If the environment
//       variable is not set, debug_lvl is set to 1 internally
//    0: This requests JSBSim not to output any messages
//       whatsoever.
//    1: This value explicity requests the normal JSBSim
//       startup messages
//    2: This value asks for a message to be printed out when
//       a class is instantiated
//    4: When this value is set, a message is displayed when a
//       FGModel object executes its Run() method
//    8: When this value is set, various runtime state variables
//       are printed out periodically
//    16: When set various parameters are sanity checked and
//       a message is printed out when they go out of bounds

void FGElectric::Debug(int from)
{
  if (debug_lvl <= 0) return;

  if (debug_lvl & 1) { // Standard console startup message output
    if (from == 0) { // Constructor

      cout << "\n    Engine Name: "         << Name << endl;
      cout << "      Power Watts: "         << PowerWatts << endl;

    }
  }
  if (debug_lvl & 2 ) { // Instantiation/Destruction notification
    if (from == 0) cout << "Instantiated: FGElectric" << endl;
    if (from == 1) cout << "Destroyed:    FGElectric" << endl;
  }
  if (debug_lvl & 4 ) { // Run() method entry print for FGModel-derived objects
  }
  if (debug_lvl & 8 ) { // Runtime state variables
  }
  if (debug_lvl & 16) { // Sanity checking
  }
  if (debug_lvl & 64) {
    if (from == 0) { // Constructor
      cout << IdSrc << endl;
      cout << IdHdr << endl;
    }
  }
}

double
FGElectric::CalcFuelNeed(void)
{
  return 0;
}

} // namespace JSBSim
