//%2006////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2006 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; Symantec Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
// Author:      Mark Hamzy,    hamzy@us.ibm.com
//
// Modified By: Mark Hamzy,    hamzy@us.ibm.com
//
//%/////////////////////////////////////////////////////////////////////////////
package org.pegasus.jmpi;

import java.util.Enumeration;

public class JMPISelectList
             extends SelectList
{
   private long ciSelectExp;

   private native long _applyInstance (long ciSelectExp, long ciInstance);
   private native long _applyClass    (long ciSelectExp, long ciClass);

   JMPISelectList (long ciSelectExp)
   {
      this.ciSelectExp = ciSelectExp;
   }

///public void addElement (AttributeExp ae)
///{
///}

   public Enumeration elements ()
   {
      return null;
   }

   public CIMElement apply (CIMElement elm)
   {
      if (elm instanceof CIMInstance)
      {
         CIMInstance ci     = (CIMInstance)elm;
         long        ciInst = 0;

         ciInst = _applyInstance (ciSelectExp, ci.cInst ());

         if (ciInst != 0)
         {
            return new CIMInstance (ciInst);
         }
      }
      else if (elm instanceof CIMClass)
      {
         CIMClass cc      = (CIMClass)elm;
         long     ciClass = 0;

         ciClass = _applyClass (ciSelectExp, cc.cInst ());

         if (ciClass != 0)
         {
            return new CIMClass (ciClass);
         }
      }

      return null;
   }

   static {
      System.loadLibrary("JMPIProviderManager");
   }
}
