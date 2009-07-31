/**
 * @file   lllogininstance_test.cpp
 * @brief  Test for lllogininstance.cpp.
 * 
 * $LicenseInfo:firstyear=2008&license=internal$
 * Copyright (c) 2008, Linden Research, Inc.
 * $/LicenseInfo$
 */

// Precompiled header
#include "../llviewerprecompiledheaders.h"
// Own header
#include "../lllogininstance.h"
// STL headers
// std headers
// external library headers
// other Linden headers
#include "../test/lltut.h"
#include "llevents.h"

#if defined(LL_WINDOWS)
#pragma warning(disable: 4355)      // using 'this' in base-class ctor initializer expr
#endif

// Constants
const std::string VIEWERLOGIN_URI("viewerlogin_uri");
const std::string VIEWERLOGIN_GRIDLABEL("viewerlogin_grid");

const std::string APPVIEWER_SERIALNUMBER("appviewer_serialno");

// Link seams.

//-----------------------------------------------------------------------------
static LLEventStream gTestPump("test_pump");

#include "lllogin.h"
static std::string gLoginURI;
static LLSD gLoginCreds;
static bool gDisconnectCalled = false;
class LLLogin::Impl
{
};
LLLogin::LLLogin() {}
LLLogin::~LLLogin() {}
LLEventPump& LLLogin::getEventPump() { return gTestPump; }
void LLLogin::connect(const std::string& uri, const LLSD& credentials) 
{
	gLoginURI = uri;
	gLoginCreds = credentials;
}

void LLLogin::disconnect() 
{
	gDisconnectCalled = true;
}

//-----------------------------------------------------------------------------
#include "../llviewernetwork.h"
unsigned char gMACAddress[MAC_ADDRESS_BYTES] = {'1','2','3','4','5','6'};		/* Flawfinder: ignore */

LLViewerLogin::LLViewerLogin() {}
LLViewerLogin::~LLViewerLogin() {}
void LLViewerLogin::getLoginURIs(std::vector<std::string>& uris) const 
{
	uris.push_back(VIEWERLOGIN_URI);
}
std::string LLViewerLogin::getGridLabel() const { return VIEWERLOGIN_GRIDLABEL; }

//-----------------------------------------------------------------------------
#include "../llviewercontrol.h"
LLControlGroup gSavedSettings("Global");
LLControlGroup gSavedSkinSettings("Skinning");
std::string gCurrentVersion = "invalid_version";

LLControlGroup::LLControlGroup(const std::string& name) :
	LLInstanceTracker<LLControlGroup, std::string>(name){}
LLControlGroup::~LLControlGroup() {}
void LLControlGroup::setBOOL(const std::string& name, BOOL val) {}
BOOL LLControlGroup::getBOOL(const std::string& name) { return FALSE; }
U32 LLControlGroup::saveToFile(const std::string& filename, BOOL nondefault_only) { return 1; }
void LLControlGroup::setString(const std::string& name, const std::string& val) {}
std::string LLControlGroup::getString(const std::string& name) { return "test_string"; }
BOOL LLControlGroup::declareBOOL(const std::string& name, BOOL initial_val, const std::string& comment, BOOL persist) { return TRUE; }
BOOL LLControlGroup::declareString(const std::string& name, const std::string &initial_val, const std::string& comment, BOOL persist) { return TRUE; }

//-----------------------------------------------------------------------------
#include "../llurlsimstring.h"
LLURLSimString LLURLSimString::sInstance;
bool LLURLSimString::parse() { return true; }

//-----------------------------------------------------------------------------
#include "../llfloatertos.h"
static LLFloaterTOS::ETOSType gTOSType;
static LLFloaterTOS::YesNoCallback gTOSCallback;
LLFloaterTOS* LLFloaterTOS::show(LLFloaterTOS::ETOSType type, 
							  const std::string & message, 
							  const YesNoCallback& callback)
{
	gTOSType = type;
	gTOSCallback = callback;
	return NULL;
}

//-----------------------------------------------------------------------------
// LLNotifications
class MockNotifications : public LLNotificationsInterface
{
	boost::function<void (const LLSD&, const LLSD&)> mResponder;
	int mAddedCount;

public: 
	MockNotifications() :
		mResponder(0),
		mAddedCount(0)
	{
	}

	virtual ~MockNotifications() {}

	/* virtual */ LLNotificationPtr add(
					const std::string& name,
					const LLSD& substitutions,
					const LLSD& payload, 
					LLNotificationFunctorRegistry::ResponseFunctor functor)
	{
		mResponder = functor;
		mAddedCount++;
		return LLNotificationPtr((LLNotification*)NULL);
	}

	void sendYesResponse()
	{
		LLSD notification;
		LLSD response;
		response = 1;
		mResponder(notification, response);
	}

	void sendNoResponse()
	{
		LLSD notification;
		LLSD response;
		response = 2;
		mResponder(notification, response);
	}

	void sendBogusResponse()
	{
		LLSD notification;
		LLSD response;
		response = 666;
		mResponder(notification, response);
	}

	int addedCount() { return mAddedCount; }
};

S32 LLNotification::getSelectedOption(const LLSD& notification, const LLSD& response)
{
	return response.asInteger();
}

// misc
std::string xml_escape_string(const std::string& in)
{
	return in;
}

/*****************************************************************************
*   TUT
*****************************************************************************/
namespace tut
{
    struct lllogininstance_data
    {
		lllogininstance_data() : logininstance(LLLoginInstance::getInstance())
		{
			// Global initialization
			gLoginURI.clear();
			gLoginCreds.clear();
			gDisconnectCalled = false;

			// gTOSType = -1; // Set to invalid value.
			gTOSCallback = 0; // clear the callback.


			gSavedSettings.declareBOOL("NoInventoryLibrary", FALSE, "", FALSE);
			gSavedSettings.declareBOOL("ConnectAsGod", FALSE, "", FALSE);
			gSavedSettings.declareBOOL("UseDebugMenus", FALSE, "", FALSE);
			gSavedSettings.declareBOOL("ForceMandatoryUpdate", FALSE, "", FALSE);
			gSavedSettings.declareString("ClientSettingsFile", "test_settings.xml", "", FALSE);
			gSavedSettings.declareString("VersionChannelName", "test_version_string", "", FALSE);
			gSavedSettings.declareString("NextLoginLocation", "", "", FALSE);
			gSavedSettings.declareBOOL("LoginLastLocation", FALSE, "", FALSE);

			credentials["first"] = "testfirst";
			credentials["last"] = "testlast";
			credentials["passwd"] = "testpass";

			logininstance->setNotificationsInterface(&notifications);
		}

		LLLoginInstance* logininstance;
		LLSD credentials;
		MockNotifications notifications;
    };

    typedef test_group<lllogininstance_data> lllogininstance_group;
    typedef lllogininstance_group::object lllogininstance_object;
    lllogininstance_group llsdmgr("lllogininstance");

    template<> template<>
    void lllogininstance_object::test<1>()
    {
		set_test_name("Test Simple Success And Disconnect");

		// Test default connect.
		logininstance->connect(credentials);

		ensure_equals("Default connect uri", gLoginURI, VIEWERLOGIN_URI); 

		// Dummy success response.
		LLSD response;
		response["state"] = "online";
		response["progress"] = 1.0;
		response["transfer_rate"] = 7;
		response["data"] = "test_data";

		gTestPump.post(response);

		ensure("Success response", logininstance->authSuccess());
		ensure_equals("Test Response Data", logininstance->getResponse().asString(), "test_data");

		logininstance->disconnect();

		ensure_equals("Called Login Module Disconnect", gDisconnectCalled, true);

		response.clear();
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 0;
		response["data"] = "test_data";

		gTestPump.post(response);

		ensure("Disconnected", !(logininstance->authSuccess()));
    }

    template<> template<>
    void lllogininstance_object::test<2>()
    {
		set_test_name("Test User TOS/Critical message Interaction");

		const std::string test_uri = "testing-uri";

		// Test default connect.
		logininstance->connect(test_uri, credentials);

		// connect should call LLLogin::connect to init gLoginURI and gLoginCreds.
		ensure_equals("Default connect uri", gLoginURI, "testing-uri"); 
		ensure_equals("Default for agree to tos", gLoginCreds["params"]["agree_to_tos"].asBoolean(), false);
		ensure_equals("Default for read critical", gLoginCreds["params"]["read_critical"].asBoolean(), false);

		// TOS failure response.
		LLSD response;
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 7;
		response["data"]["reason"] = "tos";
		gTestPump.post(response);

		ensure_equals("TOS Dialog type", gTOSType, LLFloaterTOS::TOS_TOS);
		ensure("TOS callback given", gTOSCallback != 0);
		gTOSCallback(false); // Call callback denying TOS.
		ensure("No TOS, failed auth", logininstance->authFailure());

		// Start again.
		logininstance->connect(test_uri, credentials);
		gTestPump.post(response); // Fail for tos again.
		gTOSCallback(true); // Accept tos, should reconnect w/ agree_to_tos.
		ensure_equals("Accepted agree to tos", gLoginCreds["params"]["agree_to_tos"].asBoolean(), true);
		ensure("Incomplete login status", !logininstance->authFailure() && !logininstance->authSuccess());
	
		// Fail connection, attempt connect again.
		// The new request should have reset agree to tos to default.
		response["data"]["reason"] = "key"; // bad creds.
		gTestPump.post(response);
		ensure("TOS auth failure", logininstance->authFailure());

		logininstance->connect(test_uri, credentials);
		ensure_equals("Reset to default for agree to tos", gLoginCreds["params"]["agree_to_tos"].asBoolean(), false);

		// Critical Message failure response.
		logininstance->connect(test_uri, credentials);
		response["data"]["reason"] = "critical"; // Change response to "critical message"
		gTestPump.post(response);

		ensure_equals("TOS Dialog type", gTOSType, LLFloaterTOS::TOS_CRITICAL_MESSAGE);
		ensure("TOS callback given", gTOSCallback != 0);
		gTOSCallback(true); 
		ensure_equals("Accepted read critical message", gLoginCreds["params"]["read_critical"].asBoolean(), true);
		ensure("Incomplete login status", !logininstance->authFailure() && !logininstance->authSuccess());

		// Fail then attempt new connection
		response["data"]["reason"] = "key"; // bad creds.
		gTestPump.post(response);
		ensure("TOS auth failure", logininstance->authFailure());
		logininstance->connect(test_uri, credentials);
		ensure_equals("Default for agree to tos", gLoginCreds["params"]["read_critical"].asBoolean(), false);
	}

    template<> template<>
    void lllogininstance_object::test<3>()
    {
		set_test_name("Test Mandatory Update User Accepts");

		// Part 1 - Mandatory Update, with User accepts response.
		// Test connect with update needed.
		logininstance->connect(credentials);

		ensure_equals("Default connect uri", gLoginURI, VIEWERLOGIN_URI); 

		// Update needed failure response.
		LLSD response;
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 7;
		response["data"]["reason"] = "update";
		gTestPump.post(response);

		ensure_equals("Notification added", notifications.addedCount(), 1);

		notifications.sendYesResponse();

		ensure("Disconnected", !(logininstance->authSuccess()));
	}

	template<> template<>
    void lllogininstance_object::test<4>()
    {
		set_test_name("Test Mandatory Update User Decline");

		// Test connect with update needed.
		logininstance->connect(credentials);

		ensure_equals("Default connect uri", gLoginURI, VIEWERLOGIN_URI); 

		// Update needed failure response.
		LLSD response;
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 7;
		response["data"]["reason"] = "update";
		gTestPump.post(response);

		ensure_equals("Notification added", notifications.addedCount(), 1);
		notifications.sendNoResponse();

		ensure("Disconnected", !(logininstance->authSuccess()));
	}

	template<> template<>
    void lllogininstance_object::test<6>()
    {
		set_test_name("Test Optional Update User Accept");

		// Part 3 - Mandatory Update, with bogus response.
		// Test connect with update needed.
		logininstance->connect(credentials);

		ensure_equals("Default connect uri", gLoginURI, VIEWERLOGIN_URI); 

		// Update needed failure response.
		LLSD response;
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 7;
		response["data"]["reason"] = "optional";
		gTestPump.post(response);

		ensure_equals("Notification added", notifications.addedCount(), 1);
		notifications.sendYesResponse();

		ensure("Disconnected", !(logininstance->authSuccess()));
	}

	template<> template<>
    void lllogininstance_object::test<7>()
    {
		set_test_name("Test Optional Update User Denies");

		// Part 3 - Mandatory Update, with bogus response.
		// Test connect with update needed.
		logininstance->connect(credentials);

		ensure_equals("Default connect uri", gLoginURI, VIEWERLOGIN_URI); 

		// Update needed failure response.
		LLSD response;
		response["state"] = "offline";
		response["progress"] = 0.0;
		response["transfer_rate"] = 7;
		response["data"]["reason"] = "optional";
		gTestPump.post(response);

		ensure_equals("Notification added", notifications.addedCount(), 1);
		notifications.sendNoResponse();

		// User skips, should be reconnecting.
		ensure_equals("reconnect uri", gLoginURI, VIEWERLOGIN_URI); 
		ensure_equals("skipping optional update", gLoginCreds["params"]["skipoptional"].asBoolean(), true); 
	}
}
