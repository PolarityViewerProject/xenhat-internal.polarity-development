/** 
 * @file llwindowwin32.h
 * @brief Windows implementation of LLWindow class
 *
 * $LicenseInfo:firstyear=2001&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#ifndef LL_LLWINDOWWIN32_H
#define LL_LLWINDOWWIN32_H

// Limit Windows API to small and manageable set.
#include "llwin32headerslean.h"

#include "llwindow.h"
#include "llwindowcallbacks.h"
#include "lldragdropwin32.h"

// Hack for async host by name
#define LL_WM_HOST_RESOLVED      (WM_APP + 1)
typedef void (*LLW32MsgCallback)(const MSG &msg);

class LLWindowWin32 : public LLWindow
{
public:
	/*virtual*/ void show();
	/*virtual*/ void hide();
	/*virtual*/ void close();
	/*virtual*/ BOOL getVisible();
	/*virtual*/ BOOL getMinimized();
	/*virtual*/ BOOL getMaximized();
	/*virtual*/ BOOL maximize();
	/*virtual*/ void minimize();
	/*virtual*/ void restore();
	/*virtual*/ BOOL getFullscreen();
// [SL:KB] - Patch: Viewer-FullscreenWindow | Checked: 2010-04-13 (Catznip-2.1.2a) | Added: Catznip-2.0.0a
	/*virtual*/ BOOL getFullscreenWindow();
	/*virtual*/ void setFullscreenWindow(BOOL fFullscreen);
// [/SL:KB]
	/*virtual*/ BOOL getPosition(LLCoordScreen *position);
// [SL:KB] - Patch: Viewer-FullscreenWindow | Checked: 2010-08-26 (Catznip-2.1.2a) | Added: Catznip-2.1.2a
	/*virtual*/ BOOL getRestoredPosition(LLCoordScreen *position);
// [/SL:KB]
	/*virtual*/ BOOL getSize(LLCoordScreen *size);
	/*virtual*/ BOOL getSize(LLCoordWindow *size);
// [SL:KB] - Patch: Viewer-FullscreenWindow | Checked: 2010-08-26 (Catznip-2.1.2a) | Added: Catznip-2.1.2a
	/*virtual*/ BOOL getRestoredSize(LLCoordScreen *size);
// [/SL:KB]
	/*virtual*/ BOOL setPosition(LLCoordScreen position);
	/*virtual*/ BOOL setSizeImpl(LLCoordScreen size);
	/*virtual*/ BOOL setSizeImpl(LLCoordWindow size);
	/*virtual*/ BOOL switchContext(BOOL fullscreen, const LLCoordScreen &size, EVSyncSetting vsync_setting, const LLCoordScreen * const posp = NULL);
	/*virtual*/ BOOL setCursorPosition(LLCoordWindow position);
	/*virtual*/ BOOL getCursorPosition(LLCoordWindow *position);
	/*virtual*/ void showCursor();
	/*virtual*/ void hideCursor();
	/*virtual*/ void showCursorFromMouseMove();
	/*virtual*/ void hideCursorUntilMouseMove();
	/*virtual*/ BOOL isCursorHidden();
	/*virtual*/ void updateCursor();
	/*virtual*/ ECursorType getCursor() const;
	/*virtual*/ void captureMouse();
	/*virtual*/ void releaseMouse();
	/*virtual*/ void setMouseClipping( BOOL b );
	/*virtual*/ BOOL isClipboardTextAvailable();
	/*virtual*/ BOOL pasteTextFromClipboard(LLWString &dst);
	/*virtual*/ BOOL copyTextToClipboard(const LLWString &src);
	/*virtual*/ void flashIcon(F32 seconds);
	/*virtual*/ F32 getGamma();
	/*virtual*/ BOOL setGamma(const F32 gamma); // Set the gamma
	/*virtual*/ void setFSAASamples(const U32 fsaa_samples);
	/*virtual*/ U32 getFSAASamples();
	/*virtual*/ BOOL restoreGamma();			// Restore original gamma table (before updating gamma)
	/*virtual*/ ESwapMethod getSwapMethod() { return mSwapMethod; }
	/*virtual*/ void gatherInput();
	/*virtual*/ void delayInputProcessing();
	/*virtual*/ void swapBuffers();
	/*virtual*/ void restoreGLContext() {};

	// handy coordinate space conversion routines
	/*virtual*/ BOOL convertCoords(LLCoordScreen from, LLCoordWindow *to);
	/*virtual*/ BOOL convertCoords(LLCoordWindow from, LLCoordScreen *to);
	/*virtual*/ BOOL convertCoords(LLCoordWindow from, LLCoordGL *to);
	/*virtual*/ BOOL convertCoords(LLCoordGL from, LLCoordWindow *to);
	/*virtual*/ BOOL convertCoords(LLCoordScreen from, LLCoordGL *to);
	/*virtual*/ BOOL convertCoords(LLCoordGL from, LLCoordScreen *to);

	/*virtual*/ LLWindowResolution* getSupportedResolutions(S32 &num_resolutions);
	/*virtual*/ F32	getNativeAspectRatio();
	/*virtual*/ F32 getPixelAspectRatio();
	/*virtual*/ void setNativeAspectRatio(F32 ratio) { mOverrideAspectRatio = ratio; }

	/*virtual*/	BOOL dialogColorPicker(F32 *r, F32 *g, F32 *b );

	/*virtual*/ void *getPlatformWindow();
	/*virtual*/ void bringToFront();
	/*virtual*/ void focusClient();

	/*virtual*/ void allowLanguageTextInput(LLPreeditor *preeditor, BOOL b);
	/*virtual*/ void setLanguageTextInput( const LLCoordGL & pos );
	/*virtual*/ void updateLanguageTextInputArea();
	/*virtual*/ void interruptLanguageTextInput();
	/*virtual*/ void spawnWebBrowser(const std::string& escaped_url, bool async);

	/*virtual*/ void setTitle(const std::string& win_title); // <polarity/> Dynamic window title
	static DEVMODE mDisplayInfo;
	static U32 mRefreshRate;
	static U32 probeRefreshRate();
	static U32 getRefreshRate()
	{
		if (mRefreshRate != 0)
		{
			return mRefreshRate;
		}
		else
		{
			return probeRefreshRate();
		}
	};

	/*virtual*/ F32 getSystemUISize();

	LLWindowCallbacks::DragNDropResult completeDragNDropRequest( const LLCoordGL gl_coord, const MASK mask, LLWindowCallbacks::DragNDropAction action, const std::string url );

	static std::vector<std::string> getDynamicFallbackFontList();

	static void setDPIAwareness();

protected:
	LLWindowWin32(LLWindowCallbacks* callbacks,
		const std::string& title, const std::string& name, int x, int y, int width, int height, U32 flags, 
		BOOL fullscreen, BOOL clearBg, EVSyncSetting vsync_setting, BOOL use_gl,
		BOOL ignore_pixel_depth, U32 fsaa_samples);
	~LLWindowWin32();

	void	initCursors();
	void	initInputDevices();
	HCURSOR loadColorCursor(LPCTSTR name);
	BOOL	isValid();
	void	moveWindow(const LLCoordScreen& position,const LLCoordScreen& size);
	virtual LLSD	getNativeKeyData();

	// Changes display resolution. Returns true if successful
	BOOL	setDisplayResolution(S32 width, S32 height, S32 bits, S32 refresh);

	// Go back to last fullscreen display resolution.
	BOOL	setFullscreenResolution();

	// Restore the display resolution to its value before we ran the app.
	BOOL	resetDisplayResolution();

	BOOL	shouldPostQuit() { return mPostQuit; }

	void	fillCompositionForm(const LLRect& bounds, COMPOSITIONFORM *form);
	void	fillCandidateForm(const LLCoordGL& caret, const LLRect& bounds, CANDIDATEFORM *form);
	void	fillCharPosition(const LLCoordGL& caret, const LLRect& bounds, const LLRect& control, IMECHARPOSITION *char_position);
	void	fillCompositionLogfont(LOGFONT *logfont);
	U32		fillReconvertString(const LLWString &text, S32 focus, S32 focus_length, RECONVERTSTRING *reconvert_string);
	void	handleStartCompositionMessage();
	void	handleCompositionMessage(U32 indexes);
	BOOL	handleImeRequests(WPARAM w_param, LPARAM l_param, LRESULT *result);

protected:
	//
	// Platform specific methods
	//

	BOOL	getClientRectInScreenSpace(RECT* rectp);
	void 	updateJoystick( );

	static LRESULT CALLBACK mainWindowProc(HWND h_wnd, UINT u_msg, WPARAM w_param, LPARAM l_param);
	static BOOL CALLBACK enumChildWindows(HWND h_wnd, LPARAM l_param);


	//
	// Platform specific variables
	//
	WCHAR		*mWindowTitle;
	WCHAR		*mWindowClassName;

	HWND		mWindowHandle;	// window handle
	HGLRC		mhRC;			// OpenGL rendering context
	HDC			mhDC;			// Windows Device context handle
	HINSTANCE	mhInstance;		// handle to application instance
	WNDPROC		mWndProc;		// user-installable window proc
	RECT		mOldMouseClip;  // Screen rect to which the mouse cursor was globally constrained before we changed it in clipMouse()
	WPARAM		mLastSizeWParam;
	F32			mOverrideAspectRatio;
	F32			mNativeAspectRatio;

	HCURSOR		mCursor[ UI_CURSOR_COUNT ];  // Array of all mouse cursors

	static BOOL sIsClassRegistered; // has the window class been registered?

	F32			mCurrentGamma;
	U32			mFSAASamples;
	WORD		mPrevGammaRamp[256*3];
	WORD		mCurrentGammaRamp[256*3];

	LPWSTR		mIconResource;
	BOOL		mMousePositionModified;
	BOOL		mInputProcessingPaused;

	// The following variables are for Language Text Input control.
	// They are all static, since one context is shared by all LLWindowWin32
	// instances.
	static BOOL		sLanguageTextInputAllowed;
	static BOOL		sWinIMEOpened;
	static HKL		sWinInputLocale;
	static DWORD	sWinIMEConversionMode;
	static DWORD	sWinIMESentenceMode;
	static LLCoordWindow sWinIMEWindowPosition;
	LLCoordGL		mLanguageTextInputPointGL;
	LLRect			mLanguageTextInputAreaGL;

	LLPreeditor		*mPreeditor;

	LLDragDropWin32* mDragDrop;

	U32				mKeyCharCode;
	U32				mKeyScanCode;
	U32				mKeyVirtualKey;
	U32				mRawMsg;
	U32				mRawWParam;
	U32				mRawLParam;

// [SL:KB] - Patch: Viewer-FullscreenWindow | Checked: 2010-08-26 (Catznip-2.1.2a) | Added: Catznip-2.1.2a
	WINDOWPLACEMENT mRestoredPlacement;				// Only valid when the window is "fullscreen" sized (see setFullscreenWindow)
// [/SL:KB];

// <FS:ND> Allow to query for window chrome sizes.
public:
	virtual void getWindowChrome( U32 &aChromeW, U32 &aChromeH );
// </FS:ND>

	friend class LLWindowManager;
};

extern LLW32MsgCallback gAsyncMsgCallback;
extern LPWSTR gIconResource;

static void	handleMessage( const MSG& msg );

S32 OSMessageBoxWin32(const std::string& text, const std::string& caption, U32 type);

#endif //LL_LLWINDOWWIN32_H
