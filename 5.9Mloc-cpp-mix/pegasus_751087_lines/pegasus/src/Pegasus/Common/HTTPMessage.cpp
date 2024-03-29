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

#include <Pegasus/Common/Config.h>
#include <iostream>
#include <cstring>
#include "HTTPMessage.h"
#include "ArrayIterator.h"

PEGASUS_USING_STD;

PEGASUS_NAMESPACE_BEGIN

static const String _HTTP_HEADER_CONTENT_TYPE = "content-type";

//------------------------------------------------------------------------------
//
// Implementation notes:
//
//     According to the HTTP specification:
//
//         1.  Method names are case-sensitive.
//         2.  Field names are case-insensitive.
//         3.  The first line of a message is known as the "start-line".
//         4.  Subsequent lines are known as headers.
//         5.  Headers have a field-name and field-value.
//         6.  Start-lines may be request-lines or status-lines. Request lines
//             have this form:
//
//             Request-Line = Method SP Request-URI SP HTTP-Version CRLF
//
//             Status-lines have this form:
//
//             Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
//
//------------------------------------------------------------------------------

char* HTTPMessage::findSeparator(const char* data, Uint32 size)
{
    const char* p = data;
    const char* end = p + size;

    while (p != end)
    {
        if (*p == '\r')
        {
            size_t n = end - p;

            if (n >= 2 && p[1] == '\n')
                return (char*)p;
        }
        else if (*p == '\n')
            return (char*)p;

        p++;
    }

    return 0;
}

void HTTPMessage::skipHeaderWhitespace(const char*& str)
{
    while (*str && (*str == ' ' || *str == '\t'))
    {
        ++str;
    }
}

Boolean HTTPMessage::expectHeaderToken(const char*& str, const char *token)
{
    PEGASUS_ASSERT(token);

    skipHeaderWhitespace(str);
    for ( ; *token ; ++str, ++token)
    {
        if (!*str || tolower(*str) != tolower(*token))
        {
            return false;
        }
    }
    return true;
}

HTTPMessage::HTTPMessage(
    const Buffer& message_,
    Uint32 queueId_, const CIMException *cimException_)
    :
    Message(HTTP_MESSAGE),
    message(message_),
    queueId(queueId_),
    authInfo(0),
    acceptLanguagesDecoded(false),
    contentLanguagesDecoded(false)
{
    if (cimException_)
        cimException = *cimException_;
}


HTTPMessage::HTTPMessage(const HTTPMessage & msg)
    : Base(msg)
{
    message = msg.message;
    queueId = msg.queueId;
    authInfo = msg.authInfo;
    acceptLanguages = msg.acceptLanguages;
    contentLanguages = msg.contentLanguages;
    acceptLanguagesDecoded = msg.acceptLanguagesDecoded;
    contentLanguagesDecoded = msg.contentLanguagesDecoded;
    cimException = msg.cimException;
}


void HTTPMessage::parse(
    String& startLine,
    Array<HTTPHeader>& headers,
    Uint32& contentLength) const
{
    startLine.clear();
    headers.clear();
    contentLength = 0;

    char* data = (char*)message.getData();
    Uint32 size = message.size();
    char* line = data;
    char* sep;
    Boolean firstTime = true;

    while ((sep = findSeparator(line, (Uint32)(size - (line - data)))))
    {
        // Look for double separator which terminates the header?

        if (line == sep)
        {
            // Establish pointer to content (account for "\n" and "\r\n").

            char* content = line + ((*sep == '\r') ? 2 : 1);

            // Determine length of content:

            contentLength = (Uint32)(message.size() - (content - data));
            break;
        }

        Uint32 lineLength = (Uint32)(sep - line);

        if (firstTime)
            startLine.assign(line, lineLength);
        else
        {
            // Find the colon:

            char* colon = 0;

            for (Uint32 i = 0; i < lineLength; i++)
            {
                if (line[i] == ':')
                {
                    colon = &line[i];
                    break;
                }
            }

            // This should always be true:

            if (colon)
            {
                // Get the name part:

                char* end;

                for (end = colon - 1; end > line && isspace(*end); end--)
                    ;

                end++;

                String name(line, (Uint32)(end - line));

                // Get the value part:

                char* start;

                for (start = colon + 1; start < sep && isspace(*start); start++)
                    ;

                String value(start, (Uint32)(sep - start));

                // From the HTTP/1.1 specification (RFC 2616) section 4.2
                // Message Headers:
                //
                // Multiple message-header fields with the same field-name
                // MAY be present in a message if and only if the entire
                // field-value for that header field is defined as a
                // comma-separated list [i.e., #(values)]. It MUST be
                // possible to combine the multiple header fields into one
                // "field-name: field-value" pair, without changing the
                // semantics of the message, by appending each subsequent
                // field-value to the first, each separated by a comma.  The
                // order in which header fields with the same field-name are
                // received is therefore significant to the interpretation
                // of the combined field value, and thus a proxy MUST NOT
                // change the order of these field values when a message is
                // forwarded.

                // This implementation concatenates duplicate header values,
                // with a comma separator.  If the resulting value is invalid,
                // that should be detected when the value is used.

                Uint32 headerIndex = 0;
                for (; headerIndex < headers.size(); headerIndex++)
                {
                    if (headers[headerIndex].first == name)
                    {
                        break;
                    }
                }

                if (headerIndex == headers.size())
                {
                    headers.append(HTTPHeader(name, value));
                    PEG_LOGGER_TRACE((
                        Logger::STANDARD_LOG, System::CIMSERVER, 0,
                        "HTTP header name: $0,  HTTP header value: $1",
                        name, value));
                }
                else
                {
                    headers[headerIndex].second.append(", ").append(value);
                    PEG_LOGGER_TRACE((
                        Logger::STANDARD_LOG, System::CIMSERVER, 0,
                        "HTTP header name: $0,  Updated HTTP header value: $1",
                        name, headers[headerIndex].second));
                }
            }
        }

        line = sep + ((*sep == '\r') ? 2 : 1);
        firstTime = false;
    }
}


#ifdef PEGASUS_DEBUG
void HTTPMessage::printAll(ostream& os) const
{
    Message::print(os);

    String startLine;
    Array<HTTPHeader> headers;
    Uint32 contentLength;
    parse(startLine, headers, contentLength);

    // get pointer to start of data.
    const char* content = message.getData() + message.size() - contentLength;
    // Print the first line:

    os << endl << startLine << endl;

    // Print the headers:

    Boolean image = false;

    for (Uint32 i = 0; i < headers.size(); i++)
    {
        cout << headers[i].first << ": " << headers[i].second << endl;

        if (String::equalNoCase(headers[i].first, _HTTP_HEADER_CONTENT_TYPE))
        {
            if (headers[i].second.find("image/") == 0)
                image = true;
        }
    }

    os << endl;

    // Print the content:

    for (Uint32 i = 0; i < contentLength; i++)
    {
        //char c = content[i];

        if (image)
        {
            if ((i % 60) == 0)
                os << endl;

            char c = content[i];

            if (c >= ' ' && c < '~')
                os << c;
            else
                os << '.';
        }
        else
            cout << content[i];
    }

    os << endl;
}
#endif

/*
 * Find the header prefix (i.e 2-digit number in front of cim keyword) if any.
 * If a fieldName is given it will use that, otherwise the FIRST field
 * starting with the standard keyword will be used. Given field names that do
 * not start with the standard keyword will never match.
 * if there is a keyword match, the prefix will be populated, else set to empty
 */

void HTTPMessage::lookupHeaderPrefix(
    Array<HTTPHeader>& headers_,
    const String& fieldName,
    String& prefix)
{
    ArrayIterator<HTTPHeader> headers(headers_);

    static const char keyword[] = "CIM";
    prefix.clear();

    for (Uint32 i = 0, n = headers.size(); i < n; i++)
    {
        const String &h = headers[i].first;

                if ((h.size() >= 3) &&
                    (h[0] >= '0') && (h[0] <= '9') &&
                    (h[1] >= '0') && (h[1] <= '9') &&
                    (h[2] == Char16('-')))
        {
            String fieldNameCurrent = h.subString(3);

            // ONLY fields starting with keyword can have prefixed according
            // to spec
            if (String::equalNoCase(fieldNameCurrent, keyword) == false)
                continue;

            prefix = h.subString(0,3);

            // no field name given, just return the first prefix encountered
            if (fieldName.size() == 0)
                break;

            if (String::equalNoCase(fieldNameCurrent, fieldName) == false)
                prefix.clear();
            else break;
        }
    }
}

Boolean HTTPMessage::lookupHeader(
    Array<HTTPHeader>& headers_,
    const String& fieldName,
    String& fieldValue,
    Boolean allowNamespacePrefix)
{
    ArrayIterator<HTTPHeader> headers(headers_);

    for (Uint32 i = 0, n = headers.size(); i < n; i++)
    {
        if (String::equalNoCase(headers[i].first, fieldName) ||
            (allowNamespacePrefix && (headers[i].first.size() >= 3) &&
             (headers[i].first[0] >= '0') && (headers[i].first[0] <= '9') &&
             (headers[i].first[1] >= '0') && (headers[i].first[1] <= '9') &&
             (headers[i].first[2] == Char16('-')) &&
             String::equalNoCase(headers[i].first.subString(3), fieldName)))
        {
            fieldValue = headers[i].second;
            return true;
        }
    }

    // Not found:
    return false;
}

Boolean HTTPMessage::parseRequestLine(
    const String& startLine,
    String& methodName,
    String& requestUri,
    String& httpVersion)
{
    // Request-Line = Method SP Request-URI SP HTTP-Version CRLF

    // Extract the method-name:

    Uint32 space1 = startLine.find(' ');

    if (space1 == PEG_NOT_FOUND)
        return false;

    methodName = startLine.subString(0, space1);

    // Extrat the request-URI:

    Uint32 space2 = startLine.find(space1 + 1, ' ');

    if (space2 == PEG_NOT_FOUND)
        return false;

    Uint32 uriPos = space1 + 1;

    requestUri = startLine.subString(uriPos, space2 - uriPos);

    // Extract the HTTP version:

    httpVersion = startLine.subString(space2 + 1);

    return true;
}

Boolean HTTPMessage::parseStatusLine(
    const String& statusLine,
    String& httpVersion,
    Uint32& statusCode,
    String& reasonPhrase)
{
    // Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    // Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF

    // Extract the HTTP version:

    Uint32 space1 = statusLine.find(' ');

    if (space1 == PEG_NOT_FOUND)
        return false;

    httpVersion = statusLine.subString(0, space1);

    // Extract the status code:

    Uint32 space2 = statusLine.find(space1 + 1, ' ');

    if (space2 == PEG_NOT_FOUND)
        return false;

    Uint32 statusCodePos = space1 + 1;
    String statusCodeStr;
    statusCodeStr = statusLine.subString(statusCodePos, space2 - statusCodePos);
    if (!sscanf(statusCodeStr.getCString(), "%u", &statusCode))
        return false;

    // Extract the reason phrase:

    reasonPhrase = statusLine.subString(space2 + 1);

    return true;
}

Boolean HTTPMessage::isSupportedContentType(const String& cimContentType)
{
    CString cstr = cimContentType.getCString();
    const char *str = (const char*) cstr;

    if (!expectHeaderToken(str, "application/xml"))
    {
        str = (const char*) cstr;
        if (!expectHeaderToken(str, "text/xml"))
        {
            return false;
        }
    }

    // Check for missing charset.
    skipHeaderWhitespace(str);
    if (!*str)
    {
        return true; //We assume "utf-8".
    }

    if (!expectHeaderToken(str, ";") || !expectHeaderToken(str, "charset") ||
        !expectHeaderToken (str, "="))
    {
        return false;
    }

    
    // charset is present, should be "utf-8" or utf-8, case insensitive.
    const char* strReset = str;
    if (!expectHeaderToken(str, "\"utf-8\""))//check for string "utf-8"
    {
        str = strReset;
        if(!expectHeaderToken(str, "utf-8"))//check for string utf-8
        {
            return false;  //If charset is neither "utf-8" nor utf-8
        }
    }

    // Check for any extra characters
    skipHeaderWhitespace(str);

    return !*str;
}

//
// parse the local authentication header
//
Boolean HTTPMessage::parseLocalAuthHeader(
    const String& authHeader,
    String& authType,
    String& userName,
    String& cookie)
{
    PEG_METHOD_ENTER(TRC_HTTP, "HTTPMessage::parseLocalAuthHeader()");

    //
    // Extract the authentication type:
    //
    Uint32 space = authHeader.find(' ');

    if ( space == PEG_NOT_FOUND )
    {
        PEG_METHOD_EXIT();
        return false;
    }

    authType = authHeader.subString(0, space);

    Uint32 startQuote = authHeader.find(space, '"');

    if ( startQuote == PEG_NOT_FOUND )
    {
        PEG_METHOD_EXIT();
        return false;
    }

    Uint32 endQuote = authHeader.find(startQuote + 1, '"');

    if ( endQuote == PEG_NOT_FOUND )
    {
        PEG_METHOD_EXIT();
        return false;
    }

    String temp = authHeader.subString(
        startQuote + 1, (endQuote - startQuote - 1));

    //
    // Extract the user name and cookie:
    //
    Uint32 colon = temp.find(0, ':');

    if ( colon == PEG_NOT_FOUND )
    {
        userName = temp;
    }
    else
    {
        userName = temp.subString(0, colon);
        cookie = temp;
    }

    PEG_METHOD_EXIT();

    return true;
}

//
// parse the HTTP authentication header
//
Boolean HTTPMessage::parseHttpAuthHeader(
    const String& authHeader, String& authTypeString, String& cookie)
{
    PEG_METHOD_ENTER(TRC_HTTP, "HTTPMessage::parseHttpAuthHeader()");

    //
    // Extract the authentication type:
    //
    Uint32 space = authHeader.find(' ');

    if ( space == PEG_NOT_FOUND )
    {
        PEG_METHOD_EXIT();
        return false;
    }

    authTypeString = authHeader.subString(0, space);

    //
    // Extract the cookie:
    //
    cookie = authHeader.subString(space + 1);

    PEG_METHOD_EXIT();

    return true;
}

PEGASUS_NAMESPACE_END
