#ifndef GENGRAPH_H
#define GENGRAPH_H

class cGenericGraphics
{
public:
    // an interface to a simplistic graphics system, mimicking cGrDebug
    // not everything has to be implemented; empty implementations provided
    /*----------------------------------------------------------------------+
    |                                                                       |
    |   DeleteSavedGraphics                                                 |
    |                                                                       |
    |Abstract:                                                              |
    |   If you have created any elements with the throw_away flag = kFalse, |
    |   then these graphics have been saved for later redrawing, zooming,   |
    |   etc.  You must delete them yourself by calling these functions.     |
    +----------------------------------------------------------------------*/
    virtual 
    void DeleteSavedGraphics (int segment)  // kill only on this seg
    {}
    virtual 
    void DeleteSavedGraphics ()                        // kill all dbg graphics
    {}


    virtual 
    void DrawLine (int x1, int y1, int x2, int y2, int segment,
                   int width = 0, bool filled = false, bool throw_away = true)
    {}
    
    virtual 
    void DrawArc  (int x1, int y1, int x2, int y2, 
                   int radius, // > 0 clockwise
                   int cx, int cy,
                   int segment,
                   int width = 0, bool filled = false, bool throw_away = true)
    {}

    virtual
    void DrawCircle (int x, int y, int diameter, int segment,
                     bool filled = false, bool throw_away = true)
    {}

    virtual
    void DrawRect   (int xmin, int ymin, int xmax, int ymax,
                     int segment,
                     bool filled = false, bool throw_away = true)
    {}

    virtual
    void DrawSimpleText (int x, int y, const char *string,
                         int segment, bool throw_away = true)
    {}


    /*----------------------------------------------------------------------+
    |                                                                       |
    |   Redraw -- redraw the entire contents of the window.  This does      |
    |             not redraw any unsaved graphics that were previously      |
    |             drawn.  This routine is provided for your convenience     |
    |             while inside a debugger.  This routine will discard any   |
    |             pending keyin and mouse events.                           |
    |                                                                       |
    +----------------------------------------------------------------------*/
    virtual
    void Redraw()
    {}
 
 
    // Draws & Erases do not necessarilly show up on the video screen
    // immediately due to buffering in the native graphics system.  This
    // call will force all undrawn primitives to be flushed to the screen
    // immediately.  This call should be used with great care, as over-use
    // will slow down the graphics display greatly.  I.e. you should not
    // call FlushToScreen after every draw call.
 
    virtual
    void FlushToScreen()
    {}

	// scale all saved and future drawings. x,y retains its position on screen
	virtual void Zoom(int x, int y, double koff) 
	{}

	// make x,y appear in the center of the screen; scaling unchanged
	virtual void Center(int x, int y)
	{}

    virtual void StartPolygon()
    {}
    virtual void AddPolygonPoint(int x, int y)
    {}
    virtual void AddPolygonArc(int x, int y, int cx, int cy, int r)
    {}
    virtual void DrawPolygon(int segment, bool throw_away = true)
    {}
	// changing the defaults for drawing calls like DrawLine (return previous value)
    virtual bool SetThrowAway(bool throw_away) {
		return true; }
	virtual int SetWidth(int width){
		return 0; }
	virtual bool SetFill(bool fill) {
		return false;
	}
};

enum ggColors
{
	kInvalidColor = -1,
	kBlack,
	kBlue,
	kLtGray,
	kOrange,
	kRed,
	kMagenta,
	kMaroon,
	kForest,
	kSkyBlue,
	kGold,
	kBrown,
	kPlum,
	kGreen,
	kWhite,
	kYellow,
	kPink
};


extern "C"
__declspec(dllexport) void SetGenericGraphics(cGenericGraphics *);

#endif
