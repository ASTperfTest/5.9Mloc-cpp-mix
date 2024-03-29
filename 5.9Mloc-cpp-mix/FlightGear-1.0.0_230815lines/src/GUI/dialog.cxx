// dialog.cxx: implementation of an XML-configurable dialog box.

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <Input/input.hxx>
#include <Scripting/NasalSys.hxx>

#include "dialog.hxx"
#include "new_gui.hxx"
#include "AirportList.hxx"
#include "property_list.hxx"
#include "layout.hxx"


enum format_type { f_INVALID, f_INT, f_LONG, f_FLOAT, f_DOUBLE, f_STRING };
static const int FORMAT_BUFSIZE = 255;

/**
 * Makes sure the format matches '%[ -+#]?\d*(\.\d*)?(l?[df]|s)', with
 * only one number or string placeholder and otherwise arbitrary prefix
 * and postfix containing only quoted percent signs (%%).
 */
static format_type
validate_format(const char *f)
{
    bool l = false;
    format_type type;
    for (; *f; f++) {
        if (*f == '%') {
            if (f[1] == '%')
                f++;
            else
                break;
        }
    }
    if (*f++ != '%')
        return f_INVALID;
    while (*f == ' ' || *f == '+' || *f == '-' || *f == '#' || *f == '0')
        f++;
    while (*f && isdigit(*f))
        f++;
    if (*f == '.') {
        f++;
        while (*f && isdigit(*f))
            f++;
    }

    if (*f == 'l')
        l = true, f++;

    if (*f == 'd') {
        type = l ? f_LONG : f_INT;
    } else if (*f == 'f')
        type = l ? f_DOUBLE : f_FLOAT;
    else if (*f == 's') {
        if (l)
            return f_INVALID;
        type = f_STRING;
    } else
        return f_INVALID;

    for (++f; *f; f++) {
        if (*f == '%') {
            if (f[1] == '%')
                f++;
            else
                return f_INVALID;
        }
    }
    return type;
}


////////////////////////////////////////////////////////////////////////
// Implementation of GUIInfo.
////////////////////////////////////////////////////////////////////////

/**
 * User data for a GUI object.
 */
struct GUIInfo
{
    GUIInfo(FGDialog * d);
    virtual ~GUIInfo();
    char *format(SGPropertyNode *);

    FGDialog * dialog;
    vector <FGBinding *> bindings;
    int key;
    char *text;
    char *fmt_string;
    format_type fmt_type;
};

GUIInfo::GUIInfo (FGDialog * d)
    : dialog(d),
      key(-1),
      text(0),
      fmt_string(0),
      fmt_type(f_INVALID)
{
}

GUIInfo::~GUIInfo ()
{
    for (unsigned int i = 0; i < bindings.size(); i++) {
        delete bindings[i];
        bindings[i] = 0;
    }
    delete [] text;
    delete [] fmt_string;
}

char *GUIInfo::format(SGPropertyNode *n)
{
    if (fmt_type == f_INT)
        snprintf(text, FORMAT_BUFSIZE, fmt_string, n->getIntValue());
    else if (fmt_type == f_LONG)
        snprintf(text, FORMAT_BUFSIZE, fmt_string, n->getLongValue());
    else if (fmt_type == f_FLOAT)
        snprintf(text, FORMAT_BUFSIZE, fmt_string, n->getFloatValue());
    else if (fmt_type == f_DOUBLE)
        snprintf(text, FORMAT_BUFSIZE, fmt_string, n->getDoubleValue());
    else
        snprintf(text, FORMAT_BUFSIZE, fmt_string, n->getStringValue());

    text[FORMAT_BUFSIZE] = '\0';
    return text;
}



/**
 * Key handler.
 */
int fgPopup::checkKey(int key, int updown)
{
    if (updown == PU_UP || !isVisible() || !isActive() || window != puGetWindow())
        return false;

    puObject *input = getActiveInputField(this);
    if (input)
        return input->checkKey(key, updown);

    puObject *object = getKeyObject(this, key);
    if (!object)
        return puPopup::checkKey(key, updown);

    // invokeCallback() isn't enough; we need to simulate a mouse button press
    object->checkHit(PU_LEFT_BUTTON, PU_DOWN,
            (object->getABox()->min[0] + object->getABox()->max[0]) / 2,
            (object->getABox()->min[1] + object->getABox()->max[1]) / 2);
    object->checkHit(PU_LEFT_BUTTON, PU_UP,
            (object->getABox()->min[0] + object->getABox()->max[0]) / 2,
            (object->getABox()->min[1] + object->getABox()->max[1]) / 2);
    return true;
}

puObject *fgPopup::getKeyObject(puObject *object, int key)
{
    puObject *ret;
    if (object->getType() & PUCLASS_GROUP)
        for (puObject *obj = ((puGroup *)object)->getFirstChild();
                obj; obj = obj->getNextObject())
            if ((ret = getKeyObject(obj, key)))
                return ret;

    GUIInfo *info = (GUIInfo *)object->getUserData();
    if (info && info->key == key)
        return object;

    return 0;
}

puObject *fgPopup::getActiveInputField(puObject *object)
{
    puObject *ret;
    if (object->getType() & PUCLASS_GROUP)
        for (puObject *obj = ((puGroup *)object)->getFirstChild();
                obj; obj = obj->getNextObject())
            if ((ret = getActiveInputField(obj)))
                return ret;

    if (object->getType() & PUCLASS_INPUT && ((puInput *)object)->isAcceptingInput())
        return object;

    return 0;
}

/**
 * Mouse handler.
 */
int fgPopup::checkHit(int button, int updown, int x, int y)
{
    int result = puPopup::checkHit(button, updown, x, y);

    if (!_draggable)
       return result;

    // This is annoying.  We would really want a true result from the
    // superclass to indicate "handled by child object", but all it
    // tells us is that the pointer is inside the dialog.  So do the
    // intersection test (again) to make sure we don't start a drag
    // when inside controls.

    if (updown == PU_DOWN && !_dragging) {
        if (!result)
            return 0;

        int hit = getHitObjects(this, x, y);
        if (hit & (PUCLASS_BUTTON|PUCLASS_ONESHOT|PUCLASS_INPUT))
            return result;

        int px, py;
        getPosition(&px, &py);
        _dragging = true;
        _dX = px - x;
        _dY = py - y;
    } else if (updown == PU_DRAG && _dragging) {
        setPosition(x + _dX, y + _dY);
    } else {
        _dragging = false;
    }
    return result;
}

int fgPopup::getHitObjects(puObject *object, int x, int y)
{
    if (!object->isVisible())
        return 0;

    int type = 0;
    if (object->getType() & PUCLASS_GROUP)
        for (puObject *obj = ((puGroup *)object)->getFirstChild();
                obj; obj = obj->getNextObject())
            type |= getHitObjects(obj, x, y);

    int cx, cy, cw, ch;
    object->getAbsolutePosition(&cx, &cy);
    object->getSize(&cw, &ch);
    if (x >= cx && x < cx + cw && y >= cy && y < cy + ch)
        type |= object->getType();
    return type;
}



////////////////////////////////////////////////////////////////////////
// Callbacks.
////////////////////////////////////////////////////////////////////////

/**
 * Action callback.
 */
static void
action_callback (puObject * object)
{
    GUIInfo * info = (GUIInfo *)object->getUserData();
    NewGUI * gui = (NewGUI *)globals->get_subsystem("gui");
    gui->setActiveDialog(info->dialog);
    int nBindings = info->bindings.size();
    for (int i = 0; i < nBindings; i++) {
        info->bindings[i]->fire();
        if (gui->getActiveDialog() == 0)
            break;
    }
    gui->setActiveDialog(0);
}



////////////////////////////////////////////////////////////////////////
// Static helper functions.
////////////////////////////////////////////////////////////////////////

/**
 * Copy a property value to a PUI object.
 */
static void
copy_to_pui (SGPropertyNode * node, puObject * object)
{
    // Treat puText objects specially, so their "values" can be set
    // from properties.
    if (object->getType() & PUCLASS_TEXT) {
        GUIInfo *info = (GUIInfo *)object->getUserData();
        if (info && info->fmt_string)
            object->setLabel(info->format(node));
        else
            object->setLabel(node->getStringValue());
        return;
    }

    switch (node->getType()) {
    case SGPropertyNode::BOOL:
    case SGPropertyNode::INT:
    case SGPropertyNode::LONG:
        object->setValue(node->getIntValue());
        break;
    case SGPropertyNode::FLOAT:
    case SGPropertyNode::DOUBLE:
        object->setValue(node->getFloatValue());
        break;
    default:
        object->setValue(node->getStringValue());
        break;
    }
}


static void
copy_from_pui (puObject * object, SGPropertyNode * node)
{
    // puText objects are immutable, so should not be copied out
    if (object->getType() & PUCLASS_TEXT)
        return;

    switch (node->getType()) {
    case SGPropertyNode::BOOL:
    case SGPropertyNode::INT:
    case SGPropertyNode::LONG:
        node->setIntValue(object->getIntegerValue());
        break;
    case SGPropertyNode::FLOAT:
    case SGPropertyNode::DOUBLE:
        node->setFloatValue(object->getFloatValue());
        break;
    default:
        // Special case to handle lists, as getStringValue cannot be overridden
        if(object->getType() & PUCLASS_LIST)
        {
            const char *s = ((puList *) object)->getListStringValue();
            if (s)
                node->setStringValue(s);
        }
        else
        {
            node->setStringValue(object->getStringValue());
        }
        break;
    }
}



////////////////////////////////////////////////////////////////////////
// Implementation of FGDialog.
////////////////////////////////////////////////////////////////////////

FGDialog::FGDialog (SGPropertyNode * props)
    : _object(0),
      _gui((NewGUI *)globals->get_subsystem("gui")),
      _props(props)
{
    _module = string("__dlg:") + props->getStringValue("name", "[unnamed]");
    SGPropertyNode *nasal = props->getNode("nasal");
    if (nasal) {
        _nasal_close = nasal->getNode("close");
        SGPropertyNode *open = nasal->getNode("open");
        if (open) {
            const char *s = open->getStringValue();
            FGNasalSys *nas = (FGNasalSys *)globals->get_subsystem("nasal");
            nas->createModule(_module.c_str(), _module.c_str(), s, strlen(s), props);
        }
    }
    display(props);
}

FGDialog::~FGDialog ()
{
    int x, y;
    _object->getAbsolutePosition(&x, &y);
    _props->setIntValue("lastx", x);
    _props->setIntValue("lasty", y);

    FGNasalSys *nas = (FGNasalSys *)globals->get_subsystem("nasal");
    if (_nasal_close) {
        const char *s = _nasal_close->getStringValue();
        nas->createModule(_module.c_str(), _module.c_str(), s, strlen(s), _props);
    }
    nas->deleteModule(_module.c_str());

    puDeleteObject(_object);

    unsigned int i;
                                // Delete all the info objects we
                                // were forced to keep around because
                                // PUI cannot delete its own user data.
    for (i = 0; i < _info.size(); i++) {
        delete (GUIInfo *)_info[i];
        _info[i] = 0;
    }
                                // Finally, delete the property links.
    for (i = 0; i < _propertyObjects.size(); i++) {
        delete _propertyObjects[i];
        _propertyObjects[i] = 0;
    }
}

void
FGDialog::updateValues (const char * objectName)
{
    if (objectName && !objectName[0])
        objectName = 0;

    for (unsigned int i = 0; i < _propertyObjects.size(); i++) {
        const string &name = _propertyObjects[i]->name;
        if (objectName && name != objectName)
            continue;

        puObject *obj = _propertyObjects[i]->object;
        if ((obj->getType() & PUCLASS_LIST) && (dynamic_cast<GUI_ID *>(obj)->id & FGCLASS_LIST)) {
            fgList *pl = static_cast<fgList *>(obj);
            pl->update();
        } else
            copy_to_pui(_propertyObjects[i]->node, obj);
    }
}

void
FGDialog::applyValues (const char * objectName)
{
    if (objectName && !objectName[0])
        objectName = 0;

    for (unsigned int i = 0; i < _propertyObjects.size(); i++) {
        const string &name = _propertyObjects[i]->name;
        if (objectName && name != objectName)
            continue;

        copy_from_pui(_propertyObjects[i]->object,
                      _propertyObjects[i]->node);
    }
}

void
FGDialog::update ()
{
    for (unsigned int i = 0; i < _liveObjects.size(); i++) {
        puObject *obj = _liveObjects[i]->object;
        if (obj->getType() & PUCLASS_INPUT && ((puInput *)obj)->isAcceptingInput())
            continue;

        copy_to_pui(_liveObjects[i]->node, obj);
    }
}

void
FGDialog::display (SGPropertyNode * props)
{
    if (_object != 0) {
        SG_LOG(SG_GENERAL, SG_ALERT, "This widget is already active");
        return;
    }

    int screenw = globals->get_props()->getIntValue("/sim/startup/xsize");
    int screenh = globals->get_props()->getIntValue("/sim/startup/ysize");

    bool userx = props->hasValue("x");
    bool usery = props->hasValue("y");
    bool userw = props->hasValue("width");
    bool userh = props->hasValue("height");

    // Let the layout widget work in the same property subtree.
    LayoutWidget wid(props);

    SGPropertyNode *fontnode = props->getNode("font");
    if (fontnode) {
        FGFontCache *fc = globals->get_fontcache();
        _font = fc->get(fontnode);
    } else {
        _font = _gui->getDefaultFont();
    }
    wid.setDefaultFont(_font, int(_font->getPointSize()));

    int pw=0, ph=0;
    int px, py, savex, savey;
    if(!userw || !userh)
        wid.calcPrefSize(&pw, &ph);
    pw = props->getIntValue("width", pw);
    ph = props->getIntValue("height", ph);
    px = savex = props->getIntValue("x", (screenw - pw) / 2);
    py = savey = props->getIntValue("y", (screenh - ph) / 2);

    // Negative x/y coordinates are interpreted as distance from the top/right
    // corner rather than bottom/left.
    if (userx && px < 0)
        px = screenw - pw + px;
    if (usery && py < 0)
        py = screenh - ph + py;

    // Define "x", "y", "width" and/or "height" in the property tree if they
    // are not specified in the configuration file.
    wid.layout(px, py, pw, ph);

    // Use the dimension and location properties as specified in the
    // configuration file or from the layout widget.
    _object = makeObject(props, screenw, screenh);

    // Remove automatically generated properties, so the layout looks
    // the same next time around, or restore x and y to preserve negative coords.
    if(userx)
        props->setIntValue("x", savex);
    else
        props->removeChild("x");

    if(usery)
        props->setIntValue("y", savey);
    else
        props->removeChild("y");

    if(!userw) props->removeChild("width");
    if(!userh) props->removeChild("height");

    if (_object != 0) {
        _object->reveal();
    } else {
        SG_LOG(SG_GENERAL, SG_ALERT, "Widget "
               << props->getStringValue("name", "[unnamed]")
               << " does not contain a proper GUI definition");
    }
}

puObject *
FGDialog::makeObject (SGPropertyNode * props, int parentWidth, int parentHeight)
{
    if (props->getBoolValue("hide"))
        return 0;

    bool presetSize = props->hasValue("width") && props->hasValue("height");
    int width = props->getIntValue("width", parentWidth);
    int height = props->getIntValue("height", parentHeight);
    int x = props->getIntValue("x", (parentWidth - width) / 2);
    int y = props->getIntValue("y", (parentHeight - height) / 2);
    string type = props->getName();

    if (type.empty())
        type = "dialog";

    if (type == "dialog") {
        puPopup * obj;
        bool draggable = props->getBoolValue("draggable", true);
        if (props->getBoolValue("modal", false))
            obj = new puDialogBox(x, y);
        else
            obj = new fgPopup(x, y, draggable);
        setupGroup(obj, props, width, height, true);
        setColor(obj, props);
        return obj;

    } else if (type == "group") {
        puGroup * obj = new puGroup(x, y);
        setupGroup(obj, props, width, height, false);
        setColor(obj, props);
        return obj;

    } else if (type == "frame") {
        puGroup * obj = new puGroup(x, y);
        setupGroup(obj, props, width, height, true);
        setColor(obj, props);
        return obj;

    } else if (type == "hrule" || type == "vrule") {
        puFrame * obj = new puFrame(x, y, x + width, y + height);
        obj->setBorderThickness(0);
        setColor(obj, props, BACKGROUND|FOREGROUND|HIGHLIGHT);
        return obj;

    } else if (type == "list") {
        int slider_width = props->getIntValue("slider", 20);
        fgList * obj = new fgList(x, y, x + width, y + height, props, slider_width);
        if (presetSize)
            obj->setSize(width, height);
        setupObject(obj, props);
        setColor(obj, props);
        return obj;

    } else if (type == "airport-list") {
        AirportList * obj = new AirportList(x, y, x + width, y + height);
        if (presetSize)
            obj->setSize(width, height);
        setupObject(obj, props);
        setColor(obj, props);
        return obj;

    } else if (type == "property-list") {
        PropertyList * obj = new PropertyList(x, y, x + width, y + height, globals->get_props());
        if (presetSize)
            obj->setSize(width, height);
        setupObject(obj, props);
        setColor(obj, props);
        return obj;

    } else if (type == "input") {
        puInput * obj = new puInput(x, y, x + width, y + height);
        setupObject(obj, props);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "text") {
        puText * obj = new puText(x, y);
        setupObject(obj, props);

        // Layed-out objects need their size set, and non-layout ones
        // get a different placement.
        if (presetSize)
            obj->setSize(width, height);
        else
            obj->setLabelPlace(PUPLACE_LABEL_DEFAULT);
        setColor(obj, props, LABEL);
        return obj;

    } else if (type == "checkbox") {
        puButton * obj;
        obj = new puButton(x, y, x + width, y + height, PUBUTTON_XCHECK);
        setupObject(obj, props);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "radio") {
        puButton * obj;
        obj = new puButton(x, y, x + width, y + height, PUBUTTON_CIRCLE);
        setupObject(obj, props);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "button") {
        puButton * obj;
        const char * legend = props->getStringValue("legend", "[none]");
        if (props->getBoolValue("one-shot", true))
            obj = new puOneShot(x, y, legend);
        else
            obj = new puButton(x, y, legend);
        if (presetSize)
            obj->setSize(width, height);
        setupObject(obj, props);
        setColor(obj, props);
        return obj;

    } else if (type == "combo") {
        fgComboBox * obj = new fgComboBox(x, y, x + width, y + height, props,
                           props->getBoolValue("editable", false));
        setupObject(obj, props);
#ifdef PUCOL_EDITFIELD  // plib > 0.8.4
        setColor(obj, props, EDITFIELD);
#else
        setColor(obj, props, FOREGROUND|LABEL);
#endif
        return obj;

    } else if (type == "slider") {
        bool vertical = props->getBoolValue("vertical", false);
        puSlider * obj = new puSlider(x, y, (vertical ? height : width));
        obj->setMinValue(props->getFloatValue("min", 0.0));
        obj->setMaxValue(props->getFloatValue("max", 1.0));
        setupObject(obj, props);
        if (presetSize)
            obj->setSize(width, height);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "dial") {
        puDial * obj = new puDial(x, y, width);
        obj->setMinValue(props->getFloatValue("min", 0.0));
        obj->setMaxValue(props->getFloatValue("max", 1.0));
        obj->setWrap(props->getBoolValue("wrap", true));
        setupObject(obj, props);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "textbox") {
        int slider_width = props->getIntValue("slider", 20);
        int wrap = props->getBoolValue("wrap", true);
        puaLargeInput * obj = new puaLargeInput(x, y,
                x+width, x+height, 2, slider_width, wrap);

        if (props->hasValue("editable")) {
            if (props->getBoolValue("editable")==false)
                obj->disableInput();
            else
                obj->enableInput();
        }
        if (presetSize)
            obj->setSize(width, height);
        setupObject(obj, props);
        setColor(obj, props, FOREGROUND|LABEL);
        return obj;

    } else if (type == "select") {
        fgSelectBox * obj = new fgSelectBox(x, y, x + width, y + height, props);
        setupObject(obj, props);
#ifdef PUCOL_EDITFIELD  // plib > 0.8.4
        setColor(obj, props, EDITFIELD);
#else
        setColor(obj, props, FOREGROUND|LABEL);
#endif
        return obj;
    } else {
        return 0;
    }
}

void
FGDialog::setupObject (puObject * object, SGPropertyNode * props)
{
    string type = props->getName();
    object->setLabelPlace(PUPLACE_CENTERED_RIGHT);

    if (props->hasValue("legend"))
        object->setLegend(props->getStringValue("legend"));

    if (props->hasValue("label"))
        object->setLabel(props->getStringValue("label"));

    if (props->hasValue("border"))
        object->setBorderThickness( props->getIntValue("border", 2) );

    if ( SGPropertyNode *nft = props->getNode("font", false) ) {
       FGFontCache *fc = globals->get_fontcache();
       puFont *lfnt = fc->get(nft);
       object->setLabelFont(*lfnt);
    } else {
       object->setLabelFont(*_font);
    }

    if (props->hasValue("property")) {
        const char * name = props->getStringValue("name");
        if (name == 0)
            name = "";
        const char * propname = props->getStringValue("property");
        SGPropertyNode_ptr node = fgGetNode(propname, true);
        copy_to_pui(node, object);

        PropertyObject* po = new PropertyObject(name, object, node);
        _propertyObjects.push_back(po);
        if (props->getBoolValue("live"))
            _liveObjects.push_back(po);
    }

    SGPropertyNode * dest = fgGetNode("/sim/bindings/gui", true);
    vector<SGPropertyNode_ptr> bindings = props->getChildren("binding");
    if (type == "text" || bindings.size() > 0) {
        GUIInfo * info = new GUIInfo(this);
        info->key = props->getIntValue("keynum", -1);
        if (props->hasValue("key"))
            info->key = getKeyCode(props->getStringValue("key", ""));

        for (unsigned int i = 0; i < bindings.size(); i++) {
            unsigned int j = 0;
            SGPropertyNode *binding;
            while (dest->getChild("binding", j))
                j++;

            const char *cmd = bindings[i]->getStringValue("command");
            if (!strcmp(cmd, "nasal"))
                bindings[i]->setStringValue("module", _module.c_str());

            binding = dest->getChild("binding", j, true);
            copyProperties(bindings[i], binding);
            info->bindings.push_back(new FGBinding(binding));
        }
        object->setCallback(action_callback);

        if (type == "input" && props->getBoolValue("live"))
            object->setDownCallback(action_callback);

        if (type == "text") {
            const char *format = props->getStringValue("format", 0);
            if (format) {
                info->fmt_type = validate_format(props->getStringValue("format", 0));
                if (info->fmt_type != f_INVALID) {
                    info->text = new char[FORMAT_BUFSIZE + 1];
                    info->fmt_string = new char[strlen(format) + 1];
                    strcpy(info->fmt_string, format);
                } else {
                    SG_LOG(SG_GENERAL, SG_ALERT, "DIALOG: invalid <format> '"
                            << format << '\'');
                }
            }
        }

        object->setUserData(info);
        _info.push_back(info);
    }

    object->makeReturnDefault(props->getBoolValue("default"));
}

void
FGDialog::setupGroup (puGroup * group, SGPropertyNode * props,
                    int width, int height, bool makeFrame)
{
    setupObject(group, props);

    if (makeFrame) {
        puFrame* f = new puFrame(0, 0, width, height);
        setColor(f, props);
    }

    int nChildren = props->nChildren();
    for (int i = 0; i < nChildren; i++)
        makeObject(props->getChild(i), width, height);
    group->close();
}

void
FGDialog::setColor(puObject * object, SGPropertyNode * props, int which)
{
    string type = props->getName();
    if (type.empty())
        type = "dialog";
    if (type == "textbox" && props->getBoolValue("editable"))
        type += "-editable";

    FGColor *c = new FGColor(_gui->getColor("background"));
    c->merge(_gui->getColor(type));
    c->merge(props->getNode("color"));
    if (c->isValid())
        object->setColourScheme(c->red(), c->green(), c->blue(), c->alpha());

    const struct {
        int mask;
        int id;
        const char *name;
        const char *cname;
    } pucol[] = {
        { BACKGROUND, PUCOL_BACKGROUND, "background", "color-background" },
        { FOREGROUND, PUCOL_FOREGROUND, "foreground", "color-foreground" },
        { HIGHLIGHT,  PUCOL_HIGHLIGHT,  "highlight",  "color-highlight" },
        { LABEL,      PUCOL_LABEL,      "label",      "color-label" },
        { LEGEND,     PUCOL_LEGEND,     "legend",     "color-legend" },
        { MISC,       PUCOL_MISC,       "misc",       "color-misc" },
#ifdef PUCOL_EDITFIELD  // plib > 0.8.4
        { EDITFIELD,  PUCOL_EDITFIELD,  "editfield",  "color-editfield" },
#endif
    };

    const int numcol = sizeof(pucol) / sizeof(pucol[0]);

    for (int i = 0; i < numcol; i++) {
        bool dirty = false;
        c->clear();
        c->setAlpha(1.0);

        dirty |= c->merge(_gui->getColor(type + '-' + pucol[i].name));
        if (which & pucol[i].mask)
            dirty |= c->merge(props->getNode("color"));

        if ((pucol[i].mask == LABEL) && !c->isValid())
            dirty |= c->merge(_gui->getColor("label"));

        dirty |= c->merge(props->getNode(pucol[i].cname));

        if (c->isValid() && dirty)
            object->setColor(pucol[i].id, c->red(), c->green(), c->blue(), c->alpha());
    }
}


static struct {
    const char *name;
    int key;
} keymap[] = {
    {"backspace", 8},
    {"tab", 9},
    {"return", 13},
    {"enter", 13},
    {"esc", 27},
    {"escape", 27},
    {"space", ' '},
    {"&amp;", '&'},
    {"and", '&'},
    {"&lt;", '<'},
    {"&gt;", '>'},
    {"f1", PU_KEY_F1},
    {"f2", PU_KEY_F2},
    {"f3", PU_KEY_F3},
    {"f4", PU_KEY_F4},
    {"f5", PU_KEY_F5},
    {"f6", PU_KEY_F6},
    {"f7", PU_KEY_F7},
    {"f8", PU_KEY_F8},
    {"f9", PU_KEY_F9},
    {"f10", PU_KEY_F10},
    {"f11", PU_KEY_F11},
    {"f12", PU_KEY_F12},
    {"left", PU_KEY_LEFT},
    {"up", PU_KEY_UP},
    {"right", PU_KEY_RIGHT},
    {"down", PU_KEY_DOWN},
    {"pageup", PU_KEY_PAGE_UP},
    {"pagedn", PU_KEY_PAGE_DOWN},
    {"home", PU_KEY_HOME},
    {"end", PU_KEY_END},
    {"insert", PU_KEY_INSERT},
    {0, -1},
};

int
FGDialog::getKeyCode(const char *str)
{
    enum {
        CTRL = 0x1,
        SHIFT = 0x2,
        ALT = 0x4,
    };

    while (*str == ' ')
        str++;

    char *buf = new char[strlen(str) + 1];
    strcpy(buf, str);
    char *s = buf + strlen(buf);
    while (s > str && s[-1] == ' ')
        s--;
    *s = 0;
    s = buf;

    int mod = 0;
    while (1) {
        if (!strncmp(s, "Ctrl-", 5) || !strncmp(s, "CTRL-", 5))
            s += 5, mod |= CTRL;
        else if (!strncmp(s, "Shift-", 6) || !strncmp(s, "SHIFT-", 6))
            s += 6, mod |= SHIFT;
        else if (!strncmp(s, "Alt-", 4) || !strncmp(s, "ALT-", 4))
            s += 4, mod |= ALT;
        else
            break;
    }

    int key = -1;
    if (strlen(s) == 1 && isascii(*s)) {
        key = *s;
        if (mod & SHIFT)
            key = toupper(key);
        if (mod & CTRL)
            key = toupper(key) - 64;
        if (mod & ALT)
            ;   // Alt not propagated to the gui
    } else {
        for (char *t = s; *t; t++)
            *t = tolower(*t);
        for (int i = 0; keymap[i].name; i++) {
            if (!strcmp(s, keymap[i].name)) {
                key = keymap[i].key;
                break;
            }
        }
    }
    delete[] buf;
    return key;
}



////////////////////////////////////////////////////////////////////////
// Implementation of FGDialog::PropertyObject.
////////////////////////////////////////////////////////////////////////

FGDialog::PropertyObject::PropertyObject (const char * n,
                                           puObject * o,
                                           SGPropertyNode_ptr p)
    : name(n),
      object(o),
      node(p)
{
}




////////////////////////////////////////////////////////////////////////
// Implementation of fgValueList and derived pui widgets
////////////////////////////////////////////////////////////////////////


fgValueList::fgValueList(SGPropertyNode *p) :
    _props(p)
{
    make_list();
}

void
fgValueList::update()
{
    destroy_list();
    make_list();
}

fgValueList::~fgValueList()
{
    destroy_list();
}

void
fgValueList::make_list()
{
    vector<SGPropertyNode_ptr> value_nodes = _props->getChildren("value");
    _list = new char *[value_nodes.size() + 1];
    unsigned int i;
    for (i = 0; i < value_nodes.size(); i++)
        _list[i] = strdup((char *)value_nodes[i]->getStringValue());
    _list[i] = 0;
}

void
fgValueList::destroy_list()
{
    for (int i = 0; _list[i] != 0; i++)
        if (_list[i])
            free(_list[i]);
    delete[] _list;
}



void
fgList::update()
{
    fgValueList::update();
    newList(_list);
}

// end of dialog.cxx
