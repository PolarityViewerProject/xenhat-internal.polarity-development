/** 
 * @file llpathfindingcharacter.h
 * @author William Todd Stinson
 * @brief Definition of a pathfinding character that contains various properties required for havok pathfinding.
 *
 * $LicenseInfo:firstyear=2002&license=viewerlgpl$
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

#ifndef LL_LLPATHFINDINGCHARACTER_H
#define LL_LLPATHFINDINGCHARACTER_H

#include "llpathfindingobject.h"

class LLSD;

class LLPathfindingCharacter : public LLPathfindingObject
{
public:
	LLPathfindingCharacter(const std::string &pUUID, const LLSD &pCharacterData);
	LLPathfindingCharacter(const LLPathfindingCharacter& pOther);
	virtual ~LLPathfindingCharacter();

	LLPathfindingCharacter& operator =(const LLPathfindingCharacter& pOther);

	inline F32  getCPUTime() const   {return mCPUTime;};

#ifndef SERVER_SIDE_CHARACTER_SHAPE_ROLLOUT_COMPLETE
	inline bool hasShapeData() const {return mHasShapeData;};
#endif // SERVER_SIDE_CHARACTER_SHAPE_ROLLOUT_COMPLETE
	inline BOOL isHorizontal() const {return mIsHorizontal;};
	inline F32  getLength() const    {return mLength;};
	inline F32  getRadius() const    {return mRadius;};

protected:

public:

private:
	void parseCharacterData(const LLSD &pCharacterData);

	F32  mCPUTime;

#ifndef SERVER_SIDE_CHARACTER_SHAPE_ROLLOUT_COMPLETE
	bool mHasShapeData;
#endif // SERVER_SIDE_CHARACTER_SHAPE_ROLLOUT_COMPLETE
	BOOL mIsHorizontal;
	F32  mLength;
	F32  mRadius;
};

#endif // LL_LLPATHFINDINGCHARACTER_H
