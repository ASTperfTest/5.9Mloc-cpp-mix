/**********************************************************************
                                                                       
 FILENAME:     uiuc_menu_mass.cpp

----------------------------------------------------------------------

 DESCRIPTION:  reads input data for specified aircraft and creates 
               approporiate data storage space

----------------------------------------------------------------------

 STATUS:       alpha version

----------------------------------------------------------------------

 REFERENCES:   based on "menu reader" format of Michael Selig

----------------------------------------------------------------------

 HISTORY:      04/04/2003   initial release
               06/30/2003   (RD) replaced istrstream with istringstream
                            to get rid of the annoying warning about
                            using the strstream header

----------------------------------------------------------------------

 AUTHOR(S):    Robert Deters      <rdeters@uiuc.edu>
               Michael Selig      <m-selig@uiuc.edu>

----------------------------------------------------------------------

 VARIABLES:

----------------------------------------------------------------------

 INPUTS:       n/a

----------------------------------------------------------------------

 OUTPUTS:      n/a

----------------------------------------------------------------------

 CALLED BY:    uiuc_menu()

----------------------------------------------------------------------

 CALLS TO:     check_float() if needed
	       d_2_to_3() if needed
	       d_1_to_2() if needed
	       i_1_to_2() if needed
	       d_1_to_1() if needed

 ----------------------------------------------------------------------

 COPYRIGHT:    (C) 2003 by Michael Selig

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**********************************************************************/

#include <simgear/compiler.h>

#if defined( __MWERKS__ )
// -dw- optimizer chokes (big-time) trying to optimize humongous
// loop/switch statements
#pragma optimization_level 0
#endif

#include <cstdlib>
#include <string>
#include STL_IOSTREAM

#include "uiuc_menu_mass.h"

SG_USING_STD(cerr);
SG_USING_STD(cout);
SG_USING_STD(endl);

#ifndef _MSC_VER
SG_USING_STD(exit);
#endif

void parse_mass( const string& linetoken2, const string& linetoken3,
                 const string& linetoken4, const string& linetoken5, 
		 const string& linetoken6, const string& linetoken7, 
		 const string& linetoken8, const string& linetoken9,
		 const string& linetoken10, const string& aircraft_directory,
		 LIST command_line ) {
    double token_value;
    istringstream token3(linetoken3.c_str());
    istringstream token4(linetoken4.c_str());
    istringstream token5(linetoken5.c_str());
    istringstream token6(linetoken6.c_str());
    istringstream token7(linetoken7.c_str());
    istringstream token8(linetoken8.c_str());
    istringstream token9(linetoken9.c_str());
    istringstream token10(linetoken10.c_str());

    switch(mass_map[linetoken2])
      {
      case Weight_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  Weight = token_value;
	  Mass = Weight * INVG;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case Mass_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  Mass = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_xx_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_xx = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_yy_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_yy = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_zz_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_zz = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_xz_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_xz = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case Mass_appMass_ratio_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  Mass_appMass_ratio = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_xx_appMass_ratio_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_xx_appMass_ratio = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_yy_appMass_ratio_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_yy_appMass_ratio = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_zz_appMass_ratio_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_zz_appMass_ratio = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case Mass_appMass_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  Mass_appMass = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_xx_appMass_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_xx_appMass = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_yy_appMass_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_yy_appMass = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      case I_zz_appMass_flag:
	{
	  if (check_float(linetoken3))
	    token3 >> token_value;
	  else
	    uiuc_warnings_errors(1, *command_line);
	  
	  I_zz_appMass = token_value;
	  massParts -> storeCommands (*command_line);
	  break;
	}
      default:
	{
	  if (ignore_unknown_keywords) {
	    // do nothing
	  } else {
	    // print error message
	    uiuc_warnings_errors(2, *command_line);
	  }
	  break;
	}
      };
}
