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
//%/////////////////////////////////////////////////////////////////////////////

// Please be aware that the CMPI C++ API is NOT a standard currently.

#ifndef CWS_PLAINFILE_H
#define CWS_PLAINFILE_H

#include <Pegasus/Provider/CMPI/CmpiInstanceMI.h>
#include <Pegasus/Provider/CMPI/CmpiMethodMI.h>

class CWS_PlainFile : public CmpiInstanceMI, public CmpiMethodMI
{
 public:
  
  CWS_PlainFile(const CmpiBroker &mbp, const CmpiContext& ctx);

  virtual ~CWS_PlainFile();
	
  virtual int isUnloadable() const;
	
  virtual CmpiStatus enumInstanceNames(const CmpiContext& ctx, 
				       CmpiResult& rslt,
				       const CmpiObjectPath& cop);
     
  virtual CmpiStatus enumInstances(const CmpiContext& ctx, CmpiResult& rslt,
				   const CmpiObjectPath& cop,
				   const char* *properties);
  
  virtual CmpiStatus getInstance(const CmpiContext& ctx, CmpiResult& rslt,
				 const CmpiObjectPath& cop,
				 const char* *properties);
  
  virtual CmpiStatus createInstance(const CmpiContext& ctx, 
				    CmpiResult& rslt,
				    const CmpiObjectPath& cop,
				    const CmpiInstance& inst);
  
  virtual CmpiStatus setInstance(const CmpiContext& ctx, CmpiResult& rslt,
				 const CmpiObjectPath& cop,
				 const CmpiInstance& inst,
				 const char* *properties);
  
  virtual CmpiStatus deleteInstance(const CmpiContext& ctx, CmpiResult& rslt,
				    const CmpiObjectPath& cop);

  virtual CmpiStatus invokeMethod(const CmpiContext& ctx, CmpiResult& rslt,
				  const CmpiObjectPath& cop, 
				  const char *method,
				  const CmpiArgs& in, CmpiArgs& out);
 private:
  CmpiBroker cppBroker;

};

#endif
