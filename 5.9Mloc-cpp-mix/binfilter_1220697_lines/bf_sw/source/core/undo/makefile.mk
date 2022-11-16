#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.8 $
#
#   last change: $Author: obo $ $Date: 2007/03/09 16:40:06 $
#
#   The Contents of this file are made available subject to
#   the terms of GNU Lesser General Public License Version 2.1.
#
#
#     GNU Lesser General Public License Version 2.1
#     =============================================
#     Copyright 2005 by Sun Microsystems, Inc.
#     901 San Antonio Road, Palo Alto, CA 94303, USA
#
#     This library is free software; you can redistribute it and/or
#     modify it under the terms of the GNU Lesser General Public
#     License version 2.1, as published by the Free Software Foundation.
#
#     This library is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public
#     License along with this library; if not, write to the Free Software
#     Foundation, Inc., 59 Temple Place, Suite 330, Boston,
#     MA  02111-1307  USA
#
#*************************************************************************

EXTERNAL_WARNINGS_NOT_ERRORS := TRUE

PRJ=..$/..$/..$/..
BFPRJ=..$/..$/..

PRJNAME=binfilter
TARGET=sw_undo

NO_HIDS=TRUE

# --- Settings -----------------------------------------------------

.INCLUDE :  $(PRJ)$/inc$/bf_sw$/swpre.mk
.INCLUDE :  settings.mk
.INCLUDE :  $(PRJ)$/inc$/bf_sw$/sw.mk
INC+= -I$(PRJ)$/inc$/bf_sw

.IF "$(GUI)$(COM)" == "WINMSC"
LIBFLAGS=/NOI /NOE /PAGE:128
.ENDIF

# --- Files --------------------------------------------------------

SLOFILES =	\
        $(SLO)$/sw_docundo.obj \
		$(SLO)$/sw_rolbck.obj \
		$(SLO)$/sw_unattr.obj \
		$(SLO)$/sw_unbkmk.obj \
		$(SLO)$/sw_undel.obj \
		$(SLO)$/sw_undobj.obj \
		$(SLO)$/sw_undobj1.obj \
		$(SLO)$/sw_undraw.obj \
		$(SLO)$/sw_unfmco.obj \
		$(SLO)$/sw_unins.obj \
		$(SLO)$/sw_unnum.obj \
		$(SLO)$/sw_unsect.obj \
		$(SLO)$/sw_unspnd.obj \
		$(SLO)$/sw_untbl.obj

# --- Tagets -------------------------------------------------------

.INCLUDE :  target.mk

