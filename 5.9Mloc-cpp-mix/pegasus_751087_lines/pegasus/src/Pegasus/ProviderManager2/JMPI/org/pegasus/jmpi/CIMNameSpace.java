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
// Author:      Adrian Schuur, schuur@de.ibm.com
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////
package org.pegasus.jmpi;


public class CIMNameSpace {

   static public final int DEFAULT_PORT=5988;
   static public final String DEFAULT_NAMESPACE="root/cimv2";

   private long cInst;

   private native long   _new          ();
   private native long   _newHn        (String hn);
   private native long   _newHnNs      (String hn,
                                        String ns);
   private native String _getNameSpace (long   cInst);
   private native String _getHost      (long   cInst);
   private native void   _setNameSpace (long   cInst,
                                        String ns);
   private native void   _setHost      (long   cInst,
                                        String h);
   private native void   _finalize     (long   cInst);

   public CIMNameSpace ()
   {
      cInst=_new();
   }

   protected void finalize ()
   {
      _finalize(cInst);
   }

   protected long cInst ()
   {
      return cInst;
   }

   public CIMNameSpace (String host)
   {
      cInst=_newHn(host);
   }

   public CIMNameSpace (String host,
                        String ns)
   {
      cInst=_newHnNs(host,ns);
   }

   public String getNameSpace ()
   {
      if (cInst != 0)
      {
         return _getNameSpace (cInst);
      }
      else
      {
         return null;
      }
   }

   public String getHost ()
   {
      if (cInst != 0)
      {
         return _getHost (cInst);
      }
      else
      {
         return null;
      }
   }

   public void setNameSpace (String ns)
   {
      if (cInst != 0)
      {
         _setNameSpace (cInst, ns);
      }
   }

   public void setHost (String host)
   {
      if (cInst != 0)
      {
         _setHost (cInst, host);
      }
   }

   public int getPortNumber ()
   {
      return 0;
   }

   public String getProtocol ()
   {
      return null;
   }

   public String getHostURL ()
   {
      return null;
   }

   static {
      System.loadLibrary("JMPIProviderManager");
   }
}
