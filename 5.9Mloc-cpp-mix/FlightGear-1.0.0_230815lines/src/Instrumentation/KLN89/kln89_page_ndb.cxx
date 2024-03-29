// kln89_page_*.[ch]xx - this file is one of the "pages" that
//                       are used in the KLN89 GPS unit simulation. 
//
// Written by David Luff, started 2005.
//
// Copyright (C) 2005 - David C Luff - david.luff@nottingham.ac.uk
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
// $Id: kln89_page_ndb.cxx,v 1.4 2006-02-21 01:19:03 mfranz Exp $

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "kln89_page_ndb.hxx"

KLN89NDBPage::KLN89NDBPage(KLN89* parent) 
: KLN89Page(parent) {
	_nSubPages = 2;
	_subPage = 0;
	_name = "NDB";
	_ndb_id = "SF";
	np = NULL;
}

KLN89NDBPage::~KLN89NDBPage() {
}

void KLN89NDBPage::Update(double dt) {
	bool actPage = (_kln89->_activePage->GetName() == "ACT" ? true : false);
	bool multi;  // Not set by FindFirst...
	bool exact = false;
	if(_ndb_id.size() == 3) exact = true;
	if(np == NULL) {
		np = _kln89->FindFirstNDBById(_ndb_id, multi, exact);
	} else if(np->get_ident() != _ndb_id) {
		np = _kln89->FindFirstNDBById(_ndb_id, multi, exact);
	}
	//if(np == NULL) cout << "NULL... ";
	//if(b == false) cout << "false...\n";
	/*
	if(np && b) {
		cout << "VOR FOUND!\n";
	} else {
		cout << ":-(\n";
	}
	*/
	if(np) {
		//cout << np->id << '\n';
		_ndb_id = np->get_ident();
		if(_kln89->GetActiveWaypoint()) {
			if(_ndb_id == _kln89->GetActiveWaypoint()->id) {
				if(!(_kln89->_waypointAlert && _kln89->_blink)) {
					// Active waypoint arrow
					_kln89->DrawSpecialChar(4, 2, 0, 3);
				}
			}
		}
		if(_kln89->_mode != KLN89_MODE_CRSR) {
			if(!_entInvert) {
				if(!actPage) {
					_kln89->DrawText(np->get_ident(), 2, 1, 3);
				} else {
					// If it's the ACT page, The ID is shifted slightly right to make space for the waypoint index.
					_kln89->DrawText(np->get_ident(), 2, 4, 3);
					char buf[3];
					int n = snprintf(buf, 3, "%i", _kln89->GetActiveWaypointIndex() + 1);
					_kln89->DrawText((string)buf, 2, 3 - n, 3);
				}
			} else {
				if(!_kln89->_blink) {
					_kln89->DrawText(np->get_ident(), 2, 1, 3, false, 99);
					_kln89->DrawEnt();
				}
			}
		}
		if(_subPage == 0) {
			// TODO - trim VOR-DME from the name, convert to uppercase, abbreviate, etc
			_kln89->DrawText(np->get_name(), 2, 0, 2);
			_kln89->DrawLatitude(np->get_lat(), 2, 3, 1);
			_kln89->DrawLongitude(np->get_lon(), 2, 3, 0);
		} else {
			_kln89->DrawDirDistField(np->get_lat() * SG_DEGREES_TO_RADIANS, np->get_lon() * SG_DEGREES_TO_RADIANS,
			                         2, 0, 0, _to_flag, (_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == 4 ? true : false));
		}
	} else {
		if(_kln89->_mode != KLN89_MODE_CRSR) _kln89->DrawText(_ndb_id, 2, 1, 3);
		if(_subPage == 0) {
			_kln89->DrawText("----.-", 2, 9, 3);
			_kln89->DrawText("--------------", 2, 0, 2);
			_kln89->DrawText("- -- --.--'", 2, 3, 1);
			_kln89->DrawText("---- --.--'", 2, 3, 0);
			_kln89->DrawSpecialChar(0, 2, 7, 1);
			_kln89->DrawSpecialChar(0, 2, 7, 0);
		}
	}
	
	if(_kln89->_mode == KLN89_MODE_CRSR) {
		if(_uLinePos > 0 && _uLinePos < 4) {
			// TODO - blink as well
			_kln89->Underline(2, _uLinePos, 3, 1);
		}
		for(unsigned int i = 0; i < _ndb_id.size(); ++i) {
			if(_uLinePos != (i + 1)) {
				_kln89->DrawChar(_ndb_id[i], 2, i + 1, 3);
			} else {
				if(!_kln89->_blink) _kln89->DrawChar(_ndb_id[i], 2, i + 1, 3);
			}
		}
	}
	
	_id = _ndb_id;

	KLN89Page::Update(dt);
}

void KLN89NDBPage::SetId(const string& s) {
	_last_ndb_id = _ndb_id;
	_save_ndb_id = _ndb_id;
	_ndb_id = s;
	np = NULL;
}

void KLN89NDBPage::CrsrPressed() {
	if(_kln89->_mode == KLN89_MODE_DISP) return;
	if(_kln89->_obsMode) {
		_uLinePos = 0;
	} else {
		_uLinePos = 1;
	}
	if(_subPage == 0) {
		_maxULinePos = 17;
	} else {
		_maxULinePos = 4;
	}
}

void KLN89NDBPage::ClrPressed() {
	if(_subPage == 1 && _uLinePos == 4) {
		_to_flag = !_to_flag;
	}
}

void KLN89NDBPage::EntPressed() {
	if(_entInvert) {
		_entInvert = false;
		_last_ndb_id = _ndb_id;
		_ndb_id = _save_ndb_id;
	}
}

void KLN89NDBPage::Knob2Left1() {
	if(_kln89->_mode != KLN89_MODE_CRSR || _uLinePos == 0) {
		KLN89Page::Knob2Left1();
	} else {
		if(_uLinePos < 4) {
			// Same logic for both pages - set the ID
			_ndb_id = _ndb_id.substr(0, _uLinePos);
			// ASSERT(_uLinePos > 0);
			if(_uLinePos == (_ndb_id.size() + 1)) {
				_ndb_id += '9';
			} else {
				_ndb_id[_uLinePos - 1] = _kln89->DecChar(_ndb_id[_uLinePos - 1], (_uLinePos == 1 ? false : true));
			}
		} else {
			if(_subPage == 0) {
				// set by name
			} else {
				// NO-OP - from/to field is switched by clr button, not inner knob.
			}
		}
	}
}

void KLN89NDBPage::Knob2Right1() {
	if(_kln89->_mode != KLN89_MODE_CRSR || _uLinePos == 0) {
		KLN89Page::Knob2Right1();
	} else {
		if(_uLinePos < 4) {
			// Same logic for both pages - set the ID
			_ndb_id = _ndb_id.substr(0, _uLinePos);
			// ASSERT(_uLinePos > 0);
			if(_uLinePos == (_ndb_id.size() + 1)) {
				_ndb_id += 'A';
			} else {
				_ndb_id[_uLinePos - 1] = _kln89->IncChar(_ndb_id[_uLinePos - 1], (_uLinePos == 1 ? false : true));
			}
		} else {
			if(_subPage == 0) {
				// set by name
			} else {
				// NO-OP - from/to field is switched by clr button, not inner knob.
			}
		}
	}
}
