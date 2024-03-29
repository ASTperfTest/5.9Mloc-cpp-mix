#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.10 $
#
#   last change: $Author: vg $ $Date: 2007/10/23 13:28:56 $
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
TARGET=bf_frm

NO_HIDS=TRUE

USE_DEFFILE=TRUE

# --- Settings ----------------------------------

.INCLUDE :	settings.mk
INC+= -I$(PRJ)$/inc$/bf_forms
LDUMP=ldump2.exe

# --- Library -----------------------------------

LIB1TARGET=$(SLB)$/bf_forms.lib
LIB1FILES=\
		$(SLB)$/forms_common.lib \
		$(SLB)$/forms_resource.lib \
		$(SLB)$/forms_component.lib

SHL1TARGET=$(TARGET)$(UPD)$(DLLPOSTFIX)

SHL1STDLIBS= \
		$(SALLIB) \
		$(OSLLIB) \
		$(ONELIB) \
		$(CPPULIB) \
		$(CPPUHELPERLIB) \
		$(TOOLSLIB) \
		$(I18NISOLANGLIB) \
		$(VCLLIB) \
		$(SVTOOLLIB) \
		$(SVLLIB)	\
		$(BFSVXLIB) \
		$(LEGACYSMGRLIB)	\
		$(VOSLIB) \
		$(UNOTOOLSLIB) \
		$(COMPHELPERLIB) \
		$(DBTOOLSLIB) \
        $(TKLIB) \
        $(BFSO3LIB)

SHL1LIBS=$(LIB1TARGET)
SHL1DEPN=	$(LIB1TARGET)	\
		makefile.mk


SHL1VERSIONMAP=$(TARGET).map 
SHL1DEF=$(MISC)$/$(SHL1TARGET).def
DEF1NAME=$(SHL1TARGET)

# === .res file ==========================================================

RES1FILELIST=\
	$(SRS)$/forms_resource.srs \

RESLIB1NAME=$(TARGET)
RESLIB1SRSFILES=$(RES1FILELIST)

.IF "$(GUI)"=="UNX"

# [ed] 6/19/02 Only add in libraries for X11 OS X builds

.IF "$(OS)"=="MACOSX"
.IF "$(GUIBASE)"=="unx"
SHL1STDLIBS +=\
        -lX11 -lXt -lXmu
.ENDIF
.ELSE
SHL1STDLIBS +=\
	-lX11
.ENDIF # OS == MACOSX

.ENDIF

# --- Targets ----------------------------------

.INCLUDE : target.mk

# --- Filter-Datei ---

$(MISC)$/$(SHL1TARGET).flt: makefile.mk
	@echo ------------------------------
	@echo Making: $@
	@echo CLEAR_THE_FILE			> $@
	@echo OControl					>> $@
	@echo OBoundControl				>> $@
	@echo OCombo					>> $@
	@echo OList						>> $@
	@echo OFixedText				>> $@
	@echo OCheckBox					>> $@
	@echo OGroupBox					>> $@
	@echo RadioButton				>> $@
	@echo OHidden					>> $@
	@echo OEdit						>> $@
	@echo OEditBase					>> $@
	@echo ONumeric					>> $@
	@echo OPattern					>> $@
	@echo OCurrency					>> $@
	@echo ODate						>> $@
	@echo OTime						>> $@
	@echo OFile						>> $@
	@echo OFormatted				>> $@
	@echo OComponent				>> $@
	@echo OButton					>> $@
	@echo OImage					>> $@
	@echo OInterfaceContainer		>> $@
	@echo OFormsCollection			>> $@
	@echo OGroup					>> $@
	@echo HtmlSuccess				>> $@
	@echo OSuccess					>> $@
	@echo OParameter				>> $@
	@echo ODatabaseForm				>> $@
	@echo OFormComponents			>> $@
	@echo OFormSubmitResetThread	>> $@
	@echo OGrid						>> $@
	@echo FieldColumn				>> $@
	@echo BoxColumn					>> $@
	@echo StandardFormatsSupplier	>> $@
	@echo OGuard					>> $@
	@echo OPropertyChange			>> $@
	@echo OEnumeration				>> $@
	@echo Weak						>> $@
	@echo OUString					>> $@
	@echo Any@uno@star@sun@com		>> $@
	@echo _C						>> $@
	@echo _TI2						>> $@
	@echo _real						>> $@


