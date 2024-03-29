/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 File:         FGXMLElement.h
 Author:       Jon S. Berndt
 Date started: 9/28/04

 ------------- Copyright (C) 2004  Jon S. Berndt (jsb@hal-pc.org) -------------

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place - Suite 330, Boston, MA  02111-1307, USA.

 Further information about the GNU Lesser General Public License can also be found on
 the world wide web at http://www.gnu.org.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef XMLELEMENT_H
#define XMLELEMENT_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef FGFS
#  include <simgear/compiler.h>
#  ifdef SG_HAVE_STD_INCLUDES
#    include <string>
#    include <vector>
#    include <iostream>
#    include <map>
#  else
#    include <vector.h>
#    include <string>
#    include <iostream.h>
#    include <map.h>
#  endif
#else
#  include <string>
#  include <map>
#  include <iostream>
#  include <vector>
   using std::string;
   using std::map;
   using std::vector;
   using std::cout;
   using std::endl;
#endif
   using std::string;
   using std::map;
   using std::vector;
   using std::cout;
   using std::endl;

#include <math/FGColumnVector3.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define ID_XMLELEMENT "$Id: FGXMLElement.h,v 1.1.2.1 2007-01-06 10:49:24 ehofman Exp $"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace JSBSim {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Encapsulates an XML element.
    This class handles the creation, storage, and manipulation of XML elements.
    This class also can convert supplied values as follows:

    convert ["from"]["to"] = factor, so: from * factor = to
    - convert["M"]["FT"] = 3.2808399;
    - convert["FT"]["M"] = 1.0/convert["M"]["FT"];
    - convert["M2"]["FT2"] = convert["M"]["FT"]*convert["M"]["FT"];
    - convert["FT2"]["M2"] = 1.0/convert["M2"]["FT2"];
    - convert["FT"]["IN"] = 12.0;
    - convert["IN"]["FT"] = 1.0/convert["FT"]["IN"];
    - convert["LBS"]["KG"] = 0.45359237;
    - convert["KG"]["LBS"] = 1.0/convert["LBS"]["KG"];
    - convert["SLUG*FT2"]["KG*M2"] = 1.35594;
    - convert["KG*M2"]["SLUG*FT2"] = 1.0/convert["SLUG*FT2"]["KG*M2"];
    - convert["RAD"]["DEG"] = 360.0/(2.0*3.1415926);
    - convert["DEG"]["RAD"] = 1.0/convert["RAD"]["DEG"];
    - convert["LBS/FT"]["N/M"] = 14.5939;
    - convert["LBS/FT/SEC"]["N/M/SEC"] = 14.5939;
    - convert["N/M"]["LBS/FT"] = 1.0/convert["LBS/FT"]["N/M"];
    - convert["N/M/SEC"]["LBS/FT/SEC"] = 1.0/convert["LBS/FT/SEC"]["N/M/SEC"];
    - convert["WATTS"]["HP"] = 0.001341022;
    - convert["HP"]["WATTS"] = 1.0/convert["WATTS"]["HP"];
    - convert["N"]["LBS"] = 0.22482;
    - convert["LBS"]["N"] = 1.0/convert["N"]["LBS"];
    - convert["KTS"]["FT/SEC"] = ktstofps;

    - convert["M"]["M"] = 1.00;
    - convert["FT"]["FT"] = 1.00;
    - convert["IN"]["IN"] = 1.00;
    - convert["DEG"]["DEG"] = 1.00;
    - convert["RAD"]["RAD"] = 1.00;
    - convert["M2"]["M2"] = 1.00;
    - convert["FT2"]["FT2"] = 1.00;
    - convert["KG*M2"]["KG*M2"] = 1.00;
    - convert["SLUG*FT2"]["SLUG*FT2"] = 1.00;
    - convert["KG"]["KG"] = 1.00;
    - convert["LBS"]["LBS"] = 1.00;
    - convert["LBS/FT"]["LBS/FT"] = 1.00;
    - convert["N/M"]["N/M"] = 1.00;
    - convert["LBS/FT/SEC"]["LBS/FT/SEC"] = 1.00;
    - convert["N/M/SEC"]["N/M/SEC"] = 1.00;
    - convert["PSI"]["PSI"] = 1.00;
    - convert["INHG"]["INHG"] = 1.00;
    - convert["HP"]["HP"] = 1.00;
    - convert["N"]["N"] = 1.00;
    - convert["WATTS"]["WATTS"] = 1.00;
    - convert["KTS"]["KTS"] = 1.0;
    - convert["FT/SEC"]["FT/SEC"] = 1.0;

    Where:
    - N = newtons
    - M = meters
    - M2 = meters squared
    - KG = kilograms
    - LBS = pounds force
    - FT = feet
    - FT2 = feet squared
    - SEC = seconds
    - SLUG = slug
    - DEG = degrees
    - RAD = radians
    - WATTS = watts

    @author Jon S. Berndt
    @version $Id: FGXMLElement.h,v 1.1.2.1 2007-01-06 10:49:24 ehofman Exp $
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Element {
public:
  /** Constructor
      @param nm the name of this element (if given)
      */
  Element(string nm);
  /// Destructor
  ~Element(void);

  /** Retrieves an attribute.
      @param key specifies the attribute key to retrieve the value of.
      @return the key value (as a string), or the empty string if no such
              attribute exists. */
  string GetAttributeValue(string key);

  /** Retrieves an attribute value as a double precision real number.
      @param key specifies the attribute key to retrieve the value of.
      @return the key value (as a number), or the HUGE_VAL if no such
              attribute exists. */
  double GetAttributeValueAsNumber(string key);

  /** Retrieves the element name.
      @return the element name, or the empty string if no name has been set.*/
  string GetName(void) {return name;}

  /** Gets a line of data belonging to an element.
      @param i the index of the data line to return (0 by default).
      @return a string representing the data line requested, or the empty string
              if none exists.*/
  string GetDataLine(unsigned int i=0);

  /// Returns the number of lines of data stored
  unsigned int GetNumDataLines(void) {return (unsigned int)data_lines.size();}

  /// Returns the number of child elements for this element.
  unsigned int GetNumElements(void) {return (unsigned int)children.size();}

  /// Returns the number of named child elements for this element.
  unsigned int GetNumElements(string);

  /** Converts the element data to a number.
      This function attempts to convert the first (and presumably only) line of
      data "owned" by the element into a real number. If there is not exactly one
      line of data owned by the element, then HUGE_VAL is returned.
      @return the numeric value of the data owned by the element.*/
  double GetDataAsNumber(void);

  /** Returns a pointer to the element requested by index.
      This function also resets an internal counter to the index, so that
      subsequent calls to GetNextElement() will return the following
      elements sequentially, until the last element is reached. At that point,
      GetNextElement() will return NULL.
      @param el the index of the requested element (0 by default)
      @return a pointer to the Element, or 0 if no valid element exists. */
  Element* GetElement(unsigned int el=0);

  /** Returns a pointer to the next element in the list.
      The function GetElement() must be called first to be sure that this
      function will return the correct element. The call to GetElement() resets
      the internal counter to zero. Subsequent calls to GetNextElement() return
      a pointer to subsequent elements in the list. When the final element is
      reached, 0 is returned.
      @return a pointer to the next Element in the sequence, or 0 if no valid
              Element is present. */
  Element* GetNextElement(void);

  /** Returns a pointer to the parent of an element.
      @return a pointer to the parent Element, or 0 if this is the top level Element. */
  Element* GetParent(void) {return parent;}

  /** Searches for a specified element.
      Finds the first element that matches the supplied string, or simply the first
      element if no search string is supplied. This function call resets the internal
      element counter to the first element.
      @param el the search string (empty string by default).
      @return a pointer to the first element that matches the supplied search string. */
  Element* FindElement(string el="");

  /** Searches for the next element as specified.
      This function would be called after FindElement() is first called (in order to
      reset the internal counter). If no argument is supplied (or the empty string)
      a pointer to the very next element is returned. Otherwise, the next occurence
      of the named element is returned. If the end of the list is reached, 0 is
      returned.
      @param el the name of the next element to find.
      @return the pointer to the found element, or 0 if no appropriate element us
              found.*/
  Element* FindNextElement(string el="");

  /** Searches for the named element and returns the string data belonging to it.
      This function allows the data belonging to a named element to be returned
      as a string. If no element is found, the empty string is returned. If no
      argument is supplied, the data string for the first element is returned.
      @param el the name of the element being searched for (the empty string by
      default)
      @return the data value for the named element as a string, or the empty
              string if the element cannot be found. */
  string FindElementValue(string el="");

  /** Searches for the named element and returns the data belonging to it as a number.
      This function allows the data belonging to a named element to be returned
      as a double. If no element is found, HUGE_VAL is returned. If no
      argument is supplied, the data for the first element is returned.
      @param el the name of the element being searched for (the empty string by
      default)
      @return the data value for the named element as a double, or HUGE_VAL if the
              data is missing. */
  double FindElementValueAsNumber(string el="");

  /** Searches for the named element and converts and returns the data belonging to it.
      This function allows the data belonging to a named element to be returned
      as a double. If no element is found, HUGE_VAL is returned. If no
      argument is supplied, the data for the first element is returned. Additionally,
      this function converts the value from the units specified in the config file (via
      the UNITS="" attribute in the element definition) to the native units used by
      JSBSim itself, as specified by the target_units parameter. The currently
      allowable unit conversions are seen in the source file FGXMLElement.cpp.
      Also, see above in the main documentation for this class.
      @param el the name of the element being searched for (the empty string by
      default)
      @param target_units the string representing the native units used by JSBSim
             to which the value returned will be converted.
      @return the unit-converted data value for the named element as a double,
              or HUGE_VAL if the data is missing. */
  double FindElementValueAsNumberConvertTo(string el, string target_units);

  /** Searches for the named element and converts and returns the data belonging to it.
      This function allows the data belonging to a named element to be returned
      as a double. If no element is found, HUGE_VAL is returned. If no
      argument is supplied, the data for the first element is returned. Additionally,
      this function converts the value from the units specified in the supplied_units
      parameter to the units specified in the target_units parameter. JSBSim itself,
      as specified by the target_units parameter. The currently allowable unit
      conversions are seen in the source file FGXMLElement.cpp. Also, see above
      in the main documentation for this class.
      @param el the name of the element being searched for (the empty string by
      default)
      @param supplied_units the string representing the units of the value as
             supplied by the config file.
      @param target_units the string representing the native units used by JSBSim
             to which the value returned will be converted.
      @return the unit-converted data value for the named element as a double,
              or HUGE_VAL if the data is missing. */
  double FindElementValueAsNumberConvertFromTo( string el,
                                                string supplied_units,
                                                string target_units);

  /** Composes a 3-element column vector for the supplied location or orientation.
      This function processes a LOCATION or ORIENTATION construct, returning a
      filled-out 3-element column vector containing the X, Y, Z or ROLL, PITCH,
      YAW elements found in the supplied element. If one of the mentioned components
      is not found, that component is set to zero and a warning message is printed.
      All three elements should be supplied.
      @param target_units the string representing the native units used by JSBSim
             to which the value returned will be converted.
      @return a column vector object built from the LOCATION or ORIENT components. */
  FGColumnVector3 FindElementTripletConvertTo( string target_units);

  /** This function sets the value of the parent class attribute to the supplied
      Element pointer.
      @param p pointer to the parent Element. */
  void SetParent(Element* p) {parent = p;}

  /** Adds a child element to the list of children stored for this element.
  *   @param el Child element to add. */
  void AddChildElement(Element* el) {children.push_back(el);}

  /** Stores an attribute belonging to this element.
  *   @param name The string name of the attribute.
  *   @param value The string value of the attribute. */
  void AddAttribute(string name, string value);

  /** Stores data belonging to this element.
  *   @param d the data to store. */
  void AddData(string d);

  /** Prints the element.
  *   Prints this element and calls the Print routine for child elements.
  *   @param d The tab level. A level corresponds to a single space. */
  void Print(unsigned int level=0);

private:
  string name;
  map <string, string> attributes;
  vector <string> data_lines;
  vector <Element*> children;
  vector <string> attribute_key;
  Element *parent;
  unsigned int element_index;
  typedef map <string, map <string, double> > tMapConvert;
  tMapConvert convert;
};

} // namespace JSBSim

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif
