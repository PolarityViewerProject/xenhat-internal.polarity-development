/** 
 * @file lloutputmonitorctrl.h
 * @brief LLOutputMonitorCtrl base class
 *
 * $LicenseInfo:firstyear=2001&license=viewergpl$
 * 
 * Copyright (c) 2001-2009, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlifegrid.net/programs/open_source/licensing/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlifegrid.net/programs/open_source/licensing/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 */

#ifndef LL_LLOUTPUTMONITORCTRL_H
#define LL_LLOUTPUTMONITORCTRL_H

#include "v4color.h"
#include "llview.h"

class LLTextBox;
class LLUICtrlFactory;

//
// Classes
//

class LLOutputMonitorCtrl
: public LLView
{
public:
	struct Params : public LLInitParam::Block<Params, LLView::Params>
	{
		Optional<bool>	draw_border;

		Params()
		{
			draw_border = true;
			name = "output_monitor";
			follows.flags(FOLLOWS_LEFT|FOLLOWS_TOP);
			mouse_opaque = false;
		};
	};
protected:
	bool	mBorder;
	LLOutputMonitorCtrl(const Params&);
	friend class LLUICtrlFactory;

public:
	virtual ~LLOutputMonitorCtrl();

	// llview overrides
	virtual void	draw();

	void			setPower(F32 val);
	F32				getPower(F32 val) const { return mPower; }
	
	bool			getIsMuted() const { return mIsMuted; }
	void			setIsMuted(bool val) { mIsMuted = val; }

private:
	static LLColor4	sColorMuted;
	static LLColor4	sColorNormal;
	static LLColor4	sColorOverdriven;
	static LLColor4	sColorBound;
	static S32		sRectsNumber;
	static F32		sRectWidthRatio;
	static F32		sRectHeightRatio;
	
	F32				mPower;
	bool			mIsMuted;
};

#endif
