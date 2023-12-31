// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * @file sanitycheck.cpp
 * @brief Settings sanity check engine
 *
 * Copyright (C) 2011, Zi Ree @ Second Life
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
 */

#include "llviewerprecompiledheaders.h"

#include "llcontrol.h"
#include "llnotificationsutil.h"
#include "lltrans.h"
#include "llviewercontrol.h"
#include "sanitycheck.h"



void SanityCheck::init()
{
	struct f : public LLControlGroup::ApplyFunctor
	{
		SanityCheck* chk;

		f(SanityCheck* s) : chk(s) {};

		virtual void apply(const std::string& name, LLControlVariable* control)
		{
			if (control->getSanityType() != SANITY_TYPE_NONE)
			{
				control->getSanitySignal()->connect(boost::bind(&SanityCheck::onSanity, _1));
				SanityCheck::instance().onSanity(control);
			}
		}
	} func(this);

	gSavedSettings.applyToAll(&func);
	gSavedPerAccountSettings.applyToAll(&func);
}

// static
void SanityCheck::onSanity(LLControlVariable* controlp)
{
	if (controlp->isSane())
		return;

	static LLControlVariable* lastControl = nullptr;

	if (controlp == lastControl)
		return;

	lastControl = controlp;

	std::string checkType = "SanityCheck" + gSavedSettings.sanityTypeEnumToString(controlp->getSanityType());
	std::vector<LLSD> sanityValues = controlp->getSanityValues();

	LLSD args;
	LLStringUtil::format_map_t map;
	// <polarity> print true/false for boolean values instead of 1/0
	if (controlp->isType(eControlType::TYPE_BOOLEAN))
	{
		if (sanityValues[0].asBoolean())
		{
			map["VALUE_1"] = "true";
		}
		else
		{
			map["VALUE_1"] = "false";
		}
	}
	else
	{
		map["VALUE_1"] = sanityValues[0].asString();
	}
	map["VALUE_2"] = sanityValues[1].asString();
	map["CONTROL_NAME"] = controlp->getName();
	// FIXME: Special symbols such as '&quot;' seem to be stripped from the XML
	args["SANITY_MESSAGE"] = LLTrans::getString(checkType, map);
	args["SANITY_COMMENT"] = controlp->getSanityComment();
	args["CURRENT_VALUE"] = controlp->getValue().asString();
	LLNotificationsUtil::add("SanityCheck", args);
}

