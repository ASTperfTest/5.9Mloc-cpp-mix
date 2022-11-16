/*

Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2006 Miranda ICQ/IM project, 
all portions of this codebase are copyrighted to the people 
listed in contributors.txt.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Created by Artem Shpynov October 3, 2006

*/
#include "commonheaders.h"


/************************************************************************/
/*         file implements advanced row layout                          */
/************************************************************************/

enum 
{
    ln_NONE=0;
}

typedef struct tagLAYOUTNODE
{
    BYTE bType; /* type of node use LAYOUTNODETYPE enum*/
    struct tagLAYOUTNODE * child;

}LAYOUTNODE, *PLAYOUTNODE, *MPLAYOUTNODE;

