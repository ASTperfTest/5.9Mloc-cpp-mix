#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.15 $
#
#   last change: $Author: vg $ $Date: 2007/10/29 09:58:33 $
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

PRJ=..$/..
BFPRJ=..

PRJNAME=binfilter
TARGET=bf_ofa

NO_HIDS=TRUE

# --- Settings ------------------------------------------------------------

.INCLUDE :	settings.mk
#INC+= -I$(PRJ)$/inc$/bf_offmgr

.IF "$(prjpch)" != ""
CDEFS+=-DPRECOMPILED
.ENDIF

RSCLOCINC!:=$(RSCLOCINC);$(PRJ)$/RES
SOLARLIB+=-L$(LB)

# --- Allgemein -----------------------------------------------------------

RES1FILELIST=\
	$(SOLARCOMMONRESDIR)$/svtools.srs \
	$(SRS)$/so3_src.srs 	

RESLIB1NAME=$(TARGET)
RESLIB1SRSFILES=$(RES1FILELIST)

# -------------------------------------------------------------------------

LIB3TARGET= $(SLB)$/$(TARGET).lib
LIB3FILES=	$(SLB)$/offmgr_app.lib 

SHL2TARGET= $(TARGET)$(UPD)$(DLLPOSTFIX)
SHL2IMPLIB= $(TARGET)
SHL2LIBS=	$(SLB)$/$(TARGET).lib

# dynamic libraries
SHL2STDLIBS+= \
	$(BFSVXLIB) \
	$(LEGACYSMGRLIB)	\
	$(BFBASICLIB) \
	$(BFGOODIESLIB) \
	$(SVTOOLLIB) \
	$(SVLLIB)	\
	$(VCLLIB) \
	$(TOOLSLIB) \
	$(UNOTOOLSLIB) \
	$(COMPHELPERLIB) \
	$(CPPUHELPERLIB) \
	$(CPPULIB) \
	$(VOSLIB) \
    $(SALLIB)

.IF "$(GUI)"=="WNT"
SHL2STDLIBS += $(ADVAPI32LIB)
.ENDIF # WNT

SHL2DEF=	$(MISC)$/$(SHL2TARGET).def
SHL2BASE=	0x1de00000

DEF2NAME=	$(SHL2TARGET)
DEF2DEPN=	$(MISC)$/$(SHL2TARGET).flt
DEFLIB2NAME =$(TARGET)
DEF2DES 	=offmgr app-interface

# -------------------------------------------------------------------------

.IF "$(GUI)"=="WNT"
LIB4TARGET= $(LB)$/a$(TARGET).lib
LIB4FILES=	$(LB)$/$(TARGET).lib
.ENDIF

.IF "$(BUILD_SOSL)"==""
.IF "$(depend)" == ""
ALL:\
	$(LIB3TARGET)			\
	$(INCCOM)$/class.lst	\
	ALLTAR

.ENDIF # "$(depend)" == ""
.ENDIF # "$(BUILD_SOSL)" == ""

.INCLUDE :	target.mk

$(MISC)$/$(SHL2TARGET).flt: makefile.mk
	@echo ------------------------------
	@echo Making: $@
	@echo WEP>$@
	@echo PlugInApplication>>$@
	@echo __dt__17OfficeApplication>>$@
	@echo LibMain>>$@
	@echo _Impl>>$@
	@echo _Imp>>$@
	@echo solver>>$@
	@echo bad_alloc>>$@
	@echo exception12>>$@
.IF "$(GUI)"=="WNT"
	@echo __CT>>$@
.ENDIF

# ------------------------------------------------------------------
# Windows NT
# ------------------------------------------------------------------

$(INCCOM)$/class.lst:
.IF "$(BUILD_SOSL)"==""
.IF "$(GUI)$(CPU)$(UPDATER)"=="WNTIYES"
	-$(COPY) class.lst $@
.ELSE
	@echo nix
.ENDIF
.ENDIF			# "$(BUILD_SOSL)"==""
