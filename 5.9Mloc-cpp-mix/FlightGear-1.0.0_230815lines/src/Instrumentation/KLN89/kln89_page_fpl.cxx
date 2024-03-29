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
// $Id: kln89_page_fpl.cxx,v 1.3 2006-02-21 01:19:03 mfranz Exp $

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "kln89_page_fpl.hxx"
#include <algorithm>

using namespace std;

KLN89FplPage::KLN89FplPage(KLN89* parent)
: KLN89Page(parent) {
	_nSubPages = 26;
	_subPage = 0;
	_name = "FPL";
	_fpMode = 0;
	_actFpMode = 0;
	_wLinePos = 0;
	_bEntWp = false;
	_bEntExp = false;
	_entWp = NULL;
	_fplPos = 0;
	_resetFplPos0 = true;
	_delFP = false;
	_delWp = false;
	_delAppr = false;
	_changeAppr = false;
	_fp0SelWpId = "";
}

KLN89FplPage::~KLN89FplPage() {
}

void KLN89FplPage::Update(double dt) {
	Calc();
	
	// NOTE - we need to draw the active leg arrow outside of this block to avoid the _delFP check.
	// TODO - we really ought to merge the page 0 and other pages drawing code with a couple of lines of extra logic.
	if(_subPage == 0 && !_delFP) {	// Note that in the _delFP case, the active flightplan gets a header, and hence the same geometry as the other fps, so we draw it there.
		// active FlightPlan
		// NOTE THAT FOR THE ACTIVE FLIGHT PLAN, TOP POSITION IS STILL 4 in the underline position scheme, to make 
		// copy and paste easier!!!!

		// ---------------------------------- Copy the active FlightPlan and insert approach header and fence if required ---------------
		// For synatical convienience
		//vector<GPSWaypoint*> waylist = _kln89->_flightPlans[_subPage]->waypoints;
		// Copy every waypoint for now.
        // This is inefficient, but allows us to insert dummy waypoints to represent the header and fence 
		// in our *local copy* of the flightplan, if an approach is loaded.  There must be a better way to do this!!!!
		//cout << "AA" << endl;
		vector<GPSWaypoint> waylist;
		for(unsigned int i=0; i<_kln89->_flightPlans[_subPage]->waypoints.size(); ++i) {
			waylist.push_back(*_kln89->_flightPlans[_subPage]->waypoints[i]);
		}
		//cout << "BB" << endl;
		_hdrPos = -1;
		_fencePos = -1;
		if(_kln89->_approachLoaded) {
			GPSWaypoint wp;
			wp.id = "HHHH";
			wp.type = GPS_WP_VIRT;
			wp.appType = GPS_HDR;
			//cout << "CC" << endl;
			for(unsigned int i=0; i<waylist.size(); ++i) {
				// Insert the hdr immediately before the IAF
				if(waylist[i].appType == GPS_IAF) {
					waylist.insert(waylist.begin()+i, wp);
					// Need to insert empty string into the params to keep them in sync
					_params.insert(_params.begin()+i-1, "");
					_hdrPos = i;
					break;
				}
			}
			//cout << "DD" << endl;
			wp.id = "FFFF";
			wp.type = GPS_WP_VIRT;
			wp.appType = GPS_FENCE;
			for(unsigned int i=0; i<waylist.size(); ++i) {
				// Insert the fence between the MAF and the MAP
				if(waylist[i].appType == GPS_MAHP) {
					waylist.insert(waylist.begin()+i, wp);
					// Need to insert empty string into the params to keep them in sync
					_params.insert(_params.begin()+i-1, "");
					_fencePos = i;
					break;
				}
			}
		}
		/*
		// Now make up a vector of waypoint numbers, since they aren't aligned with list position anymore
		int num = 0;
		vector<int> numlist;
		numlist.clear();
		for(unsigned int i=0; i<waylist.size(); ++i) {
			if(waylist[i].appType != GPS_HDR && waylist[i].appType != GPS_FENCE) {
				numlist.push_back(num);
				num++;
			} else {
				numlist.push_back(-1);
			}
		}
		*/
		int hfcount = 0;
		for(unsigned int i=0; i<waylist.size(); ++i) {
			//cout << i + 1 - hfcount << ":  ID= " << waylist[i].id;
			if(waylist[i].appType == GPS_HDR) {
				hfcount++;
				//cout << " HDR!";
			}
			if(waylist[i].appType == GPS_FENCE) {
				hfcount++;
				//cout << " FENCE!";
			}
			//cout << '\n';
		}
		//----------------------------------------- end active FP copy ------------------------------------------------
		
		// Recalculate which waypoint is displayed at the top of the list if required (generally if this page has lost focus).
		int idx = _parent->GetActiveWaypointIndex();
		if(_resetFplPos0) {
			if(waylist.size() <= 1) {
				_fplPos = 0;
			} else if(waylist.size() <= 4) {
				_fplPos = 1;
			} else {
				// Make the active waypoint the second WP displayed
				_fplPos = idx;
				if(_fplPos != 0) {
					_fplPos--;
				}
			}
			//cout << "HeaderPos = " << _hdrPos << ", fencePos = " << _fencePos << ", _fplPos = " << _fplPos << ", active waypoint index = " << _parent->GetActiveWaypointIndex() << '\n';
			if(_hdrPos >= 0 && idx >= _hdrPos) {
				_fplPos++;
				if(_fencePos >= 0 && (idx + 1) >= _fencePos) {
					_fplPos++;
				}
			}
			_resetFplPos0 = false;
		}
		
		// Increment the active waypoint position if required due hdr and fence here not above so it gets called every frame
		if(_hdrPos >= 0 && idx >= _hdrPos) {
			idx++;
			if(_fencePos >= 0 && idx >= _fencePos) {
				idx++;
			}
		}
		
		// Draw the leg arrow etc
		int diff = idx - (int)_fplPos;
		int drawPos = -1;
		if(idx < 0) {
			// No active waypoint
		} else if(diff < 0) {
			// Off screen to the top
		} else if(diff > 2) {
			// TODO !
		} else {
			drawPos = diff;
		}
		// Only the head is blinked during waypoint alerting
		if(!(_kln89->_waypointAlert && _kln89->_blink)) {
			_kln89->DrawSpecialChar(4, 2, 0, 3-drawPos);
		}
		// If the active waypoint is immediately after an approach header then we need to do an extra-long tail leg
		if(_hdrPos >= 0 && idx == _hdrPos + 1) {
			if(drawPos > 0 && !_kln89->_dto) _kln89->DrawLongLegTail(3-drawPos);
		} else {
			if(drawPos > 0 && !_kln89->_dto) _kln89->DrawLegTail(3-drawPos);
		}
		
		//cout << "Top pos is " << _fplPos0 << ' ';
		
		if(_kln89->_mode == KLN89_MODE_CRSR) {
			if(_uLinePos == 3) {
				_kln89->Underline(2, 13, 3, 3);
			} else if(_uLinePos >= 4) {
				if(_bEntWp) {
					if(_wLinePos == 0) {
						_kln89->Underline(2, 5, 3 - (_uLinePos - 4), 4);
					} else if(_wLinePos == 4) {
						_kln89->Underline(2, 4, 3 - (_uLinePos - 4), 4);
					} else {
						_kln89->Underline(2, 4, 3 - (_uLinePos - 4), _wLinePos);
						_kln89->Underline(2, 5 + _wLinePos, 3 - (_uLinePos - 4), 4 - _wLinePos);
					}
					if(!_kln89->_blink) {
						//_kln89->DrawText(_entWp->id, 2, 4, 2 - (_uLinePos - 4), false, _wLinePos);
						_kln89->DrawEnt();
					}
				} else {
					_kln89->Underline(2, 4, 3 - (_uLinePos - 4), 5);
				}
			}
		}
		// ----------------------------------
		//cout << "A1" << endl;
			
		// Sanity check the top position - remember that we can have an extra blank one at the bottom even if CRSR is off if crsr is switched on then off
		if((int)_fplPos > ((int)waylist.size()) - 3) _fplPos = (((int)waylist.size()) - 3 < 0 ? 0 : waylist.size() - 3);
		unsigned int last_pos;
		if(waylist.empty()) {
			last_pos = 0;
		} else {
			last_pos = ((int)_fplPos == ((int)waylist.size()) - 3 ? waylist.size() : waylist.size() - 1);
		}
		//cout << "Initialising last_pos, last_pos = " << last_pos << '\n';
		//cout << "B1" << endl;
		if(waylist.size() < 4) last_pos = waylist.size();
		
		// Don't draw the cyclic field header if the top waypoint is the approach header
		// Not sure if this also applies to the fence - don't think so but TODO - check!
		if(waylist[_fplPos].appType != GPS_HDR) {
			_kln89->DrawChar('>', 2, 12, 3);
			if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == 3 && _kln89->_blink)) {
				DrawFpMode(3);
			}
		}
		
		// ACTIVE
		for(unsigned int i=0; i<4; ++i) {
			//cout << "F1... " << i << endl;
			// Don't draw the waypoint number for the header or fence
			//cout << "_fplPos0 = " << _fplPos0 << ", waylist size is " << waylist.size() << '\n';
			//cout << "F1, i = " << i << ", way id = " << waylist[_fplPos0+i].id << '\n'; 
			if((waylist[_fplPos+i].appType == GPS_HDR || waylist[_fplPos+i].appType == GPS_FENCE) 
			    && i != 3) {	// By definition, the header and fence lines can't be displayed on the last line hence the unconditional !i==3 is safe.
				// no-op
				//cout << "NOOP\n";
			} else {
				int n = (i < 3 ? _fplPos + i + 1 : last_pos + 1);
				if(_kln89->_approachLoaded) {
					if(n > _hdrPos) --n;
					if(n > _fencePos) --n;
				}
				string s = GPSitoa(n);
				// Don't draw the colon for waypoints that are part of the published approach
				if(waylist[_fplPos+i].appType == GPS_APP_NONE) {
					s += ':';
				}
				if(!(_delWp && _uLinePos == i+4)) _kln89->DrawText(s, 2, 4 - (s[s.size()-1] == ':' ? s.size() : s.size()+1), 3 - i);
			}
			//cout << "F1 done!\n";
			bool drawID = true;
			if(_delWp && _uLinePos == i+4) {
				if(!_kln89->_blink) {
					_kln89->DrawText("Del", 2, 0, 3-i);
					_kln89->DrawChar('?', 2, 10, 3-i);
					_kln89->Underline(2, 0, 3-i, 11);
					_kln89->DrawEnt();
				}
			} else if(_kln89->_mode == KLN89_MODE_CRSR && _bEntWp && _uLinePos == i+4) {
				if(!_kln89->_blink) {
					if(_wLinePos >= _entWp->id.size()) {
						_kln89->DrawText(_entWp->id, 2, 4, 3-i);
						_kln89->DrawChar(' ', 2, 4+_wLinePos, 3-i, false, true);
					} else {
						_kln89->DrawText(_entWp->id.substr(0, _wLinePos), 2, 4, 3-i);
						_kln89->DrawChar(_entWp->id[_wLinePos], 2, 4+_wLinePos, 3-i, false, true);
						_kln89->DrawText(_entWp->id.substr(_wLinePos+1, _entWp->id.size()-_wLinePos-1), 2, 5+_wLinePos, 3-i);
					}
				}
				drawID = false;
			}
			//cout << "F2" << endl;
			if(drawID) {
				//cout << "F2a" << endl;
				if(i == 3 || _fplPos + i == waylist.size()) {
					//cout << "F2a1" << endl;
					//cout << "_uLinePos = " << _uLinePos << ", i = " << i << ", waylist.size() = " << waylist.size() << endl;
					if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == (i + 4) && _kln89->_blink)) {
						//cout << "Drawing underline..." << endl;
						_kln89->DrawText(last_pos < waylist.size() ? waylist[last_pos].GetAprId() : "_____", 2, 4, 3-i);
						//cout << "2" << endl;
					}
					//cout << "3" << endl;
					//cout << "last_pos = " << last_pos << endl;
					if(last_pos > 0 && last_pos < waylist.size() && i > 0) {
						//cout << "4" << endl; 
						// Draw the param
						if(_actFpMode == 0) {
							string s = _params[last_pos - 1];
							_kln89->DrawText(s, 2, 16-s.size(), 3-i);
						} else if(_actFpMode == 3) {
							string s = _params[last_pos - 1];
							_kln89->DrawText(s, 2, 15-s.size(), 3-i);
							_kln89->DrawSpecialChar(0, 2, 15, 3-i);
						}
						//cout << "5" << endl;
					}
					//cout << "6" << endl;
					break;
				} else {
					//cout << "F2a2" << endl;
					if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == (i + 4) && _kln89->_blink)) {
						if(waylist[_fplPos+i].appType == GPS_HDR) {
							if(_delAppr) {
								_kln89->DrawText("DELETE APPR?", 2, 1, 3-i);
							} else if(_changeAppr) {
								_kln89->DrawText("CHANGE APPR?", 2, 1, 3-i);
							} else {
								_kln89->DrawText(_kln89->_approachAbbrev, 2, 1, 3-i);
								_kln89->DrawText(_kln89->_approachRwyStr, 2, 7, 3-i);
								_kln89->DrawText(_kln89->_approachID, 2, 12, 3-i);
							}
						} else if(waylist[_fplPos+i].appType == GPS_FENCE) {
							_kln89->DrawText("*NO WPT SEQ", 2, 0, 3-i);
						} else {
							_kln89->DrawText(waylist[_fplPos+i].GetAprId(), 2, 4, 3-i);
						}
					}
				}
				//cout << "F2b" << endl;
				if(i > 0) {
					// Draw the param
					//cout << "i > 0 param draw...\n";
					if(_actFpMode == 0) {
						string s = _params[_fplPos + i - 1];
						_kln89->DrawText(s, 2, 16-s.size(), 3-i);
					} else if(_actFpMode == 3) {
						string s = _params[_fplPos + i - 1];
						_kln89->DrawText(s, 2, 15-s.size(), 3-i);
						_kln89->DrawSpecialChar(0, 2, 15, 3-i);
					}
				}
				//cout << "F2c" << endl;
			}
			//cout << "F3" << endl;
		}
		//cout << "GGGGGG" << endl;
	} else {  // Not active flightplan
		//cout << "Top pos is " << _fplPos << ' ';
		// For synatical convienience
		//int nWp = (_subPage == 0 && !_delFP ? 4 : 3);	// number of waypoints to display
		vector<GPSWaypoint*> waylist = _kln89->_flightPlans[_subPage]->waypoints;
		if(waylist.empty()) {
			if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == 1 && _kln89->_blink)) {
				_kln89->DrawText(_delFP ? "Delete FPL?" : "Copy FPL 0?", 2, 0, 3);
			}
		} else {
			if(!(_kln89->_mode == KLN89_MODE_CRSR && (_uLinePos == 1 || _uLinePos == 2) && _kln89->_blink)) {
				_kln89->DrawText(_delFP ? "Delete FPL?" : "Use?", 2, 0, 3);
			}
			if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == 2 && _kln89->_blink)) {
				if(!_delFP) _kln89->DrawText("Inverted?", 2, 5, 3);
			}
		}
		
		// ----------------------------------
		if(_kln89->_mode == KLN89_MODE_CRSR) {
			if(_uLinePos == 1) {
				if(!_kln89->_blink) {
					_kln89->Underline(2, 0, 3, (waylist.empty() || _delFP ? 11 : 4));	// This underline is blinked
					_kln89->DrawEnt();
				}
			} else if(_uLinePos == 2) {
				// assert(!waylist.empty());
				if(!_kln89->_blink) {
					_kln89->Underline(2, 0, 3, 14);	// This underline is blinked
					_kln89->DrawEnt();
				}
			} else if(_uLinePos == 3) {
				_kln89->Underline(2, 13, 2, 3);
			} else if(_uLinePos >= 4) {
				if(_bEntWp) {
					if(_wLinePos == 0) {
						_kln89->Underline(2, 5, 2 - (_uLinePos - 4), 4);
					} else if(_wLinePos == 4) {
						_kln89->Underline(2, 4, 2 - (_uLinePos - 4), 4);
					} else {
						_kln89->Underline(2, 4, 2 - (_uLinePos - 4), _wLinePos);
						_kln89->Underline(2, 5 + _wLinePos, 2 - (_uLinePos - 4), 4 - _wLinePos);
					}
					if(!_kln89->_blink) {
						//_kln89->DrawText(_entWp->id, 2, 4, 2 - (_uLinePos - 4), false, _wLinePos);
						_kln89->DrawEnt();
					}
				} else {
					if(!_delWp) _kln89->Underline(2, 4, 2 - (_uLinePos - 4), 5);
				}
			}
		}
		// ----------------------------------
			
		_kln89->DrawChar('>', 2, 12, 2);
		if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == 3 && _kln89->_blink)) DrawFpMode(2);
		// Sanity check the top position - remember that we can have an extra blank one at the bottom even if CRSR is off if crsr is switched on then off
		if((int)_fplPos > ((int)waylist.size()) - 2) _fplPos = (((int)waylist.size()) - 2 < 0 ? 0 : waylist.size() - 2);
		unsigned int last_pos;
		if(waylist.empty()) {
			last_pos = 0;
		} else {
			last_pos = ((int)_fplPos == ((int)waylist.size()) - 2 ? waylist.size() : waylist.size() - 1);
		}
		if(waylist.size() < 3) last_pos = waylist.size();
		for(unsigned int i=0; i<3; ++i) {
			string s = GPSitoa(i < 2 ? _fplPos + i + 1 : last_pos + 1);
			s += ':';
			if(!(_delWp && _uLinePos == i+4)) _kln89->DrawText(s, 2, 4 - s.size(), 2 - i);
			bool drawID = true;
			if(_delWp && _uLinePos == i+4) {
				if(!_kln89->_blink) {
					_kln89->DrawText("Del", 2, 0, 2-i);
					_kln89->DrawChar('?', 2, 10, 2-i);
					_kln89->Underline(2, 0, 2-i, 11);
					_kln89->DrawEnt();
				}
			} else if(_kln89->_mode == KLN89_MODE_CRSR && _bEntWp && _uLinePos == i+4) {
				if(!_kln89->_blink) {
					if(_wLinePos >= _entWp->id.size()) {
						_kln89->DrawText(_entWp->id, 2, 4, 2-i);
						_kln89->DrawChar(' ', 2, 4+_wLinePos, 2-i, false, true);
					} else {
						_kln89->DrawText(_entWp->id.substr(0, _wLinePos), 2, 4, 2-i);
						_kln89->DrawChar(_entWp->id[_wLinePos], 2, 4+_wLinePos, 2-i, false, true);
						_kln89->DrawText(_entWp->id.substr(_wLinePos+1, _entWp->id.size()-_wLinePos-1), 2, 5+_wLinePos, 2-i);
					}
				}
				drawID = false;
			}
			if(drawID) {
				if(i == 2 || _fplPos + i == waylist.size()) {
					if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == (i + 4) && _kln89->_blink)) {
						_kln89->DrawText(last_pos < waylist.size() ? waylist[last_pos]->id : "_____", 2, 4, 2-i);
					}
					if(last_pos > 0 && last_pos < waylist.size() && i > 0) {
						// Draw the param
						if(_fpMode == 0) {
							string s = _params[last_pos - 1];
							_kln89->DrawText(s, 2, 16-s.size(), 2-i);
						}
					}
					break;
				} else {
					if(!(_kln89->_mode == KLN89_MODE_CRSR && _uLinePos == (i + 4) && _kln89->_blink)) {
						_kln89->DrawText(waylist[_fplPos+i]->id, 2, 4, 2-i);
					}
					if(i > 0) {
						// Draw the param
						if(_fpMode == 0) {
							string s = _params[_fplPos + i - 1];
							_kln89->DrawText(s, 2, 16-s.size(), 2-i);
						}
					}
				}
			}
		}
	}
	
	KLN89Page::Update(dt);
}

void KLN89FplPage::DrawFpMode(int ypos) {
	string s = "Dis";
	if(0 == _subPage) {
		if(_actFpMode == 1) {
			s = "ETE";
		} else if(_actFpMode == 2) {
			s = "UTC";	// TODO - alter depending on chosen timezone
		} else if(_actFpMode == 3) {
			s = (_kln89->_obsMode ? "OBS" : "Dtk");
		}
	} else {
		if(_fpMode == 1) {
			s = "Dtk";
		}
	}
	_kln89->DrawText(s, 2, 13, ypos);
}

// Bit of an ipsy-dipsy function this one - calc the required parameters for the displayed flightplan.
void KLN89FplPage::Calc() {
	_params.clear();
	GPSFlightPlan* fp = _kln89->_flightPlans[_subPage];
	vector<GPSWaypoint*> wv = fp->waypoints;
	if(0 == _subPage) {
		// Active FP - parameters are only displayed for the active waypoint onwards for the active plan,
		// and distance is cumulative from the user position.
		if(0 == _actFpMode) {
			// Dis
			double cum_tot = 0.0;
			if(wv.size() > 0) {
				//cum_tot += _kln89->GetHorizontalSeparation(_kln89->_gpsLat, _kln89->_gpsLon, wv[0]->lat, wv[0]->lon) * SG_METER_TO_NM;
				cum_tot += _kln89->GetGreatCircleDistance(_kln89->_gpsLat, _kln89->_gpsLon, wv[0]->lat, wv[0]->lon);
			}
			for(unsigned int i=1; i<wv.size(); ++i) {
				//cum_tot += _kln89->GetHorizontalSeparation(wv[i-1]->lat, wv[i-1]->lon, wv[i]->lat, wv[i]->lon) * SG_METER_TO_NM;	// TODO - add units switch!
				cum_tot += _kln89->GetGreatCircleDistance(wv[i-1]->lat, wv[i-1]->lon, wv[i]->lat, wv[i]->lon);	// TODO - add units switch!
				int n = (int)(cum_tot + 0.5);
				_params.push_back(GPSitoa(n));
			}
		} else if(1 == _actFpMode) {
		} else if(2 == _actFpMode) {
		} else {
			// Dtk
			for(int i=1; i<wv.size(); ++i) {
				double dtk = _kln89->GetMagHeadingFromTo(wv[i-1]->lat, wv[i-1]->lon, wv[i]->lat, wv[i]->lon);
				int n = (int)(dtk + 0.5);
				_params.push_back(GPSitoa(n));
			}
			
		}
	} else {
		// other FPs
		if(0 == _fpMode) {
			double cum_tot = 0.0;
			for(int i=1; i<wv.size(); ++i) {
				//cum_tot += _kln89->GetHorizontalSeparation(wv[i-1]->lat, wv[i-1]->lon, wv[i]->lat, wv[i]->lon) * SG_METER_TO_NM;	// TODO - add units switch!
				cum_tot += _kln89->GetGreatCircleDistance(wv[i-1]->lat, wv[i-1]->lon, wv[i]->lat, wv[i]->lon);	// TODO - add units switch!
				int n = (int)(cum_tot + 0.5);
				_params.push_back(GPSitoa(n));
			}
		} else {
		}
	}
}

void KLN89FplPage::CrsrPressed() {
	if(_delFP) {
		_delFP = false;
		_kln89->_mode = KLN89_MODE_DISP;
		return;
	}

	_wLinePos = 0;
	if(_kln89->_mode == KLN89_MODE_DISP) {
		_fp0SelWpId.clear();
		if(_bEntWp) {
			for(unsigned int i = 0; i < _kln89->_flightPlans[_subPage]->waypoints.size(); ++i) {
				if(_kln89->_flightPlans[_subPage]->waypoints[i] == _entWp) {
					_kln89->_flightPlans[_subPage]->waypoints.erase(_kln89->_flightPlans[_subPage]->waypoints.begin() + i);
				}
			}
			delete _entWp;
			_entWp = NULL;
			_bEntWp = false;
			_entWpStr.clear();
		}
	} else {
		if(_kln89->_obsMode) {
			_uLinePos = 0;
		} else {
			if(_kln89->_flightPlans[_subPage]->IsEmpty()) {
				_uLinePos = 4;
			} else {
				_uLinePos = (_subPage == 0 ? 3 : 1);
			}
		}
	}
}

void KLN89FplPage::ClrPressed() {
	if(_delFP) {
		_kln89->_mode = KLN89_MODE_DISP;
		_delFP = false;
	} else if(_delAppr) {
		_kln89->_mode = KLN89_MODE_DISP;
		_delAppr = false;
	} else {
		if(KLN89_MODE_CRSR == _kln89->_mode) {
			// TODO - see if we need to delete a waypoint
			if(_uLinePos >= 4) {
				if(_delWp) {
					_kln89->_mode = KLN89_MODE_DISP;
					_delWp = false;
				} else {
					// First check that we're not trying to delete an approach waypoint.  Note that we can delete the approach by deleting the header though.
					// Check for approach waypoints or header/fences in flightplan 0
					int n = _fplPos + _uLinePos - 4;
					bool hdrPos = false;
					bool fencePos = false;
					//cout << "_fplPos = " << _fplPos << ", _uLinePos = " << _uLinePos << ", n = " << n << ", _hdrPos = " << _hdrPos << ", _fencePos = " << _fencePos << '\n';
					if(n == _hdrPos) {
						//cout << "HEADER POS\n";
						hdrPos = true;
					}
					if(n == _fencePos) {
						//cout << "FENCE POS\n";
						fencePos = true;
					}
					if(_hdrPos >= 0 && n > _hdrPos) --n;
					if(_fencePos >= 0 && n >= _fencePos) --n;	// This one needs to be >= since n is already decremented by 1 in the line above!
					//cout << "New n = " << n << '\n';
					if(hdrPos) {
						//cout << "HDRP\n";
						_delAppr = true;
					} else if(fencePos) {
						//cout << "FENP\n";
						// no-op
					} else if(n >= _kln89->_flightPlans[_subPage]->waypoints.size()) {
						// no-op - off the end of the list on the entry field
					} else if(_kln89->_flightPlans[_subPage]->waypoints[n]->appType == GPS_APP_NONE) {
						//cout << "DELFP\n";
						_kln89->_mode = KLN89_MODE_CRSR;
						_delWp = true;
					} else {
						ShowScratchpadMessage("Invald", " Del  ");
					}
				}
			} else if(_uLinePos == 3) {
				if(_subPage == 0) {
					_actFpMode++;
					if(_actFpMode > 3) _actFpMode = 0;
				} else {
					_fpMode++;
					if(_fpMode > 1) _fpMode = 0;
				}
			}
		} else {
			_delFP = true;
			_uLinePos = 1;
			_kln89->_mode = KLN89_MODE_CRSR;
		}
	}
}

void KLN89FplPage::CleanUp() {
	// TODO - possibly need to clean up _delWp here as well, since it goes off if dto and then ent are pressed.
	
	_bEntWp = false;
	for(unsigned int i = 0; i < _kln89->_flightPlans[_subPage]->waypoints.size(); ++i) {
		if(_kln89->_flightPlans[_subPage]->waypoints[i] == _entWp) {
			_kln89->_flightPlans[_subPage]->waypoints.erase(_kln89->_flightPlans[_subPage]->waypoints.begin() + i);
		}
	}
	delete _entWp;
	_entWp = NULL;
	_entWpStr.clear();
	KLN89Page::CleanUp();
}

void KLN89FplPage::LooseFocus() {
	_fplPos = 0;
	_resetFplPos0 = true;
	_wLinePos = 0;
	_uLinePos = 0;
	_fp0SelWpId.clear();
	_scratchpadMsg = false;
}

void KLN89FplPage::EntPressed() {
	if(_delFP) {
		_parent->ClearFlightPlan(_subPage);
		CrsrPressed();
	} else if(_delWp) {
		int pos = _uLinePos - 4 + _fplPos;
		// Sanity check - the calculated wp position should never be off the end of the waypoint list.
		if(pos > _kln89->_flightPlans[_subPage]->waypoints.size() - 1) {
			cout << "ERROR - _uLinePos too big in KLN89FplPage::EntPressed!\n";
			return;
		}
		_kln89->_flightPlans[_subPage]->waypoints.erase(_kln89->_flightPlans[_subPage]->waypoints.begin() + pos);
		_delWp = false;
		// Do we need to re-calc _fplPos here?
	} else if(_bEntExp) {
		_bEntWp = false;
		_bEntExp = false;
		_entWp = NULL;	// DON'T delete it! - it's been pushed onto the waypoint list at this point.
		_entWpStr.clear();
		_kln89->_cleanUpPage = -1;
		_wLinePos = 0;
		// TODO - in actual fact the previously underlined waypoint stays in the same position and underlined
		// in some or possibly all circumstances - need to check this out and match it, but not too important
		// for now.
	} else if(_bEntWp) {
		if(_entWp != NULL) {
			// TODO - should be able to get rid of this switch I think and use the enum values.
			switch(_entWp->type) {
			case GPS_WP_APT:
				_kln89->_activePage = _kln89->_pages[0];
				_kln89->_curPage = 0;
				((KLN89Page*)_kln89->_pages[0])->SetEntInvert(true);
				break;
			case GPS_WP_VOR:
				_kln89->_activePage = _kln89->_pages[1];
				_kln89->_curPage = 1;
				((KLN89Page*)_kln89->_pages[1])->SetEntInvert(true);
				break;
			case GPS_WP_NDB:
				_kln89->_activePage = _kln89->_pages[2];
				_kln89->_curPage = 2;
				((KLN89Page*)_kln89->_pages[2])->SetEntInvert(true);
				break;
			case GPS_WP_INT:
				_kln89->_activePage = _kln89->_pages[3];
				_kln89->_curPage = 3;
				((KLN89Page*)_kln89->_pages[3])->SetEntInvert(true);
				break;
			case GPS_WP_USR:
				_kln89->_activePage = _kln89->_pages[4];
				_kln89->_curPage = 4;
				((KLN89Page*)_kln89->_pages[4])->SetEntInvert(true);
				break;
			default:
				cout << "Error - unknown waypoint type found in KLN89::FplPage::EntPressed()\n";
			}
			_kln89->_activePage->SetId(_entWp->id);
			_kln89->_entJump = 7;
			_kln89->_cleanUpPage = 7;
			_kln89->_entRestoreCrsr = true;
			_kln89->_mode = KLN89_MODE_DISP;
		}
		_bEntExp = true;
	} else if(_uLinePos == 1) {
		if(_kln89->_flightPlans[_subPage]->IsEmpty()) {
			// Copy fpl 0
			for(unsigned int i=0; i<_kln89->_flightPlans[0]->waypoints.size(); ++i) {
				GPSWaypoint* wp = new GPSWaypoint;
				*wp = *(_kln89->_flightPlans[0]->waypoints[i]);
				_kln89->_flightPlans[_subPage]->waypoints.push_back(wp);
			}
		} else {
			// Use
			_parent->ClearFlightPlan(0);
			for(unsigned int i=0; i<_kln89->_flightPlans[_subPage]->waypoints.size(); ++i) {
				GPSWaypoint* wp = new GPSWaypoint;
				*wp = *(_kln89->_flightPlans[_subPage]->waypoints[i]);
				_kln89->_flightPlans[0]->waypoints.push_back(wp);
			}
			_kln89->OrientateToActiveFlightPlan();
			_subPage = 0;
		}
		_parent->CrsrPressed();
	} else if(_uLinePos == 2) {
		if(_kln89->_flightPlans[_subPage]->IsEmpty()) {
			// ERROR !!!
		} else {
			// Use Invert
			_parent->ClearFlightPlan(0);
			for(unsigned int i=0; i<_kln89->_flightPlans[_subPage]->waypoints.size(); ++i) {
				GPSWaypoint* wp = new GPSWaypoint;
				*wp = *(_kln89->_flightPlans[_subPage]->waypoints[i]);
				// FIXME - very inefficient - use a reverse iterator on the source array and push_back instead!!!!!!!!
				_kln89->_flightPlans[0]->waypoints.insert(_kln89->_flightPlans[0]->waypoints.begin(), wp);
			}
			_kln89->OrientateToActiveFlightPlan();
		}
		_parent->CrsrPressed();
		_subPage = 0;
	}
}

void KLN89FplPage::Knob1Left1() {
	if(_delFP) {
		_delFP = false;
		return;
	}
	_delWp = false;
	_changeAppr = false;

	if(_kln89->_mode == KLN89_MODE_CRSR) {
		if(_bEntWp) {
			if(_wLinePos > 0) _wLinePos--;
		} else {
			// _uLinePos with empty/not-empty plan: 1 = Copy FPL 0? / Use?, 2 = unused if empty / Invert?, 3 = >Dis/Dtk field, 4+ = Waypoint 1+
			if(_uLinePos == 0) {
				// No-op
			} else if(_uLinePos == 1 || _uLinePos == 2) {
				_uLinePos--;
			} else if(_uLinePos == 3) {
				_uLinePos = 4;
			} else if(_uLinePos == 4) {
				if(_kln89->_flightPlans[_subPage]->IsEmpty()) {
					_uLinePos = (_subPage == 0 ? 0 : 1);
				} else if(_fplPos == 0) {
					_uLinePos = (_subPage == 0 ? 0 : 2);
				} else {
					_fplPos--;
				}
			} else if(_uLinePos == 5) {
				_uLinePos = 3;
			} else {
				_uLinePos--;
			}

			if(_subPage == 0 && _uLinePos > 3) {
				int ix = _fplPos + (_uLinePos - 4);
				if(_fencePos >= 0 && ix >= _fencePos) ix--;
				if(_hdrPos >= 0 && ix >= _hdrPos) ix--;
				if(ix >= _kln89->_activeFP->waypoints.size()) {
					_fp0SelWpId.clear();
				} else {
					_fp0SelWpId = _kln89->_activeFP->waypoints[ix]->id;
				}
			} else {
				_fp0SelWpId.clear();
				//cout << "Not page 0, or not in waypoints, clearing id!\n";
			}
		}
	}
}

void KLN89FplPage::Knob1Right1() {
	if(_delFP) {
		_delFP = false;
		return;
	}
	_delWp = false;
	_changeAppr = false;
	
	if(_kln89->_mode == KLN89_MODE_CRSR) {
		if(_bEntWp) {
			if(_wLinePos < 4) _wLinePos++;
		} else {
			// _uLinePos with empty/not-empty plan: 
			// 1 = Copy FPL 0? / Use?, 2 = unused if empty / Invert?, 3 = >Dis/Dtk field, 4+ = Waypoint 1+
			if(_uLinePos == 0) {
				_uLinePos = (_subPage == 0 ? 4 : 1);
			} else if(_uLinePos == 1) {
				_uLinePos = (_kln89->_flightPlans[_subPage]->IsEmpty() ? 4 : 2);
			} else if(_uLinePos == 2) {
				_uLinePos = 4;
			} else if(_uLinePos == 3) {
				if(!_kln89->_flightPlans[_subPage]->IsEmpty()) _uLinePos = 5;
			} else if(_uLinePos == 4) {
				_uLinePos = 3;
			} else if((_subPage == 0 && _uLinePos == 6) || (_subPage > 0 && _uLinePos == 5)) {
				// Urrggh - complicated!
				// 3 possibilities:
				// 1: We're on the entry field at the end of the list, and can't move any more.
				// 2: We're on the last or second-last field, and move to the last position
				// 3: We're on a field before the second-last one, and don't move, but change the list-head position
				// And 4: _subPage 0 can be complicated by the presence of header/fence lines in an approach.
				int hfcount = 0;
				if(_subPage == 0) {
					if(_hdrPos >= 0) hfcount++;
					if(_fencePos >= 0) hfcount++;
				}
				if(_kln89->_flightPlans[_subPage]->waypoints.size() == 1 || _fplPos == _kln89->_flightPlans[_subPage]->waypoints.size() + hfcount - 1) {
					// 1: Don't move
				} else if(_fplPos >= _kln89->_flightPlans[_subPage]->waypoints.size() + hfcount - (_subPage == 0 ? 4 : 3)) {
					_uLinePos++;
				} else {
					_fplPos++;
				}
			} else if(_uLinePos == 5) {
				// Must be _subPage 0
				_uLinePos++;
			} else {
				// Must be the last line - either _uLinePos 6 or 7 depending on _subPage
				int thresh = (_subPage == 0 ? 3 : 2);
				if(_kln89->_flightPlans[_subPage]->waypoints.size() == thresh || _fplPos == _kln89->_flightPlans[_subPage]->waypoints.size() - thresh) {
					// Don't move
				} else {
					_fplPos++;
				}
			}
			
			if(_subPage == 0 && _uLinePos > 3) {
				int ix = _fplPos + (_uLinePos - 4);
				if(_fencePos >= 0 && ix >= _fencePos) ix--;
				if(_hdrPos >= 0 && ix >= _hdrPos) ix--;
				if(ix >= _kln89->_activeFP->waypoints.size()) {
					_fp0SelWpId.clear();
				} else {
					_fp0SelWpId = _kln89->_activeFP->waypoints[ix]->id;
				}
			} else {
				_fp0SelWpId.clear();
				//cout << "Not page 0, or not in waypoints, clearing id!\n";
			}
		}
	}
}

void KLN89FplPage::Knob2Left1() {
	if(_delFP) {
		_delFP = false;
		return;
	}
	_delWp = false;

	if(_kln89->_mode != KLN89_MODE_CRSR || _uLinePos == 0) {
		if(_kln89->_mode != KLN89_MODE_CRSR) _resetFplPos0 = true;
		KLN89Page::Knob2Left1();
	} else {
		if(_uLinePos > 3) {
			// Check for approach waypoints or header/fences in flightplan 0
			int n = _fplPos + _uLinePos - 4;
			bool hdrPos = false;
			bool fencePos = false;
			bool appWp = false;
			//cout << "_fplPos = " << _fplPos << ", _uLinePos = " << _uLinePos << ", n = " << n << ", _hdrPos = " << _hdrPos << ", _fencePos = " << _fencePos << '\n';
			if(n == _hdrPos) {
				//cout << "HEADER POS\n";
				hdrPos = true;
			}
			if(n == _fencePos) {
				//cout << "FENCE POS\n";
				fencePos = true;
			}
			if(_hdrPos >= 0 && n > _hdrPos) --n;
			if(_fencePos >= 0 && n >= _fencePos) --n;	// This one needs to be >= since n is already decremented by 1 in the line above!
			//cout << "New n = " << n << '\n';
			
			if(n < _kln89->_flightPlans[_subPage]->waypoints.size()) {
				if(_kln89->_flightPlans[_subPage]->waypoints[n]->appType != GPS_APP_NONE) {
					appWp = true;
				}
			}
			
			if(hdrPos) {
				// TODO - not sure what we actually do in this condition
				_changeAppr = true;
			} else if(fencePos) {
				// no-op?
			} else if(appWp) {
				ShowScratchpadMessage("Invald", " Add  ");
			} else {
				if((_wLinePos + 1) > _entWpStr.size()) {
					_entWpStr += '9';
				} else {
					_entWpStr[_wLinePos] = _kln89->DecChar(_entWpStr[_wLinePos], (_wLinePos == 0 ? false : true));
				}
				_bEntWp = true;
				_fp0SelWpId.clear();	// Waypoints don't become the DTO default whilst being entered.
				
				bool multi;
				const GPSWaypoint* wp = _kln89->FindFirstById(_entWpStr.substr(0, _wLinePos+1), multi, false);
				if(NULL == wp) {
					// no-op
				} else {
					if(_entWp == NULL) {
						_entWp = new GPSWaypoint;
						if(_fplPos + (_uLinePos - 4) >= _kln89->_flightPlans[_subPage]->waypoints.size()) {
							_kln89->_flightPlans[_subPage]->waypoints.push_back(_entWp);
						} else {
							_kln89->_flightPlans[_subPage]->waypoints.insert(_kln89->_flightPlans[_subPage]->waypoints.begin()+(_fplPos + (_uLinePos - 4)), _entWp);
						}
					}
					// copy
					*_entWp = *wp;
				}
			}
		}
	}
}

void KLN89FplPage::Knob2Right1() {
	if(_delFP) {
		_delFP = false;
		return;
	}
	_delWp = false;

	if(_kln89->_mode != KLN89_MODE_CRSR || _uLinePos == 0) {
		if(_kln89->_mode != KLN89_MODE_CRSR) _resetFplPos0 = true;
		KLN89Page::Knob2Right1();
	} else {
		if(_uLinePos > 3) {
			// Check for approach waypoints or header/fences in flightplan 0
			int n = _fplPos + _uLinePos - 4;
			bool hdrPos = false;
			bool fencePos = false;
			bool appWp = false;
			//cout << "_fplPos = " << _fplPos << ", _uLinePos = " << _uLinePos << ", n = " << n << ", _hdrPos = " << _hdrPos << ", _fencePos = " << _fencePos << '\n';
			if(n == _hdrPos) {
				//cout << "HEADER POS\n";
				hdrPos = true;
			}
			if(n == _fencePos) {
				//cout << "FENCE POS\n";
				fencePos = true;
			}
			if(_hdrPos >= 0 && n > _hdrPos) --n;
			if(_fencePos >= 0 && n >= _fencePos) --n;	// This one needs to be >= since n is already decremented by 1 in the line above!
			//cout << "New n = " << n << '\n';
			
			if(n < _kln89->_flightPlans[_subPage]->waypoints.size()) {
				if(_kln89->_flightPlans[_subPage]->waypoints[n]->appType != GPS_APP_NONE) {
					appWp = true;
				}
			}
			
			if(hdrPos) {
				// TODO - not sure what we actually do in this condition
				_changeAppr = true;
			} else if(fencePos) {
				// no-op?
			} else if(appWp) {
				ShowScratchpadMessage("Invald", " Add  ");
			} else {
				if((_wLinePos + 1) > _entWpStr.size()) {
					_entWpStr += '9';
				} else {
					_entWpStr[_wLinePos] = _kln89->DecChar(_entWpStr[_wLinePos], (_wLinePos == 0 ? false : true));
				}
				_bEntWp = true;
				_fp0SelWpId.clear();	// Waypoints don't become the DTO default whilst being entered.
				
				bool multi;
				const GPSWaypoint* wp = _kln89->FindFirstById(_entWpStr.substr(0, _wLinePos+1), multi, false);
				if(NULL == wp) {
					// no-op
				} else {
					if(_entWp == NULL) {
						_entWp = new GPSWaypoint;
						if(_fplPos + (_uLinePos - 4) >= _kln89->_flightPlans[_subPage]->waypoints.size()) {
							_kln89->_flightPlans[_subPage]->waypoints.push_back(_entWp);
						} else {
							_kln89->_flightPlans[_subPage]->waypoints.insert(_kln89->_flightPlans[_subPage]->waypoints.begin()+(_fplPos + (_uLinePos - 4)), _entWp);
						}
					}
					// copy
					*_entWp = *wp;
				}
			}
		}
	}
}
