// \file props.cxx
// Property server class.
//
// Written by Curtis Olson, started September 2000.
// Modified by Bernie Bright, May 2002.
//
// Copyright (C) 2000  Curtis L. Olson - http://www.flightgear.org/~curt
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// $Id: props.cxx,v 1.18.2.2 2007-02-20 21:12:44 mfranz Exp $


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <simgear/compiler.h>
#include <simgear/debug/logstream.hxx>
#include <simgear/structure/commands.hxx>
#include <simgear/misc/strutils.hxx>
#include <simgear/props/props.hxx>
#include <simgear/props/props_io.hxx>

#include <sstream>

#include <Main/globals.hxx>
#include <Main/viewmgr.hxx>

#include <plib/netChat.h>

#include "props.hxx"

SG_USING_STD(stringstream);
SG_USING_STD(ends);

/**
 * Props connection class.
 * This class represents a connection to props client.
 */
class PropsChannel : public netChat
{
    netBuffer buffer;

    /**
     * Current property node name.
     */
    string path;

    enum Mode {
	PROMPT,
	DATA
    };
    Mode mode;

public:
    /**
     * Constructor.
     */
    PropsChannel();
    
    /**
     * Append incoming data to our request buffer.
     *
     * @param s Character string to append to buffer
     * @param n Number of characters to append.
     */
    void collectIncomingData( const char* s, int n );

    /**
     * Process a complete request from the props client.
     */
    void foundTerminator();

private:
    /**
     * Return a "Node no found" error message to the client.
     */
    void node_not_found_error( const string& node_name );
};

/**
 * 
 */
PropsChannel::PropsChannel()
    : buffer(512),
      path("/"),
      mode(PROMPT)
{
    setTerminator( "\r\n" );
}

/**
 * 
 */
void
PropsChannel::collectIncomingData( const char* s, int n )
{
    buffer.append( s, n );
}

/**
 * 
 */
void
PropsChannel::node_not_found_error( const string& node_name )
{
    string error = "-ERR Node \"";
    error += node_name;
    error += "\" not found.";
    push( error.c_str() );
    push( getTerminator() );
}

// return a human readable form of the value "type"
static string
getValueTypeString( const SGPropertyNode *node )
{
    string result;

    if ( node == NULL )
    {
	return "unknown";
    }

    SGPropertyNode::Type type = node->getType();
    if ( type == SGPropertyNode::UNSPECIFIED ) {
	result = "unspecified";
    } else if ( type == SGPropertyNode::NONE ) {
        result = "none";
    } else if ( type == SGPropertyNode::BOOL ) {
	result = "bool";
    } else if ( type == SGPropertyNode::INT ) {
	result = "int";
    } else if ( type == SGPropertyNode::LONG ) {
	result = "long";
    } else if ( type == SGPropertyNode::FLOAT ) {
	result = "float";
    } else if ( type == SGPropertyNode::DOUBLE ) {
	result = "double";
    } else if ( type == SGPropertyNode::STRING ) {
	result = "string";
    }

    return result;
}

/**
 * We have a command.
 * 
 */
void
PropsChannel::foundTerminator()
{
    const char* cmd = buffer.getData();
    SG_LOG( SG_IO, SG_INFO, "processing command = \"" << cmd << "\"" );

    vector<string> tokens = simgear::strutils::split( cmd );

    SGPropertyNode* node = globals->get_props()->getNode( path.c_str() );

    if (!tokens.empty()) {
	string command = tokens[0];

	if (command == "ls") {
	    SGPropertyNode* dir = node;
	    if (tokens.size() == 2) {
		if (tokens[1][0] == '/') {
		    dir = globals->get_props()->getNode( tokens[1].c_str() );
		} else {
		    string s = path;
		    s += "/";
		    s += tokens[1];
		    dir = globals->get_props()->getNode( s.c_str() );
		}

		if (dir == 0) {
		    node_not_found_error( tokens[1] );
		    goto prompt;
		}
	    }

	    for (int i = 0; i < dir->nChildren(); i++) {
		SGPropertyNode * child = dir->getChild(i);
		string line = child->getDisplayName(true);

		if ( child->nChildren() > 0 ) {
		    line += "/";
		} else {
		    if (mode == PROMPT) {
			string value = child->getStringValue();
			line += " =\t'" + value + "'\t(";
			line += getValueTypeString( child );
			line += ")";
		    }
		}

		line += getTerminator();
		push( line.c_str() );
	    }
	} else if ( command == "dump" ) {
	    stringstream buf;
	    if ( tokens.size() <= 1 ) {
		writeProperties( buf, node );
		buf << ends; // null terminate the string
		push( buf.str().c_str() );
		push( getTerminator() );
	    } else {
		SGPropertyNode *child = node->getNode( tokens[1].c_str() );
		if ( child ) {
		    writeProperties ( buf, child );
		    buf << ends; // null terminate the string
		    push( buf.str().c_str() );
		    push( getTerminator() );
		} else {
		    node_not_found_error( tokens[1] );
		}
	    }
	}
	else if ( command == "cd" ) {
	    if (tokens.size() == 2) {
		try {
		    SGPropertyNode* child = node->getNode( tokens[1].c_str() );
		    if ( child ) {
			node = child;
			path = node->getPath();
		    } else {
			node_not_found_error( tokens[1] );
		    }
		} catch (...) {
		    // Ignore attempt to move past root node with ".."
		}
	    }
	} else if ( command == "pwd" ) {
	    string ttt = node->getPath();
	    if (ttt.empty()) {
		ttt = "/";
	    }

	    push( ttt.c_str() );
	    push( getTerminator() );
	} else if ( command == "get" || command == "show" ) {
	    if ( tokens.size() == 2 ) {
		string tmp;	
		string value = node->getStringValue ( tokens[1].c_str(), "" );
		if ( mode == PROMPT ) {
		    tmp = tokens[1];
		    tmp += " = '";
		    tmp += value;
		    tmp += "' (";
		    tmp += getValueTypeString(
				     node->getNode( tokens[1].c_str() ) );
		    tmp += ")";
		} else {
		    tmp = value;
		}
		push( tmp.c_str() );
		push( getTerminator() );
	    }
	} else if ( command == "set" ) {
	    if ( tokens.size() >= 2 ) {
                string value, tmp;
                for (unsigned int i = 2; i < tokens.size(); i++) {
                    if (i > 2)
                        value += " ";
                    value += tokens[i];
                }
                node->getNode( tokens[1].c_str(), true )
                    ->setStringValue(value.c_str());

		if ( mode == PROMPT ) {
		    // now fetch and write out the new value as confirmation
		    // of the change
		    value = node->getStringValue ( tokens[1].c_str(), "" );
		    tmp = tokens[1] + " = '" + value + "' (";
		    tmp += getValueTypeString( node->getNode( tokens[1].c_str() ) );
		    tmp += ")";
		    push( tmp.c_str() );
		    push( getTerminator() );
		}
	    } 
	} else if ( command == "reinit" ) {
	    if ( tokens.size() == 2 ) {
		string tmp;	
                SGPropertyNode args;
                for ( unsigned int i = 1; i < tokens.size(); ++i ) {
                    cout << "props: adding subsystem = " << tokens[i] << endl;
                    SGPropertyNode *node = args.getNode("subsystem", i-1, true);
                    node->setStringValue( tokens[i].c_str() );
                }
                if ( !globals->get_commands()
                         ->execute( "reinit", &args) )
                {
                    SG_LOG( SG_GENERAL, SG_ALERT,
                            "Command " << tokens[1] << " failed.");
                    if ( mode == PROMPT ) {
                        tmp += "*failed*";
                        push( tmp.c_str() );
                        push( getTerminator() );
                    }
                } else {
                    if ( mode == PROMPT ) {
                        tmp += "<completed>";
                        push( tmp.c_str() );
                        push( getTerminator() );
                    }
                }
	    }
	} else if ( command == "run" ) {
            string tmp;	
            if ( tokens.size() >= 2 ) {
                SGPropertyNode args;
                if ( tokens[1] == "reinit" ) {
                    for ( unsigned int i = 2; i < tokens.size(); ++i ) {
                        cout << "props: adding subsystem = " << tokens[i]
                             << endl;
                        SGPropertyNode *node
                            = args.getNode("subsystem", i-2, true);
                        node->setStringValue( tokens[i].c_str() );
                    }
                } else if ( tokens[1] == "set-sea-level-air-temp-degc" ) {
                    for ( unsigned int i = 2; i < tokens.size(); ++i ) {
                        cout << "props: set-sl command = " << tokens[i]
                             << endl;
                        SGPropertyNode *node
                            = args.getNode("temp-degc", i-2, true);
                        node->setStringValue( tokens[i].c_str() );
                    }
                } else if ( tokens[1] == "set-outside-air-temp-degc" ) {
                    for ( unsigned int i = 2; i < tokens.size(); ++i ) {
                        cout << "props: set-oat command = " << tokens[i]
                             << endl;
                        SGPropertyNode *node
                            = args.getNode("temp-degc", i-2, true);
                        node->setStringValue( tokens[i].c_str() );
                    }
                } else if ( tokens[1] == "timeofday" ) {
                    for ( unsigned int i = 2; i < tokens.size(); ++i ) {
                        cout << "props: time of day command = " << tokens[i]
                             << endl;
                        SGPropertyNode *node
                            = args.getNode("timeofday", i-2, true);
                        node->setStringValue( tokens[i].c_str() );
                    }
                } else if ( tokens[1] == "play-audio-message" ) {
                    if ( tokens.size() == 4 ) {
                        cout << "props: play audio message = " << tokens[2]
                             << " " << tokens[3] << endl;
                        SGPropertyNode *node;
                        node = args.getNode("path", 0, true);
                        node->setStringValue( tokens[2].c_str() );
                        node = args.getNode("file", 0, true);
                        node->setStringValue( tokens[3].c_str() );
                   }
                }
                if ( !globals->get_commands()
                         ->execute(tokens[1].c_str(), &args) )
                {
                    SG_LOG( SG_GENERAL, SG_ALERT,
                            "Command " << tokens[1] << " failed.");
                    if ( mode == PROMPT ) {
                        tmp += "*failed*";
                        push( tmp.c_str() );
                        push( getTerminator() );
                    }
                } else {
                    if ( mode == PROMPT ) {
                        tmp += "<completed>";
                        push( tmp.c_str() );
                        push( getTerminator() );
                    }
                }
	    } else {
                if ( mode == PROMPT ) {
                    tmp += "no command specified";
                    push( tmp.c_str() );
                    push( getTerminator() );
                }
            }
	} else if (command == "quit") {
	    close();
	    shouldDelete();
	    return;
	} else if ( command == "data" ) {
	    mode = DATA;
	} else if ( command == "prompt" ) {
	    mode = PROMPT;
	} else {
	    const char* msg = "\
Valid commands are:\r\n\
\r\n\
cd <dir>           cd to a directory, '..' to move back\r\n\
data               switch to raw data mode\r\n\
dump               dump current state (in xml)\r\n\
get <var>          show the value of a parameter\r\n\
help               show this help message\r\n\
ls [<dir>]         list directory\r\n\
prompt             switch to interactive mode (default)\r\n\
pwd                display your current path\r\n\
quit               terminate connection\r\n\
run <command>      run built in command\r\n\
set <var> <val>    set <var> to a new <val>\r\n";
	    push( msg );
	}
    }

 prompt:
    if (mode == PROMPT) {
	string prompt = node->getPath();
	if (prompt.empty()) {
	    prompt = "/";
	}
	prompt += "> ";
	push( prompt.c_str() );
    }

    buffer.remove();
}

/**
 * 
 */
FGProps::FGProps( const vector<string>& tokens )
{
    // tokens:
    //   props,port#
    //   props,medium,direction,hz,hostname,port#,style
    if (tokens.size() == 2) {
	port = atoi( tokens[1].c_str() );
        set_hz( 5 );                // default to processing requests @ 5Hz
    } else if (tokens.size() == 7) {
        char* endptr;
        errno = 0;
        int hz = strtol( tokens[3].c_str(), &endptr, 10 );
        if (errno != 0) {
           SG_LOG( SG_IO, SG_ALERT, "I/O poll frequency out of range" );
           set_hz( 5 );           // default to processing requests @ 5Hz
        } else {
            SG_LOG( SG_IO, SG_INFO, "Setting I/O poll frequency to "
                    << hz << " Hz");
            set_hz( hz );
        }
	port = atoi( tokens[5].c_str() );
    } else {
	throw FGProtocolConfigError( "FGProps: incorrect number of configuration arguments" );
    }
}

/**
 * 
 */
FGProps::~FGProps()
{
}

/**
 * 
 */
bool
FGProps::open()
{
    if ( is_enabled() )
    {
	SG_LOG( SG_IO, SG_ALERT, "This shouldn't happen, but the channel " 
		<< "is already in use, ignoring" );
	return false;
    }

    netChannel::open();
    netChannel::bind( "", port );
    netChannel::listen( 5 );
    SG_LOG( SG_IO, SG_INFO, "Props server started on port " << port );

    set_enabled( true );
    return true;
}

/**
 * 
 */
bool
FGProps::close()
{
    SG_LOG( SG_IO, SG_INFO, "closing FGProps" );   
    return true;
}

/**
 * 
 */
bool
FGProps::process()
{
    netChannel::poll();
    return true;
}

/**
 * 
 */
void
FGProps::handleAccept()
{
    netAddress addr;
    int handle = accept( &addr );
    SG_LOG( SG_IO, SG_INFO, "Props server accepted connection from "
	    << addr.getHost() << ":" << addr.getPort() );
    PropsChannel* channel = new PropsChannel();
    channel->setHandle( handle );
}