#include "DCInstance.h"

#define DISCORD_INITIALIZE_SIG(name) void (name)(const char* appId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
typedef DISCORD_INITIALIZE_SIG(Discord_InitializeProc);

#define DISCORD_UPDATEPRESENCE_SIG(name) void (name)(const DiscordRichPresence* presence)
typedef DISCORD_UPDATEPRESENCE_SIG(Discord_UpdatePresenceProc);

DISCORD_INITIALIZE_SIG(Discord_InitializeStub)
{
}

DISCORD_UPDATEPRESENCE_SIG(Discord_UpdatePresenceStub)
{
}

static Discord_InitializeProc* Discord_InitializePtr = nullptr;
static Discord_UpdatePresenceProc* Discord_UpdatePresencePtr = nullptr;

static HMODULE discordRPCDLL;

void DCInstance::InitializeRPC(const char* applicationID)
{
	Discord_InitializePtr = Discord_InitializeStub;
	Discord_UpdatePresencePtr = Discord_UpdatePresenceStub;

	discordRPCDLL = LoadLibraryA("discord-rpc.dll");
	if (discordRPCDLL)
	{
		Discord_InitializePtr = (Discord_InitializeProc*)GetProcAddress(discordRPCDLL, "Discord_Initialize");
		if (!Discord_InitializePtr)
			Discord_InitializePtr = Discord_InitializeStub;

		Discord_UpdatePresencePtr = (Discord_UpdatePresenceProc*)GetProcAddress(discordRPCDLL, "Discord_UpdatePresence");
		if (!Discord_UpdatePresencePtr)
			Discord_UpdatePresencePtr = Discord_UpdatePresenceStub;
	}

	Discord_InitializePtr(applicationID, NULL, 1, NULL); 

	memset(&DCInstance::Presence, 0, sizeof(DCInstance::Presence));

	auto _currTime = chrono::system_clock::now().time_since_epoch();
	auto _secondCast = chrono::duration_cast<chrono::seconds>(_currTime);

	DCInstance::Presence.startTimestamp = _secondCast.count();
	Discord_UpdatePresencePtr(&DCInstance::Presence);
}

void DCInstance::UpdateDetails(const char* input)
{
	DCInstance::Presence.details = input;
	Discord_UpdatePresencePtr(&DCInstance::Presence);
}

void DCInstance::UpdateState(const char* input)
{
	DCInstance::Presence.state = input;
	Discord_UpdatePresencePtr(&DCInstance::Presence);
}

void DCInstance::UpdateLImage(const char* key, const char* text)
{
	DCInstance::Presence.largeImageKey = key;
	DCInstance::Presence.largeImageText = text;
	Discord_UpdatePresencePtr(&DCInstance::Presence);
}

void DCInstance::UpdateSImage(const char* key, const char* text)
{
	DCInstance::Presence.smallImageKey = key;
	DCInstance::Presence.smallImageText = text;
	Discord_UpdatePresencePtr(&DCInstance::Presence);
}