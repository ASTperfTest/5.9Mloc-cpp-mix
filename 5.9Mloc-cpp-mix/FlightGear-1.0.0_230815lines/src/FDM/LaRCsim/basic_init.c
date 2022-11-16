/***************************************************************************

        TITLE:  basic_init.c
        
----------------------------------------------------------------------------

        FUNCTION:

----------------------------------------------------------------------------

        MODULE STATUS:  developmental

----------------------------------------------------------------------------

        GENEALOGY:

----------------------------------------------------------------------------

        DESIGNED BY:    EBJ
        
        CODED BY:       EBJ
        
        MAINTAINED BY:  EBJ

----------------------------------------------------------------------------

        MODIFICATION HISTORY:
        
----------------------------------------------------------------------------

        REFERENCES:

----------------------------------------------------------------------------

        CALLED BY:

----------------------------------------------------------------------------

        CALLS TO:

----------------------------------------------------------------------------

        INPUTS:

----------------------------------------------------------------------------

        OUTPUTS:

--------------------------------------------------------------------------*/
#include "ls_types.h"
#include "ls_generic.h"
#include "ls_cockpit.h"
#include "ls_constants.h"
#include "basic_aero.h"

void basic_init( void ) {

  Throttle[3] = 0.2; 
  
  Dx_pilot = 0; Dy_pilot = 0; Dz_pilot = 0;
/*    Mass=2300*INVG; */
/*    I_xx=948; */
/*    I_yy=1346; */
/*    I_zz=1967; */
/*    I_xz=0; */
        Mass = 2./32.174;
        I_xx   = 0.0454;
        I_yy   = 0.0191;
        I_zz   = 0.0721;
        I_xz   = 0;


  Flap_Position=Flap_handle;
  Flaps_In_Transit=0;


  
  
}
