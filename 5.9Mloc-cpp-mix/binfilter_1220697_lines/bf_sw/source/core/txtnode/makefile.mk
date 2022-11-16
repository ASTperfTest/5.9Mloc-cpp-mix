#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.8 $
#
#   last change: $Author: rt $ $Date: 2006/10/27 23:16:28 $
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
TARGET=sw_txtnode

NO_HIDS=TRUE

# --- Settings -----------------------------------------------------

.INCLUDE :  $(PRJ)$/inc$/bf_sw$/swpre.mk
.INCLUDE :  settings.mk
.INCLUDE :  $(PRJ)$/inc$/bf_sw$/sw.mk
INC+= -I$(PRJ)$/inc$/bf_sw
INCEXT=s:$/solar$/inc$/hm

.IF "$(mydebug)" != ""
CDEFS+=-Dmydebug
.ENDIF

.IF "$(GUI)$(COM)" == "WINMSC"
LIBFLAGS=/NOI /NOE /PAGE:512
.ENDIF

# --- Files --------------------------------------------------------

SLOFILES =  \
		$(SLO)$/sw_atrfld.obj \
		$(SLO)$/sw_atrflyin.obj \
		$(SLO)$/sw_atrftn.obj \
		$(SLO)$/sw_atrref.obj \
		$(SLO)$/sw_atrtox.obj \
		$(SLO)$/sw_fmtatr1.obj \
		$(SLO)$/sw_fmtatr2.obj \
		$(SLO)$/sw_fntcap.obj \
		$(SLO)$/sw_fntcache.obj \
		$(SLO)$/sw_swfntcch.obj \
		$(SLO)$/sw_ndhints.obj \
		$(SLO)$/sw_ndtxt.obj \
		$(SLO)$/sw_swfont.obj \
		$(SLO)$/sw_thints.obj \
		$(SLO)$/sw_txatbase.obj \
		$(SLO)$/sw_txtatr2.obj \
		$(SLO)$/sw_txtedt.obj

.IF "$(dbutil)" != ""
OFILES+=$(SLO)$/sw_dbchratr.$(QBJX)
.ENDIF

# --- Tagets -------------------------------------------------------

.INCLUDE :  target.mk
