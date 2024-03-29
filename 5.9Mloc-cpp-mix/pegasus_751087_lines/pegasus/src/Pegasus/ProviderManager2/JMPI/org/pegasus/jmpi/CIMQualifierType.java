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
// Author:      Adrian Duta
//
// Modified By: Adrian Schuur, schuur@de.ibm.com
//              Mark Hamzy, hamzy@us.ibm.com
//
//%/////////////////////////////////////////////////////////////////////////////
package org.pegasus.jmpi;

public class CIMQualifierType
{
   private long cInst;

   private native long   _new      ();
   private native String _getName  (long   qt);
   private native long   _setName  (long   qt,
                                    String n);
   private native void   _setValue (long   qt,
                                    long   v);
   private native void   _finalize (long   qt);

   protected void finalize ()
   {
      _finalize(cInst);
   }

   protected long cInst ()
   {
      return cInst;
   }

   CIMQualifierType (long qt)
   {
      cInst=qt;
   }

   public CIMQualifierType ()
   {
      cInst=_new();
   }

   public String getName ()
   {
      if (cInst != 0)
      {
         return _getName (cInst);
      }
      else
      {
         return null;
      }
   }

   public void setName (String n)
   {
      if (cInst != 0)
      {
         cInst = _setName (cInst, n);
      }
   }

   public void setValue (CIMValue v)
   {
      if (cInst != 0)
      {
         _setValue (cInst, v.cInst ());
      }
   }

   public void setDefaultValue (CIMValue v)
   {
   }

   public void setType (CIMDataType t)
   {
   }

   public void addFlavor (CIMFlavor t)
   {
   }

   public void addScope (CIMScope t)
   {
   }

   static {
      System.loadLibrary("JMPIProviderManager");
   }
}
