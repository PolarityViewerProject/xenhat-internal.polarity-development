// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/** 
 * @file llversioninfo.cpp
 * @brief Routines to access the viewer version and build information
 * @author Martin Reddy
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
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

#include "llviewerprecompiledheaders.h"
#include <iostream>
#include <sstream>
#include "llversioninfo.h"
#include <boost/regex.hpp>

#if ! defined(LL_VIEWER_CHANNEL)       \
 || ! defined(LL_VIEWER_VERSION_MAJOR) \
 || ! defined(LL_VIEWER_VERSION_MINOR) \
 || ! defined(LL_VIEWER_VERSION_PATCH) \
 || ! defined(LL_VIEWER_VERSION_BUILD) \
 || ! defined(LINDEN_SOURCE_MAJOR) \
 || ! defined(LINDEN_SOURCE_MINOR) \
 || ! defined(LINDEN_SOURCE_PATCH) \
 || ! defined(BUILD_COMMIT_HASH) \
 || ! defined(BUILD_COMMIT_HASH_LONG) \
 || ! defined(BUILD_NUMBER)
 #error "Channel or Version information is undefined"
#endif

//
// Set the version numbers in indra/VIEWER_VERSION
//

//static
S32 LLVersionInfo::getMajor()
{
	return LL_VIEWER_VERSION_MAJOR;
}

//static
S32 LLVersionInfo::getMinor()
{
	return LL_VIEWER_VERSION_MINOR;
}

//static
S32 LLVersionInfo::getPatch()
{
	return LL_VIEWER_VERSION_PATCH;
}

//static
S32 LLVersionInfo::getBuild()
{
	return LL_VIEWER_VERSION_BUILD;
}

//static
const std::string &LLVersionInfo::getVersion()
{
	static std::string version("");
	if (version.empty())
	{
		std::ostringstream stream;
		stream << LLVersionInfo::getShortVersion() << "." << LLVersionInfo::getBuild();
		// cache the version string
		version = stream.str();
	}
	return version;
}

//static
const std::string &LLVersionInfo::getShortVersion()
{
	static std::string short_version("");
	if(short_version.empty())
	{
		// cache the version string
		std::ostringstream stream;
		stream << LL_VIEWER_VERSION_MAJOR << "."
		       << LL_VIEWER_VERSION_MINOR << "."
		       << LL_VIEWER_VERSION_PATCH;
		short_version = stream.str();
	}
	return short_version;
}

namespace
{
	/// Storage of the channel name the viewer is using.
	//  The channel name is set by hardcoded constant, 
	//  or by calling LLVersionInfo::resetChannel()
	std::string sWorkingChannelName(LL_VIEWER_CHANNEL);

	// Storage for the "version and channel" string.
	// This will get reset too.
	std::string sVersionChannel("");
    std::string sVersionChannelForPVData("");
	std::string sCompiledChannel("");
	// </polarity> PVData
}
// <polarity> PVData
const std::string &LLVersionInfo::getChannelAndVersionStatic()
{
	if (sVersionChannelForPVData.empty())
	{
		// cache the version string
		std::ostringstream stream;
		stream	<< LL_VIEWER_CHANNEL << " "
				<< LL_VIEWER_VERSION_MAJOR << "."
				<< LL_VIEWER_VERSION_MINOR << "."
		 		<< LL_VIEWER_VERSION_PATCH << " ("
		 		<< LL_VIEWER_VERSION_BUILD << ")";
		sVersionChannelForPVData = stream.str();
		LL_INFOS("PVDataOldAPI") << " Full viewer version = \"" << sVersionChannelForPVData << "\"" << LL_ENDL;
	}
	return sVersionChannelForPVData;
}
const std::string &LLVersionInfo::getCompiledChannel()
{
	sCompiledChannel = LL_VIEWER_CHANNEL;
	return sCompiledChannel;
}
// </polarity> PVData

//static
const std::string &LLVersionInfo::getChannelAndVersion()
{
	if (sVersionChannel.empty())
	{
		// cache the version string
		sVersionChannel = LLVersionInfo::getChannel() + " " + LLVersionInfo::getVersion();
	}

	return sVersionChannel;
}
const std::string & LLVersionInfo::getLastLindenRelease()
{
	static std::string sLindenRelease("");
	if(sLindenRelease.empty())
	{
		// cache the version string
		std::ostringstream llrelease;
		llrelease << LINDEN_SOURCE_MAJOR << "."
				<< LINDEN_SOURCE_MINOR << "."
				<< LINDEN_SOURCE_PATCH;
		sLindenRelease = llrelease.str();
	}
	return sLindenRelease;
}

//static
const std::string &LLVersionInfo::getChannel()
{
	return sWorkingChannelName;
}

void LLVersionInfo::resetChannel(const std::string& channel)
{
	sWorkingChannelName = channel;
	sVersionChannel.clear(); // Reset version and channel string til next use.
}

//static
LLVersionInfo::ViewerMaturity LLVersionInfo::getViewerMaturity()
{
    ViewerMaturity maturity;
    
    std::string channel = getChannel();

	static const boost::regex is_test_channel("\\bTest\\b");
	static const boost::regex is_beta_channel("\\bBeta\\b");
	static const boost::regex is_project_channel("\\bProject\\b");
	static const boost::regex is_release_channel("\\bRelease\\b");

    if (boost::regex_search(channel, is_release_channel))
    {
        maturity = RELEASE_VIEWER;
    }
    else if (boost::regex_search(channel, is_beta_channel))
    {
        maturity = BETA_VIEWER;
    }
    else if (boost::regex_search(channel, is_project_channel))
    {
        maturity = PROJECT_VIEWER;
    }
    else if (boost::regex_search(channel, is_test_channel))
    {
        maturity = TEST_VIEWER;
    }
    else
    {
        LL_WARNS() << "Channel '" << channel
                   << "' does not follow naming convention, assuming Test"
                   << LL_ENDL;
        maturity = TEST_VIEWER;
    }
    return maturity;
}

    
const std::string &LLVersionInfo::getBuildConfig()
{
    static const std::string build_configuration(LLBUILD_CONFIG); // set in indra/cmake/BuildVersion.cmake
    return build_configuration;
}

const std::string &LLVersionInfo::getBuildCommitHash()
{
	static const std::string hash_short(BUILD_COMMIT_HASH);
	return hash_short;
}

const std::string &LLVersionInfo::getBuildCommitHashLong()
{
	static const std::string hash_long(BUILD_COMMIT_HASH_LONG);
	return hash_long;
}

const std::string &LLVersionInfo::getBuildNumber()
{
	static const std::string build_id(std::to_string(BUILD_NUMBER)); // oh surprise, we get numbers!
	return build_id;
}
