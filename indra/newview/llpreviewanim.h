/** 
 * @file llpreviewanim.h
 * @brief LLPreviewAnim class definition
 *
 * $LicenseInfo:firstyear=2004&license=viewergpl$
 * 
 * Copyright (c) 2004-2009, Linden Research, Inc.
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

#ifndef LL_LLPREVIEWANIM_H
#define LL_LLPREVIEWANIM_H

#include "llpreview.h"
#include "llcharacter.h"

class LLPreviewAnim : public LLPreview
{
public:
	enum e_activation_type { NONE = 0, PLAY = 1, AUDITION = 2 };
	LLPreviewAnim(const LLSD& key);

	static void playAnim( void* userdata );
	static void auditionAnim( void* userdata );
	static void endAnimCallback( void *userdata );
	/*virtual*/	BOOL postBuild();
	void activate(e_activation_type type);
	
protected:
	virtual void onClose(bool app_quitting);
	
	LLAnimPauseRequest	mPauseRequest;
	LLUUID		mItemID;
	std::string	mTitle;
	LLUUID		mObjectID;
	LLButton*	mPlayBtn;
	LLButton*	mAuditionBtn;
};

#endif  // LL_LLPREVIEWSOUND_H
