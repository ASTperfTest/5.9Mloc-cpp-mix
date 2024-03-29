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

#include <iostream>
//#include <cstdio>
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/System.h>
#include <Pegasus/Common/AutoPtr.h>
#include "FileSystem.h"
#include "Dir.h"
#ifndef PEGASUS_OS_TYPE_WINDOWS
#include <pwd.h>
#endif
#include <Pegasus/Common/Tracer.h>
PEGASUS_NAMESPACE_BEGIN

// Clone the path as a C String but discard trailing slash if any:

static CString _clonePath(const String& path)
{
    String clone = path;

    if (clone.size() && clone[clone.size()-1] == '/')
        clone.remove(clone.size()-1);

    return clone.getCString();
}

Boolean FileSystem::exists(const String& path)
{
    return System::exists(_clonePath(path));
}

Boolean FileSystem::getCurrentDirectory(String& path)
{
    path.clear();
    char tmp[4096];

    if (!System::getCurrentDirectory(tmp, sizeof(tmp) - 1))
        return false;

    path.append(tmp);
    return true;
}

Boolean FileSystem::existsNoCase(const String& path, String& realPath)
{
    realPath.clear();
    CString cpath = _clonePath(path);
    const char* p = cpath;

    const char* dirPath;
    const char* fileName;
    char* slash = (char *) strrchr(p, '/');

    if (slash)
    {
        *slash = '\0';
        fileName = slash + 1;
        dirPath = p;

        if (*fileName == '\0')
            return false;
    }
    else
    {
        fileName = p;
        dirPath = ".";
    }


    for (Dir dir(dirPath); dir.more(); dir.next())
    {
        if (System::strcasecmp(fileName, dir.getName()) == 0)
        {
            if (strcmp(dirPath, ".") == 0)
                realPath = dir.getName();
            else
            {
                realPath = dirPath;
                realPath.append('/');
                realPath.append(dir.getName());
            }
            return true;
        }
    }

    return false;
}

Boolean FileSystem::canRead(const String& path)
{
    return System::canRead(_clonePath(path));
}

Boolean FileSystem::canWrite(const String& path)
{
    return System::canWrite(_clonePath(path));
}

Boolean FileSystem::getFileSize(const String& path, Uint32& size)
{
    return System::getFileSize(_clonePath(path), size);
}

Boolean FileSystem::removeFile(const String& path)
{
    return System::removeFile(_clonePath(path));
}

void FileSystem::loadFileToMemory(
    Buffer& array,
    const String& fileName)
{
    Uint32 fileSize;

    if (!getFileSize(fileName, fileSize))
        throw CannotOpenFile(fileName);

    FILE* fp = fopen(fileName.getCString(), "rb");

    if (fp == NULL)
        throw CannotOpenFile(fileName);

    array.reserveCapacity(fileSize);
    char buffer[4096];
    size_t n;

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        array.append(buffer, static_cast<Uint32>(n));

    fclose(fp);
}

Boolean FileSystem::compareFiles(
    const String& path1,
    const String& path2)
{
    Uint32 fileSize1;

    if (!getFileSize(path1, fileSize1))
        throw CannotOpenFile(path1);

    Uint32 fileSize2;

    if (!getFileSize(path2, fileSize2))
        throw CannotOpenFile(path2);

    if (fileSize1 != fileSize2)
        return false;

    FILE* fp1 = fopen(path1.getCString(), "rb");

    if (fp1 == NULL)
        throw CannotOpenFile(path1);

    FILE* fp2 = fopen(path2.getCString(), "rb");

    if (fp2 == NULL)
    {
        fclose(fp1);
        throw CannotOpenFile(path2);
    }

    int c1;
    int c2;

    while ((c1 = fgetc(fp1)) != EOF && (c2 = fgetc(fp2)) != EOF)
    {
        if (c1 != c2)
        {
            fclose(fp1);
            fclose(fp2);
            return false;
        }
    }

    fclose(fp1);
    fclose(fp2);
    return true;
}

Boolean FileSystem::renameFile(
    const String& oldPath,
    const String& newPath)
{
    return System::renameFile(oldPath.getCString(), newPath.getCString());
}

Boolean FileSystem::copyFile(
    const String& fromPath,
    const String& toPath)
{
    return System::copyFile(fromPath.getCString(), toPath.getCString());
}

Boolean FileSystem::openNoCase(PEGASUS_STD(ifstream)& is, const String& path)
{
    String realPath;

    if (!existsNoCase(path, realPath))
        return false;

    is.open(_clonePath(realPath) PEGASUS_IOS_BINARY);

    return !!is;
}

Boolean FileSystem::openNoCase(
    PEGASUS_STD(fstream)& fs,
    const String& path,
    int mode)
{
    String realPath;

    if (!existsNoCase(path, realPath))
        return false;
#if defined(__GNUC__) && GCC_VERSION >= 30200
    fs.open(_clonePath(realPath), PEGASUS_STD(ios_base::openmode)(mode));
#else
    fs.open(_clonePath(realPath), mode);
#endif
    return !!fs;
}

Boolean FileSystem::isDirectory(const String& path)
{
    return System::isDirectory(_clonePath(path));
}

Boolean FileSystem::changeDirectory(const String& path)
{
    return System::changeDirectory(_clonePath(path));
}

Boolean FileSystem::makeDirectory(const String& path)
{
    return System::makeDirectory(_clonePath(path));
}

Boolean FileSystem::removeDirectory(const String& path)
{
    return System::removeDirectory(_clonePath(path));
}

Boolean FileSystem::removeDirectoryHier(const String& path)
{
    Array<String> fileList;

    // Get contents of current directory

    if (!FileSystem::getDirectoryContents(path,fileList))
        return false;

    // for files-in-directory, delete or recall removedir

    for (Uint32 i = 0, n = fileList.size(); i < n; i++)
    {
        String newPath = path;   // extend path to subdir
        newPath.append("/");
        newPath.append(fileList[i]);

        if (FileSystem::isDirectory(newPath))
        {
            // Recall ourselves with extended path
            if (!FileSystem::removeDirectoryHier(newPath))
                return false;
        }

        else
        {
          if (!FileSystem::removeFile(newPath))
                return false;
        }
    }

    return removeDirectory(path);
}

//
//  Get the file list in the directory into the
//  array of strings provided
//  @return The function should return false under these circumstances:
//
//
//  1. The directory does not exist.
//  2. The file exists but is not a directory.
//  3. The directory is inaccessible.
//
//
Boolean FileSystem::getDirectoryContents(
    const String& path,
    Array<String>& paths)
{
    paths.clear();

    try
    {
        for (Dir dir(path); dir.more(); dir.next())
        {
            String name = dir.getName();

            if (String::equal(name, ".") || String::equal(name, ".."))
                continue;

            paths.append(name);
        }
        return true;
    }

    // Catch the Dir exception
    catch (CannotOpenDirectory&)
    {
        return false;
    }
}

Boolean FileSystem::isDirectoryEmpty(const String& path)
{
    for (Dir dir(path); dir.more(); dir.next())
    {
        const char* name = dir.getName();

        if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
            return false;
    }

    return true;
}

void FileSystem::translateSlashes(String& path)
{
    for (Uint32 i = 0; i < path.size(); i++)
    {
        if (path[i] == '\\')
            path[i] = '/';
    }
}

// Return the just the base name from the path.
String FileSystem::extractFileName(const String& path)
{
    AutoArrayPtr<char> p_path(new char[path.size() + 1]);
    String basename = System::extract_file_name(
        (const char*)path.getCString(), p_path.get());

    return basename;
}

// Return just the path to the file or directory into path
String FileSystem::extractFilePath(const String& path)
{
    AutoArrayPtr<char> p_path(new char[path.size() + 1]);
    String newpath = System::extract_file_path(
        (const char*)path.getCString(), p_path.get());

    return newpath;
}

// Changes file permissions on the given file.
Boolean FileSystem::changeFilePermissions(const String& path, mode_t mode)
{
    CString tempPath = path.getCString();

    return System::changeFilePermissions(tempPath, mode);
}

String FileSystem::getAbsoluteFileName(
    const String& paths,
    const String& filename)
{
    Uint32 pos = 0;
    Uint32 token = 0;
    String path;
    String root;
    String tempPath = paths;

    do
    {
        if ((pos = tempPath.find(FileSystem::getPathDelimiter())) ==
            PEG_NOT_FOUND)
        {
            pos = tempPath.size();
            token = 0;
        }
        else
        {
            token = 1;
        }
        path = tempPath.subString(0, pos);
        tempPath.remove(0,pos+token);
        if (FileSystem::exists(path + "/" + filename) == true)
        {
            root = path + "/" + filename;
            break;
        }
        else
        {
            //  cout << "File does not exist.\n";
        }
    } while (tempPath.size() > 0);

    return root;
}

String FileSystem::buildLibraryFileName(const String &libraryName)
{
    String fileName;

    //
    // Add the necessary prefix and suffix to convert the library name to its
    // corresponding file name.
    //
#if defined(PEGASUS_OS_TYPE_WINDOWS)
    fileName = libraryName + String(".dll");
#elif defined(PEGASUS_PLATFORM_HPUX_PARISC_ACC)
    fileName = String("lib") + libraryName + String(".sl");
#elif defined(PEGASUS_OS_DARWIN)
    fileName = String("lib") + libraryName + String(".dylib");
#elif defined(PEGASUS_OS_VMS)
    fileName = String("lib") + libraryName + String(".exe");
#else
    fileName = String("lib") + libraryName + String(".so");
#endif

    return fileName;
}


Boolean GetLine(PEGASUS_STD(istream)& is, String& line)
{
    line.clear();

    Boolean gotChar = false;
#ifdef PEGASUS_PLATFORM_ZOS_ZSERIES_IBM
    char input[1000];
    is.getline(input,1000);
    line.assign(input);

    gotChar = !(is.rdstate() & PEGASUS_STD(istream)::failbit);
#else
    char c;

    while (is.get(c))
    {
        gotChar = true;

        if (c == '\n')
            break;

        line.append(c);
    }
#endif

    return gotChar;
}

//
// changes the file owner to one specified
//
Boolean FileSystem::changeFileOwner(
    const String& fileName,
    const String& userName)
{
#if defined(PEGASUS_OS_TYPE_WINDOWS)

    return true;

#else

    PEG_METHOD_ENTER(TRC_AUTHENTICATION, "FileSystem::changeFileOwner()");

    struct passwd* userPasswd;
#if defined(PEGASUS_PLATFORM_SOLARIS_SPARC_CC) || \
    defined(PEGASUS_OS_HPUX) || \
    defined(PEGASUS_OS_LINUX) || \
    defined (PEGASUS_OS_VMS)

    const unsigned int PWD_BUFF_SIZE = 1024;
    struct passwd pwd;
    struct passwd *result;
    char pwdBuffer[PWD_BUFF_SIZE];

    if (getpwnam_r(userName.getCString(), &pwd, pwdBuffer, PWD_BUFF_SIZE,
                  &userPasswd) != 0)
    {
        userPasswd = (struct passwd*)NULL;
    }

#else

    userPasswd = getpwnam(userName.getCString());
#endif

    if (userPasswd  == NULL)
    {
        PEG_METHOD_EXIT();
        return false;
    }

    Sint32 ret = chown(
        fileName.getCString(), userPasswd->pw_uid, userPasswd->pw_gid);
        
    if (ret == -1)
    {
        PEG_METHOD_EXIT();
        return false;
    }

    PEG_METHOD_EXIT();

    return true;
#endif
}

void FileSystem::syncWithDirectoryUpdates(PEGASUS_STD(fstream)& fs)
{
#if defined(PEGASUS_OS_HPUX)
    // Writes the data from the iostream buffers to the OS buffers
    fs.flush();
    // Writes the data from the OS buffers to the disk
    fsync(fs.rdbuf()->fd());
#endif
}

PEGASUS_NAMESPACE_END
