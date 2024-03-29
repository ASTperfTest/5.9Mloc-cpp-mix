#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.6 $
#
#   last change: $Author: kz $ $Date: 2006/07/19 13:13:25 $
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

PRJ=..$/..$/..
BFPRJ=..$/..

PRJNAME=binfilter
TARGET=forms_component

#GEN_HID=TRUE
#GEN_HID_OTHER=TRUE
NO_HIDS=TRUE

ENABLE_EXCEPTIONS=TRUE

# --- Settings ----------------------------------

.INCLUDE : settings.mk
INC+= -I$(PRJ)$/inc$/bf_forms
# --- Types -------------------------------------

INCPRE+=$(SOLARINCDIR)$/offuh

# --- Files -------------------------------------

CXXFILES=	forms_Grid.cxx	\
			forms_Columns.cxx\
			forms_DatabaseForm.cxx \
			forms_GroupManager.cxx \
			forms_FormsCollection.cxx \
			forms_EventThread.cxx \
			forms_File.cxx \
			forms_Edit.cxx \
			forms_EditBase.cxx \
			forms_Numeric.cxx \
			forms_Pattern.cxx \
			forms_Currency.cxx \
			forms_Date.cxx \
			forms_Time.cxx \
			forms_FormattedField.cxx \
			forms_FormattedFieldWrapper.cxx \
			forms_ListBox.cxx \
			forms_ComboBox.cxx \
			forms_GroupBox.cxx \
			forms_Hidden.cxx \
			forms_CheckBox.cxx \
			forms_RadioButton.cxx \
			forms_FixedText.cxx \
			forms_FormComponent.cxx \
			forms_Image.cxx \
			forms_Button.cxx \
			forms_ImageButton.cxx \
			forms_ImageControl.cxx \
			forms_cloneable.cxx \
			forms_imgprod.cxx

.IF "$(COM)"=="GCC"
NOOPTFILES= $(SLO)$/forms_EventThread.obj
.ENDIF

SLOFILES=	$(SLO)$/forms_Grid.obj \
			$(SLO)$/forms_Columns.obj \
			$(SLO)$/forms_DatabaseForm.obj \
			$(SLO)$/forms_GroupManager.obj \
			$(SLO)$/forms_FormsCollection.obj \
			$(SLO)$/forms_EventThread.obj \
			$(SLO)$/forms_File.obj \
			$(SLO)$/forms_Edit.obj \
			$(SLO)$/forms_EditBase.obj \
			$(SLO)$/forms_Numeric.obj \
			$(SLO)$/forms_Pattern.obj \
			$(SLO)$/forms_Currency.obj \
			$(SLO)$/forms_Date.obj \
			$(SLO)$/forms_Time.obj \
			$(SLO)$/forms_FormattedField.obj \
			$(SLO)$/forms_FormattedFieldWrapper.obj \
			$(SLO)$/forms_ListBox.obj \
			$(SLO)$/forms_ComboBox.obj \
			$(SLO)$/forms_GroupBox.obj \
			$(SLO)$/forms_Hidden.obj \
			$(SLO)$/forms_CheckBox.obj \
			$(SLO)$/forms_RadioButton.obj \
			$(SLO)$/forms_FixedText.obj \
			$(SLO)$/forms_FormComponent.obj \
			$(SLO)$/forms_Image.obj \
			$(SLO)$/forms_Button.obj \
			$(SLO)$/forms_ImageButton.obj \
			$(SLO)$/forms_ImageControl.obj \
			$(SLO)$/forms_errorbroadcaster.obj \
			$(SLO)$/forms_cloneable.obj \
			$(SLO)$/forms_Filter.obj \
			$(SLO)$/forms_imgprod.obj

# --- Targets ----------------------------------

.INCLUDE : target.mk

