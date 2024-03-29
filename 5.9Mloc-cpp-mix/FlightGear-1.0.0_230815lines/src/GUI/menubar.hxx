// menubar.hxx - XML-configured menu bar.

#ifndef __MENUBAR_HXX
#define __MENUBAR_HXX 1

#ifndef __cplusplus
# error This library requires C++
#endif

#include <simgear/compiler.h>	// for SG_USING_STD
#include <Main/fg_props.hxx>

#include <plib/pu.h>

#include <map>
SG_USING_STD(map);

#include <vector>
SG_USING_STD(vector);


class puMenuBar;
class puObject;
class FGBinding;


/**
 * XML-configured PUI menu bar.
 *
 * This class creates a menu bar from a tree of XML properties.  These
 * properties are not part of the main FlightGear property tree, but
 * are read from a separate file ($FG_ROOT/gui/menubar.xml).
 *
 * WARNING: because PUI provides no easy way to attach user data to a
 * menu item, all menu item strings must be unique; otherwise, this
 * class will always use the first binding with any given name.
 */
class FGMenuBar
{
public:

    /**
     * Constructor.
     */
    FGMenuBar ();


    /**
     * Destructor.
     */
    virtual ~FGMenuBar ();


    /**
     * Initialize the menu bar from $FG_ROOT/gui/menubar.xml
     */
    virtual void init ();
    
    /**
     * Make the menu bar visible.
     */
    virtual void show ();


    /**
     * Make the menu bar invisible.
     */
    virtual void hide ();


    /**
     * Test whether the menu bar is visible.
     */
    virtual bool isVisible () const;


    /**
     * IGNORE THIS METHOD!!!
     *
     * This is necessary only because plib does not provide any easy
     * way to attach user data to a menu item.  FlightGear should not
     * have to know about PUI internals, but this method allows the
     * callback to pass the menu item one-shot on to the current menu.
     */
    virtual void fireItem (puObject * item);


    /**
     * create a menubar based on a PropertyList within the PropertyTree
     */
    void make_menubar (SGPropertyNode * props);


    /**
     * destroy a menubar based on a PropertyList within the PropertyTree
     */
    void destroy_menubar ();


    /**
     * Disable/enable menu titles and entries
     */
    bool enable_item (const SGPropertyNode * item, bool state);


private:

    // Make a single menu.
    void make_menu (SGPropertyNode * node);

    // Make the top-level menubar.
    void make_menubar ();

    // Create a property-path -> puObject map for menu node
    void make_object_map(SGPropertyNode * node);

    // Add <enabled> listener that enables/disables menu entries.
    void add_enabled_listener(SGPropertyNode * node);

    // Is the menu visible?
    bool _visible;

    // The top-level menubar itself.
    puMenuBar * _menuBar;

    // A map of bindings for the menubar.
    map<string,vector<FGBinding *> > _bindings;

    // These are hoops that we have to jump through because PUI doesn't
    // do memory management for lists.  We have to allocate the arrays,
    // hang onto pointers, and then delete them when the menubar is
    // freed.
    char ** make_char_array (int size);
    puCallback * make_callback_array (int size);
    vector<char **> _char_arrays;
    vector<puCallback *> _callback_arrays;

    // A map for {menu node path}->puObject translation.
    map<string, puObject *> _objects;
};

#endif // __MENUBAR_HXX
