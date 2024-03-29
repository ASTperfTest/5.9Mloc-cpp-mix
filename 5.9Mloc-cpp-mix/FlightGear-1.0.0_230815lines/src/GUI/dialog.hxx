// dialog.hxx - XML-configured dialog box.

#ifndef __DIALOG_HXX
#define __DIALOG_HXX 1

#ifndef __cplusplus
# error This library requires C++
#endif

#include <plib/puAux.h>
#include <plib/sg.h>

#include <simgear/compiler.h>	// for SG_USING_STD
#include <simgear/props/props.hxx>
#include <simgear/misc/sg_path.hxx>

#undef PUCLASS_LIST
#include "puList.hxx"

#include <vector>
SG_USING_STD(vector);


// ugly temproary workaround for plib's lack of user defined class ids  FIXME
#define FGCLASS_LIST          0x00000001
#define FGCLASS_AIRPORTLIST   0x00000002
#define FGCLASS_PROPERTYLIST  0x00000004
class GUI_ID { public: GUI_ID(int id) : id(id) {} int id; };



class FGDialog;
class FGBinding;
class NewGUI;
class FGColor;


/**
 * An XML-configured dialog box.
 *
 * The GUI manager stores only the property tree for the dialog
 * boxes.  This class creates a PUI dialog box on demand from
 * the properties in that tree.  The manager recreates the dialog
 * every time it needs to show it.
 */
class FGDialog
{
public:

    /**
     * Construct a new GUI widget configured by a property tree.
     *
     * The configuration properties are not part of the main
     * FlightGear property tree; the GUI manager reads them
     * from individual configuration files.
     *
     * @param props A property tree describing the dialog.
     */
    FGDialog (SGPropertyNode * props);


    /**
     * Destructor.
     */
    virtual ~FGDialog ();


    /**
     * Update the values of all GUI objects with a specific name,
     * or all if name is 0 (default).
     *
     * This method copies values from the FlightGear property tree to
     * the GUI object(s).
     *
     * @param objectName The name of the GUI object(s) to update.
     *        Use the empty name for all unnamed objects.
     */
    virtual void updateValues (const char * objectName = 0);


    /**
     * Apply the values of all GUI objects with a specific name,
     * or all if name is 0 (default)
     *
     * This method copies values from the GUI object(s) to the
     * FlightGear property tree.
     *
     * @param objectName The name of the GUI object(s) to update.
     *        Use the empty name for all unnamed objects.
     */
    virtual void applyValues (const char * objectName = 0);


    /**
     * Update state.  Called on active dialogs before rendering.
     */
    virtual void update ();

private:

    enum {
        BACKGROUND = 0x01,
        FOREGROUND = 0x02,
        HIGHLIGHT = 0x04,
        LABEL = 0x08,
        LEGEND = 0x10,
        MISC = 0x20,
        EDITFIELD = 0x40
    };

    // Private copy constructor to avoid unpleasant surprises.
    FGDialog (const FGDialog &);

    // Show the dialog.
    void display (SGPropertyNode * props);

    // Build the dialog or a subobject of it.
    puObject * makeObject (SGPropertyNode * props,
                           int parentWidth, int parentHeight);

    // Common configuration for all GUI objects.
    void setupObject (puObject * object, SGPropertyNode * props);

    // Common configuration for all GUI group objects.
    void setupGroup (puGroup * group, SGPropertyNode * props,
                     int width, int height, bool makeFrame = false);

    // Set object colors: the "which" argument defines which color qualities
    // (PUCOL_LABEL, etc.) should pick up the <color> property.
    void setColor(puObject * object, SGPropertyNode * props, int which = 0);

    // return key code number for keystring
    int getKeyCode(const char *keystring);

    // The top-level PUI object.
    puObject * _object;

    // The GUI subsystem.
    NewGUI * _gui;

    // The dialog font. Defaults to the global gui font, but can get
    // overridden by a top level font definition.
    puFont * _font;

    // The source xml tree, so that we can pass data back, such as the
    // last position.
    SGPropertyNode_ptr _props;

    // Nasal module.
    string _module;
    SGPropertyNode_ptr _nasal_close;

    // PUI provides no way for userdata to be deleted automatically
    // with a GUI object, so we have to keep track of all the special
    // data we allocated and then free it manually when the dialog
    // closes.
    vector<void *> _info;
    struct PropertyObject {
        PropertyObject (const char * name,
                        puObject * object,
                        SGPropertyNode_ptr node);
        string name;
        puObject * object;
        SGPropertyNode_ptr node;
    };
    vector<PropertyObject *> _propertyObjects;
    vector<PropertyObject *> _liveObjects;
};

//
// Custom subclass of puPopup to implement "draggable" windows in the
// interface.  Note that this is a subclass of puPopup, not
// puDialogBox.  Sadly, PUI (mis)uses subclassing to implement a
// boolean property: modality.  That means that we can't implement
// dragging of both puPopup windows and puDialogBoxes with the same
// code.  Rather than duplicate code, I've chosen to implement only
// "non-modal dragability" here.  Modal dialog boxes (like the exit
// confirmation) are not draggable.
//
class fgPopup : public puPopup {
public:
    fgPopup(int x, int y, bool d = true) : puPopup(x, y) { _dragging = false; _draggable = d;}
    int checkHit(int b, int up, int x, int y);
    int checkKey(int key, int updown);
    int getHitObjects(puObject *, int x, int y);
    puObject *getKeyObject(puObject *, int key);
    puObject *getActiveInputField(puObject *);
private:
    bool _draggable;
    bool _dragging;
    int _dX, _dY;
};


class fgValueList {
public:
    fgValueList(SGPropertyNode *p);
    virtual ~fgValueList();
    virtual void update();

protected:
    char **_list;

private:
    void make_list();
    void destroy_list();
    SGPropertyNode_ptr _props;
};


class fgList : public fgValueList, public puList, public GUI_ID {
public:
    fgList(int x1, int y1, int x2, int y2, SGPropertyNode *p, int sw) :
            fgValueList(p), puList(x1, y1, x2, y2, _list, sw), GUI_ID(FGCLASS_LIST) {}
    void update();
};

class fgComboBox : public fgValueList, public puaComboBox {
public:
    fgComboBox(int x1, int y1, int x2, int y2, SGPropertyNode *p, bool editable) :
        fgValueList(p), puaComboBox(x1, y1, x2, y2, _list, editable) {}
};

class fgSelectBox : public fgValueList, public puaSelectBox {
public:
    fgSelectBox(int x1, int y1, int x2, int y2, SGPropertyNode *p) :
        fgValueList(p), puaSelectBox(x1, y1, x2, y2, _list) {}
};

#endif // __DIALOG_HXX
