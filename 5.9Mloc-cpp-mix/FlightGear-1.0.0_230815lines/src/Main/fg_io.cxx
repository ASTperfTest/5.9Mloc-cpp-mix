// fg_io.cxx -- higher level I/O channel management routines
//
// Written by Curtis Olson, started November 1999.
//
// Copyright (C) 1999  Curtis L. Olson - http://www.flightgear.org/~curt
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
// $Id: fg_io.cxx,v 1.24.2.1 2007-07-25 15:17:10 mfranz Exp $

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <simgear/compiler.h>

#include <stdlib.h>             // atoi()

#include STL_STRING

#include <simgear/debug/logstream.hxx>
#include <simgear/io/iochannel.hxx>
#include <simgear/io/sg_file.hxx>
#include <simgear/io/sg_serial.hxx>
#include <simgear/io/sg_socket.hxx>
#include <simgear/io/sg_socket_udp.hxx>
#include <simgear/math/sg_types.hxx>
#include <simgear/timing/timestamp.hxx>
#include <simgear/misc/strutils.hxx>

#include <Network/protocol.hxx>
#include <Network/ATC-Main.hxx>
#include <Network/atlas.hxx>
#include <Network/AV400.hxx>
#include <Network/garmin.hxx>
#include <Network/httpd.hxx>
#ifdef FG_JPEG_SERVER
#  include <Network/jpg-httpd.hxx>
#endif
#include <Network/joyclient.hxx>
#include <Network/jsclient.hxx>
#include <Network/native.hxx>
#include <Network/native_ctrls.hxx>
#include <Network/native_fdm.hxx>
#include <Network/native_gui.hxx>
#include <Network/opengc.hxx>
#include <Network/nmea.hxx>
#include <Network/props.hxx>
#include <Network/pve.hxx>
#include <Network/ray.hxx>
#include <Network/rul.hxx>
#include <Network/generic.hxx>
#include <Network/multiplay.hxx>

#include "globals.hxx"
#include "fg_io.hxx"

SG_USING_STD(string);


FGIO::FGIO()
{
}

#include STL_ALGORITHM
SG_USING_STD(for_each);

static void delete_ptr( FGProtocol* p ) { delete p; }

FGIO::~FGIO()
{
    shutdown_all();
    for_each( io_channels.begin(), io_channels.end(), delete_ptr );
}


// configure a port based on the config string
FGProtocol*
FGIO::parse_port_config( const string& config )
{
    SG_LOG( SG_IO, SG_INFO, "Parse I/O channel request: " << config );

    vector<string> tokens = simgear::strutils::split( config, "," );
    if (tokens.empty())
    {
	SG_LOG( SG_IO, SG_ALERT,
		"Port configuration error: empty config string" );
	return 0;
    }

    string protocol = tokens[0];
    SG_LOG( SG_IO, SG_INFO, "  protocol = " << protocol );

    FGProtocol *io = 0;

    try
    {
	if ( protocol == "atcsim" ) {
            FGATCMain *atcsim = new FGATCMain;
	    atcsim->set_hz( 30 );
            if ( tokens.size() != 6 ) {
                SG_LOG( SG_IO, SG_ALERT, "Usage: --atcsim=[no-]pedals,"
                        << "input0_config,input1_config,"
                        << "output0_config,output1_config,file.nas" );
		delete atcsim;
                return NULL;
            }
            if ( tokens[1] == "no-pedals" ) {
                fgSetBool( "/input/atcsim/ignore-pedal-controls", true );
            } else {
                fgSetBool( "/input/atcsim/ignore-pedal-controls", false );
            }
            atcsim->set_path_names(tokens[2], tokens[3], tokens[4], tokens[5]);
	    return atcsim;
	} else if ( protocol == "atlas" ) {
	    FGAtlas *atlas = new FGAtlas;
	    io = atlas;
	} else if ( protocol == "opengc" ) {
	    // char wait;
	    // printf("Parsed opengc\n"); cin >> wait;
	    FGOpenGC *opengc = new FGOpenGC;
	    io = opengc;
	} else if ( protocol == "AV400" ) {
	    FGAV400 *av400 = new FGAV400;
	    io = av400;
	} else if ( protocol == "garmin" ) {
	    FGGarmin *garmin = new FGGarmin;
	    io = garmin;
	} else if ( protocol == "httpd" ) {
	    // determine port
	    string port = tokens[1];
	    return new FGHttpd( atoi(port.c_str()) );
#ifdef FG_JPEG_SERVER
	} else if ( protocol == "jpg-httpd" ) {
	    // determine port
	    string port = tokens[1];
	    return new FGJpegHttpd( atoi(port.c_str()) );
#endif
	} else if ( protocol == "joyclient" ) {
	    FGJoyClient *joyclient = new FGJoyClient;
	    io = joyclient;
        } else if ( protocol == "jsclient" ) {
            FGJsClient *jsclient = new FGJsClient;
            io = jsclient;
	} else if ( protocol == "native" ) {
	    FGNative *native = new FGNative;
	    io = native;
	} else if ( protocol == "native-ctrls" ) {
	    FGNativeCtrls *native_ctrls = new FGNativeCtrls;
	    io = native_ctrls;
	} else if ( protocol == "native-fdm" ) {
	    FGNativeFDM *native_fdm = new FGNativeFDM;
	    io = native_fdm;
	} else if ( protocol == "native-gui" ) {
	    FGNativeGUI *net_gui = new FGNativeGUI;
	    io = net_gui;
	} else if ( protocol == "nmea" ) {
	    FGNMEA *nmea = new FGNMEA;
	    io = nmea;
	} else if ( protocol == "props" || protocol == "telnet" ) {
	    io = new FGProps( tokens );
	    return io;
	} else if ( protocol == "pve" ) {
	    FGPVE *pve = new FGPVE;
	    io = pve;
	} else if ( protocol == "ray" ) {
	    FGRAY *ray = new FGRAY;
	    io = ray;
	} else if ( protocol == "rul" ) {
	    FGRUL *rul = new FGRUL;
	    io = rul;
        } else if ( protocol == "generic" ) {
            int n = 6;
            if (tokens[1] == "socket")  n++;
            else if (tokens[1] == "file") n--;
            FGGeneric *generic = new FGGeneric( tokens[n] );
            io = generic;
	} else if ( protocol == "multiplay" ) {
	    if ( tokens.size() != 5 ) {
		SG_LOG( SG_IO, SG_ALERT, "Ignoring invalid --multiplay option "
			"(4 arguments expected: --multiplay=dir,hz,hostname,port)" );
		return NULL;
	    }
	    string dir = tokens[1];
	    string rate = tokens[2];
	    string host = tokens[3];
	    string port = tokens[4];
	    return new FGMultiplay(dir, atoi(rate.c_str()), host, atoi(port.c_str()));
	} else {
	    return NULL;
	}
    }
    catch (FGProtocolConfigError& err)
    {
	SG_LOG( SG_IO, SG_ALERT, "Port configuration error: " << err.what() );
	delete io;
	return 0;
    }
    
    if (tokens.size() < 3) {
      SG_LOG( SG_IO, SG_ALERT, "Incompatible number of network arguments.");
      return NULL;
    }
    string medium = tokens[1];
    SG_LOG( SG_IO, SG_INFO, "  medium = " << medium );

    string direction = tokens[2];
    io->set_direction( direction );
    SG_LOG( SG_IO, SG_INFO, "  direction = " << direction );

    string hertz_str = tokens[3];
    double hertz = atof( hertz_str.c_str() );
    io->set_hz( hertz );
    SG_LOG( SG_IO, SG_INFO, "  hertz = " << hertz );

    if ( medium == "serial" ) {
        if ( tokens.size() < 5) {
          SG_LOG( SG_IO, SG_ALERT, "Incompatible number of arguments for serial communications.");
	  return NULL;
        }
	// device name
	string device = tokens[4];
	SG_LOG( SG_IO, SG_INFO, "  device = " << device );

	// baud
	string baud = tokens[5];
	SG_LOG( SG_IO, SG_INFO, "  baud = " << baud );

	SGSerial *ch = new SGSerial( device, baud );
	io->set_io_channel( ch );
    } else if ( medium == "file" ) {
	// file name
        if ( tokens.size() < 4) {
          SG_LOG( SG_IO, SG_ALERT, "Incompatible number of arguments for file I/O.");
	  return NULL;
        }
	  
	string file = tokens[4];
	SG_LOG( SG_IO, SG_INFO, "  file name = " << file );

	SGFile *ch = new SGFile( file );
	io->set_io_channel( ch );
    } else if ( medium == "socket" ) {
        if ( tokens.size() < 6) {
          SG_LOG( SG_IO, SG_ALERT, "Incompatible number of arguments for socket communications.");
	  return NULL;
        }
      	string hostname = tokens[4];
	string port = tokens[5];
	string style = tokens[6];

	SG_LOG( SG_IO, SG_INFO, "  hostname = " << hostname );
	SG_LOG( SG_IO, SG_INFO, "  port = " << port );
	SG_LOG( SG_IO, SG_INFO, "  style = " << style );

	io->set_io_channel( new SGSocket( hostname, port, style ) );
    }

    return io;
}


// step through the port config streams (from fgOPTIONS) and setup
// serial port channels for each
void
FGIO::init()
{
    // SG_LOG( SG_IO, SG_INFO, "I/O Channel initialization, " <<
    //         globals->get_channel_options_list()->size() << " requests." );

    FGProtocol *p;

    // we could almost do this in a single step except pushing a valid
    // port onto the port list copies the structure and destroys the
    // original, which closes the port and frees up the fd ... doh!!!

    // parse the configuration strings and store the results in the
    // appropriate FGIOChannel structures
    typedef vector<string> container;
    container::iterator i = globals->get_channel_options_list()->begin();
    container::iterator end = globals->get_channel_options_list()->end();
    for (; i != end; ++i )
    {
	p = parse_port_config( *i );
	if ( p != NULL ) {
	    p->open();
	    io_channels.push_back( p );
	    if ( !p->is_enabled() ) {
		SG_LOG( SG_IO, SG_ALERT, "I/O Channel config failed." );
		exit(-1);
	    }
	} else {
	    SG_LOG( SG_IO, SG_INFO, "I/O Channel parse failed." );
	}
    }
}


// process any IO channel work
void
FGIO::update( double delta_time_sec )
{
    // cout << "processing I/O channels" << endl;
    // cout << "  Elapsed time = " << delta_time_sec << endl;

    typedef vector< FGProtocol* > container;
    container::iterator i = io_channels.begin();
    container::iterator end = io_channels.end();
    for (; i != end; ++i ) {
	FGProtocol* p = *i;

	if ( p->is_enabled() ) {
	    p->dec_count_down( delta_time_sec );
	    double dt = 1 / p->get_hz();
	    if ( p->get_count_down() < 0.33 * dt ) {
	      p->process();
	      p->inc_count();
	      while ( p->get_count_down() < 0.33 * dt ) {
		p->inc_count_down( dt );
	      }
	      // double ave = elapsed_time / p->get_count();
	      // cout << "  ave rate = " << ave << endl;
	    }
	}
    }
}


void
FGIO::shutdown_all() {
    FGProtocol *p;

    // cout << "shutting down all I/O channels" << endl;

    typedef vector< FGProtocol* > container;
    container::iterator i = io_channels.begin();
    container::iterator end = io_channels.end();
    for (; i != end; ++i )
    {
	p = *i;

	if ( p->is_enabled() ) {
	    p->close();
	}
    }
}

void
FGIO::bind()
{
}

void
FGIO::unbind()
{
}
