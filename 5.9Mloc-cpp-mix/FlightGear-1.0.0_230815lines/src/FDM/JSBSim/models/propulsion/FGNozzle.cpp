/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Module:       FGNozzle.cpp
 Author:       Jon S. Berndt
 Date started: 08/24/00
 Purpose:      Encapsulates the nozzle object

 ------------- Copyright (C) 2000  Jon S. Berndt (jsb@hal-pc.org) -------------

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

HISTORY
--------------------------------------------------------------------------------
08/24/00  JSB  Created

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <sstream>

#include "FGNozzle.h"
#include <models/FGAtmosphere.h>

namespace JSBSim {

static const char *IdSrc = "$Id: FGNozzle.cpp,v 1.1.2.2 2007-05-27 09:33:11 ehofman Exp $";
static const char *IdHdr = ID_NOZZLE;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


FGNozzle::FGNozzle(FGFDMExec* FDMExec, Element* nozzle_element, int num)
                    : FGThruster(FDMExec, nozzle_element, num)
{

  if (nozzle_element->FindElement("pe"))
    PE = nozzle_element->FindElementValueAsNumberConvertTo("pe", "PSF");
  else {
    cerr << "Fatal Error: Nozzle exit pressure must be given in nozzle config file." << endl;
    exit(-1);
  }
  if (nozzle_element->FindElement("expr"))
    ExpR = nozzle_element->FindElementValueAsNumber("expr");
  else {
    cerr << "Fatal Error: Nozzle expansion ratio must be given in nozzle config file." << endl;
    exit(-1);
  }
  if (nozzle_element->FindElement("nzl_eff"))
    nzlEff = nozzle_element->FindElementValueAsNumber("nzl_eff");
  else {
    cerr << "Fatal Error: Nozzle efficiency must be given in nozzle config file." << endl;
    exit(-1);
  }
  if (nozzle_element->FindElement("diam"))
    Diameter = nozzle_element->FindElementValueAsNumberConvertTo("diam", "FT");
  else {
    cerr << "Fatal Error: Nozzle diameter must be given in nozzle config file." << endl;
    exit(-1);
  }

  Thrust = 0;
  Type = ttNozzle;
  Area2 = (Diameter*Diameter/4.0)*M_PI;
  AreaT = Area2/ExpR;
  
  Debug(0);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

FGNozzle::~FGNozzle()
{
  Debug(1);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double FGNozzle::Calculate(double CfPc)
{
  double pAtm = fdmex->GetAtmosphere()->GetPressure();
  if (CfPc > 0)
    Thrust = max((double)0.0, (CfPc * AreaT + (PE - pAtm)*Area2) * nzlEff);
  else
    Thrust = 0.0;

  vFn(1) = Thrust * cos(ReverserAngle);

  ThrustCoeff = max((double)0.0, CfPc / ((pAtm - PE) * Area2));

  return Thrust;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double FGNozzle::GetPowerRequired(void)
{
  return PE;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

string FGNozzle::GetThrusterLabels(int id, string delimeter)
{
  std::ostringstream buf;

  buf << Name << " Thrust (engine " << id << " in lbs)";

  return buf.str();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

string FGNozzle::GetThrusterValues(int id, string delimeter)
{
  std::ostringstream buf;

  buf << Thrust;

  return buf.str();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

void FGNozzle::Debug(int from)
{
  if (debug_lvl <= 0) return;

  if (debug_lvl & 1) { // Standard console startup message output
    if (from == 0) { // Constructor
      cout << "      Nozzle Name: " << Name << endl;
      cout << "      Nozzle Exit Pressure = " << PE << endl;
      cout << "      Nozzle Expansion Ratio = " << ExpR << endl;
      cout << "      Nozzle Efficiency = " << nzlEff << endl;
      cout << "      Nozzle Diameter = " << Diameter << endl;
    }
  }
  if (debug_lvl & 2 ) { // Instantiation/Destruction notification
    if (from == 0) cout << "Instantiated: FGNozzle" << endl;
    if (from == 1) cout << "Destroyed:    FGNozzle" << endl;
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
}
