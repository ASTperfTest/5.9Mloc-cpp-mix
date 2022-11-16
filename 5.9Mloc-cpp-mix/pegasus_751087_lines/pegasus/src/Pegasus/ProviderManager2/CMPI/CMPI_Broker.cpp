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

#include "CMPI_Version.h"

#include "CMPI_Broker.h"
#include "CMPI_Object.h"
#include "CMPI_ContextArgs.h"
#include "CMPI_Enumeration.h"
#include "CMPI_Value.h"
#include "CMPIProviderManager.h"
#include "CMPI_String.h"

#include <Pegasus/Common/CIMName.h>
#include <Pegasus/Common/CIMPropertyList.h>
#include <Pegasus/Provider/CIMOMHandle.h>
#include <Pegasus/Common/CIMValue.h>
#include <Pegasus/Common/CIMType.h>


PEGASUS_USING_STD;
PEGASUS_NAMESPACE_BEGIN

static const CMPIUint32 MB_CAPABILITIES =
#   ifdef CMPI_VER_200
    CMPI_MB_Supports_Extended_Error |
#   endif
    CMPI_MB_BasicRead | CMPI_MB_BasicWrite | CMPI_MB_InstanceManipulation |
    CMPI_MB_AssociationTraversal | CMPI_MB_QueryNormalization |
    CMPI_MB_Indications | CMPI_MB_BasicQualifierSupport |
    CMPI_MB_OSEncapsulationSupport
#   ifndef PEGASUS_DISABLE_EXECQUERY
    | CMPI_MB_QueryExecution
#   endif
    ;

static CIMPropertyList getList(const char** l)
{
    CIMPropertyList pl;
    if (l)
    {
        Array<CIMName> n;
        while (*l)
        {
            n.append(*l++);
        }
        pl.set(n);
    }
    return pl;
}

CIMClass* mbGetClass(const CMPIBroker *mb, const CIMObjectPath &cop)
{
    PEG_METHOD_ENTER(TRC_CMPIPROVIDERINTERFACE, "CMPI_Broker:mbGetClass()");

    mb=CM_BROKER;
    CMPI_Broker *xBroker=(CMPI_Broker*)mb;
    String clsId =
        cop.getNameSpace().getString()+":"+cop.getClassName().getString();
    CIMClass *ccp;

    {
        ReadLock readLock (xBroker->rwsemClassCache);

        if (xBroker->clsCache->lookup(clsId,ccp))
        {
            PEG_METHOD_EXIT();
            return ccp;
        }
    }

    try
    {
        WriteLock writeLock (xBroker->rwsemClassCache);

        if (xBroker->clsCache->lookup(clsId,ccp))
        {
            PEG_METHOD_EXIT();
            return ccp;
        }

        CIMClass cc = CM_CIMOM(mb)->getClass(
            OperationContext(),
            cop.getNameSpace(),
            cop.getClassName(),
            (bool)0,
            (bool)1,
            (bool)0,
            CIMPropertyList());

        ccp = new CIMClass(cc);
        xBroker->clsCache->insert(clsId,ccp);
        PEG_METHOD_EXIT();
        return ccp;
    }
    catch (const CIMException &e)
    {
        PEG_TRACE_STRING(
            TRC_CMPIPROVIDERINTERFACE,
            Tracer::LEVEL2,
            "Exception: " + e.getMessage());
    }
    PEG_METHOD_EXIT();
    return NULL;
}

extern "C"
{

    static CMPIInstance* mbGetInstance(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char **properties,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbGetInstance()");

        mb = CM_BROKER;
        CMPIFlags flgs =
            ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
        const CIMPropertyList props = getList(properties);
        CIMObjectPath qop(
            String::EMPTY,
            CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            CIMInstance ci = CM_CIMOM(mb)->getInstance(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                qop, //*CM_ObjectPath(cop),
                CM_LocalOnly(flgs),
                CM_IncludeQualifiers(flgs),
                CM_ClassOrigin(flgs),
                props);

            ci.setPath(*CM_ObjectPath(cop));
            CMSetStatus(rc,CMPI_RC_OK);
            CMPIInstance* cmpiInst = reinterpret_cast<CMPIInstance*>(
                new CMPI_Object(new CIMInstance(ci)));
            PEG_METHOD_EXIT();
            return cmpiInst;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIObjectPath* mbCreateInstance(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const CMPIInstance *ci,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbCreateInstance()");

        mb = CM_BROKER;

        try
        {
            CIMObjectPath ncop = CM_CIMOM(mb)->createInstance(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                *CM_Instance(ci));
            CMSetStatus(rc,CMPI_RC_OK);
            CMPIObjectPath* cmpiObjPath = reinterpret_cast<CMPIObjectPath*>(
                new CMPI_Object(new CIMObjectPath(ncop)));
            PEG_METHOD_EXIT();
            return cmpiObjPath;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,(CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIStatus mbModifyInstance(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const CMPIInstance *ci,
        const char ** properties)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbModifyInstance()");
        mb = CM_BROKER;
        CMPIFlags flgs =
            ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
        const CIMPropertyList props = getList(properties);

        try
        {
            CIMInstance cmi(*CM_Instance(ci));
            cmi.setPath(*CM_ObjectPath(cop));
            CM_CIMOM(mb)->modifyInstance(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                cmi,
                CM_IncludeQualifiers(flgs),
                props);
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            PEG_METHOD_EXIT();
            CMReturnWithString(
                (CMPIrc)e.getCode(),
                (CMPIString*)string2CMPIString(e.getMessage()));
        }
        PEG_METHOD_EXIT();
        CMReturn(CMPI_RC_OK);
    }

    static CMPIStatus mbDeleteInstance(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbDeleteInstance()");
        mb = CM_BROKER;
        CIMObjectPath qop(
            String::EMPTY,CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            CM_CIMOM(mb)->deleteInstance(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                qop); //*CM_ObjectPath(cop));
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            PEG_METHOD_EXIT();
            CMReturnWithString(
                (CMPIrc)e.getCode(),
                (CMPIString*)string2CMPIString(e.getMessage()));
        }
        PEG_METHOD_EXIT();
        CMReturn(CMPI_RC_OK);
    }

    static CMPIEnumeration* mbExecQuery(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *query, const char *lang, CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbExecQuery()");
        mb = CM_BROKER;

        try
        {
            Array<CIMObject> const &en = CM_CIMOM(mb)->execQuery(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                String(lang),
                String(query));
                CMSetStatus(rc,CMPI_RC_OK);

            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*> (
                new CMPI_Object(
                new CMPI_ObjEnumeration(new Array<CIMObject>(en))));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbEnumInstances(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char **properties,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbEnumInstances()");
        mb = CM_BROKER;

        CMPIFlags flgs =
            ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
        const CIMPropertyList props = getList(properties);

        try
        {
            Array<CIMInstance> const &en =
                CM_CIMOM(mb)->enumerateInstances(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    CM_ObjectPath(cop)->getClassName(),
                    CM_DeepInheritance(flgs),
                    CM_LocalOnly(flgs),
                    CM_IncludeQualifiers(flgs),
                    CM_ClassOrigin(flgs),
                    props);

            CMSetStatus(rc,CMPI_RC_OK);

            // Workaround for bugzilla 4677
            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.

            Array<CIMInstance> * aInst = new Array<CIMInstance>(en);
            for (unsigned int index=0; index < aInst->size(); index++)
            {
                CIMInstance& myInst = (*aInst)[index];
                CIMObjectPath orgCop = myInst.getPath();
                orgCop.setNameSpace(CM_ObjectPath(cop)->getNameSpace());
                (*aInst)[index].setPath(orgCop);
            }

            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_InstEnumeration(aInst)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbEnumInstanceNames(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbEnumInstanceNames()");
        mb = CM_BROKER;

        try
        {
            Array<CIMObjectPath> const &en =
                CM_CIMOM(mb)->enumerateInstanceNames(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    CM_ObjectPath(cop)->getClassName());
                    CMSetStatus(rc,CMPI_RC_OK);

            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.
            Array<CIMObjectPath> * aObj = new Array<CIMObjectPath>(en);
            for (unsigned int index=0; index < aObj->size(); index++)
            {
                (*aObj)[index].setNameSpace(
                    CM_ObjectPath(cop)->getNameSpace());
            }
            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_OpEnumeration(aObj)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbAssociators(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *assocClass,
        const char *resultClass,
        const char *role,
        const char *resultRole,
        const char **properties,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbAssociators()");
        mb = CM_BROKER;
        //  ATTN-CAKG-P2-20020726:  The following condition does not correctly
        //  distinguish instanceNames from classNames in every case
        //  The instanceName of a singleton instance of a keyless class has no
        //  key bindings
        if (!CM_ObjectPath(cop)->getKeyBindings().size())
        {
            CMSetStatus(rc, CMPI_RC_ERR_FAILED);
            PEG_METHOD_EXIT();
            return 0;
        }
        CMPIFlags flgs =
            ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
        const CIMPropertyList props = getList(properties);
        CIMObjectPath qop(
            String::EMPTY,CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            Array<CIMObject> const &en =
                CM_CIMOM(mb)->associators(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    qop,
                    assocClass ? CIMName(assocClass) : CIMName(),
                    resultClass ? CIMName(resultClass) : CIMName(),
                    role ? String(role) : String::EMPTY,
                    resultRole ? String(resultRole) : String::EMPTY,
                    CM_IncludeQualifiers(flgs),
                    CM_ClassOrigin(flgs),
                    props);

            CMSetStatus(rc,CMPI_RC_OK);

            // Workaround for bugzilla 4677
            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.
            Array<CIMObject> * aInst = new Array<CIMObject>(en);
            for (unsigned int index=0; index < aInst->size(); index++)
            {
                CIMObject& myInst = (*aInst)[index];
                CIMObjectPath orgCop = myInst.getPath();
                orgCop.setNameSpace(CM_ObjectPath(cop)->getNameSpace());
                (*aInst)[index].setPath(orgCop);
            }
            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_ObjEnumeration(aInst)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbAssociatorNames(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *assocClass,
        const char *resultClass,
        const char *role,
        const char *resultRole,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbAssociatorNames()");
        mb = CM_BROKER;
        //  ATTN-CAKG-P2-20020726:  The following condition does not correctly
        //  distinguish instanceNames from classNames in every case
        //  The instanceName of a singleton instance of a keyless class has no
        //  key bindings
        if (!CM_ObjectPath(cop)->getKeyBindings().size())
        {
            CMSetStatus(rc, CMPI_RC_ERR_FAILED);
            PEG_METHOD_EXIT();
            return 0;
        }
        CIMObjectPath qop(
            String::EMPTY,CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            Array<CIMObjectPath> const &en =
                CM_CIMOM(mb)->associatorNames(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    qop,
                    assocClass ? CIMName(assocClass) : CIMName(),
                    resultClass ? CIMName(resultClass) : CIMName(),
                    role ? String(role) : String::EMPTY,
                    resultRole ? String(resultRole) : String::EMPTY);
                    CMSetStatus(rc,CMPI_RC_OK);

            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.
            Array<CIMObjectPath> * aObj = new Array<CIMObjectPath>(en);
            for (unsigned int index=0; index < aObj->size(); index++)
            {
                (*aObj)[index].setNameSpace(
                    CM_ObjectPath(cop)->getNameSpace());
            }
            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_OpEnumeration(aObj)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbReferences(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *resultClass,
        const char *role ,
        const char **properties,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbReferences()");
        mb = CM_BROKER;
        //  ATTN-CAKG-P2-20020726:  The following condition does not correctly
        //  distinguish instanceNames from classNames in every case
        //  The instanceName of a singleton instance of a keyless class has no
        //  key bindings
        if (!CM_ObjectPath(cop)->getKeyBindings().size())
        {
            CMSetStatus(rc, CMPI_RC_ERR_FAILED);
            PEG_METHOD_EXIT();
            return 0;
        }
        CMPIFlags flgs =
           ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
        CIMPropertyList props = getList(properties);
        CIMObjectPath qop(
            String::EMPTY,
            CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            Array<CIMObject> const &en =
                CM_CIMOM(mb)->references(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    qop,
                    resultClass ? CIMName(resultClass) : CIMName(),
                    role ? String(role) : String::EMPTY,
                    CM_IncludeQualifiers(flgs),
                    CM_ClassOrigin(flgs),
                    props);

            CMSetStatus(rc,CMPI_RC_OK);
            // Workaround for bugzilla 4677
            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.
            Array<CIMObject> * aInst = new Array<CIMObject>(en);
            for (unsigned int index=0; index < aInst->size(); index++)
            {
                CIMObject& myInst = (*aInst)[index];
                CIMObjectPath orgCop = myInst.getPath();
                orgCop.setNameSpace(CM_ObjectPath(cop)->getNameSpace());
                (*aInst)[index].setPath(orgCop);
            }
            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_ObjEnumeration(aInst)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

    static CMPIEnumeration* mbReferenceNames(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *resultClass,
        const char *role,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbReferenceNames()");
        mb = CM_BROKER;
        //  ATTN-CAKG-P2-20020726:  The following condition does not correctly
        //  distinguish instanceNames from classNames in every case
        //  The instanceName of a singleton instance of a keyless class has no
        //  key bindings
        if (!CM_ObjectPath(cop)->getKeyBindings().size())
        {
            CMSetStatus(rc, CMPI_RC_ERR_FAILED);
            PEG_METHOD_EXIT();
            return 0;
        }
        CIMObjectPath qop(
            String::EMPTY,
            CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            Array<CIMObjectPath> const &en =
                CM_CIMOM(mb)->referenceNames(
                    OperationContext(*CM_Context(ctx)),
                    CM_ObjectPath(cop)->getNameSpace(),
                    qop,
                    resultClass ? CIMName(resultClass) : CIMName(),
                    role ? String(role) : String::EMPTY);
                    CMSetStatus(rc,CMPI_RC_OK);

            // When running out of process the returned instances don't contain
            // a name space. Create a writable copy of the array and add the
            // namespace from the input parameters.
            Array<CIMObjectPath> * aObj = new Array<CIMObjectPath>(en);
            for (unsigned int index=0; index < aObj->size(); index++)
            {
                (*aObj)[index].setNameSpace(
                    CM_ObjectPath(cop)->getNameSpace());
            }
            CMPIEnumeration* cmpiEnum = reinterpret_cast<CMPIEnumeration*>(
                new CMPI_Object(new CMPI_OpEnumeration(aObj)));
            PEG_METHOD_EXIT();
            return cmpiEnum;
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
            PEG_METHOD_EXIT();
            return NULL;
        }
        // Code flow should never get here.
    }

#define CM_Args(args) ((Array<CIMParamValue>*)args->hdl)

    static CMPIData mbInvokeMethod(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *method,
        const CMPIArgs *in,
        CMPIArgs *out,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbInvokeMethod()");
        CMPIData data = {0,CMPI_nullValue,{0}};
        mb = CM_BROKER;
        CIMObjectPath qop(
            String::EMPTY,CIMNamespaceName(),
            CM_ObjectPath(cop)->getClassName(),
            CM_ObjectPath(cop)->getKeyBindings());

        try
        {
            CIMValue v = CM_CIMOM(mb)->invokeMethod(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                qop,
                method ? String(method) : String::EMPTY,
                *CM_Args(in),
                *CM_Args(out));
                CIMType vType=v.getType();
                CMPIType t = type2CMPIType(vType,v.isArray());
                value2CMPIData(v,t,&data);
            if (rc)
            {
                CMSetStatus(rc,CMPI_RC_OK);
            }
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            if (rc)
            {
                CMSetStatusWithString(
                    rc,
                    (CMPIrc)e.getCode(),
                    (CMPIString*)string2CMPIString(e.getMessage()));
            }
        }
        PEG_METHOD_EXIT();
        return data; // "data" will be valid data or nullValue (in error case)
    }

    static CMPIStatus mbSetProperty(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *name,
        const CMPIValue *val,
        CMPIType type)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbSetProperty()");
        mb = CM_BROKER;
        CMPIrc rc;
        CIMValue v = value2CIMValue(val,type,&rc);

        try
        {
            CM_CIMOM(mb)->setProperty(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                *CM_ObjectPath(cop),
                String(name),
                v);
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            PEG_METHOD_EXIT();
            CMReturnWithString(
                (CMPIrc)e.getCode(),
                (CMPIString*)string2CMPIString(e.getMessage()));
        }
        PEG_METHOD_EXIT();
        CMReturn(CMPI_RC_OK);
    }

    static CMPIData mbGetProperty(
        const CMPIBroker *mb,
        const CMPIContext *ctx,
        const CMPIObjectPath *cop,
        const char *name,
        CMPIStatus *rc)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbGetProperty()");
        mb = CM_BROKER;
        CMPIData data = {0,CMPI_nullValue,{0}};

        try
        {
            CIMValue v = CM_CIMOM(mb)->getProperty(
                OperationContext(*CM_Context(ctx)),
                CM_ObjectPath(cop)->getNameSpace(),
                *CM_ObjectPath(cop),
                String(name));
            CIMType vType = v.getType();
            CMPIType t = type2CMPIType(vType,v.isArray());
            value2CMPIData(v,t,&data);
            CMSetStatus(rc,CMPI_RC_OK);
        }
        catch (const CIMException &e)
        {
            PEG_TRACE_STRING(
                TRC_CMPIPROVIDERINTERFACE,
                Tracer::LEVEL2,
                "Exception: " + e.getMessage());
            CMSetStatus(rc,(CMPIrc)e.getCode());
        }
        PEG_METHOD_EXIT();
        return data; // "data" will be valid data or nullValue (in error case)
    }

    static CMPIContext* mbPrepareAttachThread(
        const CMPIBroker* mb,
        const CMPIContext* eCtx)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbPrepareAttachThread()");
        mb = CM_BROKER;
        OperationContext *ctx = (OperationContext*)((CMPI_Context*)eCtx)->ctx;
        OperationContext nctx = *ctx;
        CMPIContext* neCtx = new CMPI_Context(*(new OperationContext(nctx)));
        CMPIString *name;
        for (int i=0,s=CMPI_Args_Ftab->getArgCount(
            reinterpret_cast<const CMPIArgs*>(eCtx),NULL); i<s; i++)
        {
            CMPIData data = CMPI_Args_Ftab->getArgAt(
                reinterpret_cast<const CMPIArgs*>(eCtx),i,&name,NULL);
            CMPI_Args_Ftab->addArg(
                reinterpret_cast<CMPIArgs*>(neCtx),
                CMGetCharPtr(name),
                &data.value,data.type);
        }
        PEG_METHOD_EXIT();
        return neCtx;
    }

    static CMPIStatus mbAttachThread(
        const CMPIBroker* mb,
        const CMPIContext* eCtx)
    {
        ((CMPI_Context*)eCtx)->thr = new CMPI_ThreadContext(mb,eCtx);
        CMReturn(CMPI_RC_OK);
    }

    static CMPIStatus mbDetachThread(
        const CMPIBroker* mb,
        const CMPIContext* eCtx)
    {
        mb = CM_BROKER;
        CMPI_Context *neCtx = (CMPI_Context *)eCtx;
        delete neCtx->thr;
        // Delete also CMPIContext
        delete neCtx;
        CMReturn(CMPI_RC_OK);
    }

    static CMPIStatus mbDeliverIndication(
        const CMPIBroker* eMb,
        const CMPIContext* ctx,
        const char *ns,
        const CMPIInstance* ind)
    {
        PEG_METHOD_ENTER(
            TRC_CMPIPROVIDERINTERFACE,
            "CMPI_Broker:mbDeliverIndication()");
        eMb = CM_BROKER;
        CMPI_Broker *mb = (CMPI_Broker*)eMb;
        CMPIProviderManager::indProvRecord *prec;
        OperationContext* context = CM_Context(ctx);
        // When an indication to be delivered comes from Remote providers,
        // the CMPIBroker contains the name of the provider in the form
        // of physical-name:logical-name. Search using logical-name. -V 5884
        String provider_name;
        CMPIUint32 n;

        if ( (n = mb->name.find(':') ) != PEG_NOT_FOUND)
        {
            provider_name = mb->name.subString (n + 1);
        }
        else
        {
            provider_name = mb->name;
        }
        ReadLock readLock(CMPIProviderManager::rwSemProvTab);
        if (CMPIProviderManager::provTab.lookup(provider_name,prec))
        {
            if (prec->enabled)
            {
                try
                {
                    context->get(SubscriptionInstanceNamesContainer::NAME);
                }
                catch (const Exception &e)
                {
                    PEG_TRACE_STRING(
                        TRC_CMPIPROVIDERINTERFACE,
                        Tracer::LEVEL2,
                        "Exception: " + e.getMessage());
                    Array<CIMObjectPath> subscriptionInstanceNames;
                    context->insert(
                        SubscriptionInstanceNamesContainer(
                            subscriptionInstanceNames));
                }
                CIMIndication cimIndication(*CM_Instance(ind));
                try
                {
                    prec->handler->deliver(
                        *context,
   //                   OperationContext(*CM_Context(ctx)),
                        cimIndication);
                    PEG_METHOD_EXIT();
                    CMReturn(CMPI_RC_OK);
                }
                catch (const CIMException &e)
                {
                    PEG_TRACE_STRING(
                        TRC_CMPIPROVIDERINTERFACE,
                        Tracer::LEVEL2,
                        "Exception: " + e.getMessage());
                    PEG_METHOD_EXIT();
                    CMReturn((CMPIrc)e.getCode());
                }
            }
        }
        PEG_METHOD_EXIT();
        CMReturn(CMPI_RC_ERR_FAILED);
    }
}

static CMPIBrokerFT broker_FT =
{
    MB_CAPABILITIES, // brokerClassification;
    CMPICurrentVersion,
    "Pegasus",
    mbPrepareAttachThread,
    mbAttachThread,
    mbDetachThread,
    mbDeliverIndication,
    mbEnumInstanceNames,
    mbGetInstance,
    mbCreateInstance,
    mbModifyInstance,
    mbDeleteInstance,
    mbExecQuery,
    mbEnumInstances,
    mbAssociators,
    mbAssociatorNames,
    mbReferences,
    mbReferenceNames,
    mbInvokeMethod,
    mbSetProperty,
    mbGetProperty,
};

CMPIBrokerFT *CMPI_Broker_Ftab = & broker_FT;

PEGASUS_NAMESPACE_END

