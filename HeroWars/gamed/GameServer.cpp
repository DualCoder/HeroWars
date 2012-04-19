#include "GameServer.h"
#include "LoadingState.h"

GameServer::~GameServer()
{
	if(isRunning())
	{
		stop();
	}
}

void GameServer::Initialize(ENetAddress& adress, const char* baseKey, unsigned int frameRate)
{
	_adress = &adress;
	_baseKey = baseKey;
	_frameRate = frameRate;
}

void GameServer::start()
{
	if(!_adress) return;	
	Log::getMainInstance()->writeLine("Starting %s \n",_serverName.c_str());

	_listener = shared_ptr<NetworkListener>(new NetworkListener());
	_listener->initialize(_adress,_baseKey);

	StateManager* stateManager = new StateManager();
	shared_ptr<Party> currentParty(new Party());
	_gameStateManager = shared_ptr<StateManager>(stateManager);
	_gameStateManager->pushState(shared_ptr<LoadingState>(new LoadingState(stateManager,currentParty,_listener->getPacketManager())));

	_running = true;

	unsigned int lastTime = GetTickCount();	
	while(_running && !_gameStateManager->isEmpty())
	{
		_listener->processPacket();
		unsigned int currentTime = GetTickCount();
		float alphaTime = (lastTime - currentTime)/ _frameRate;
		_gameStateManager->update(alphaTime);
		lastTime = currentTime;
	}
}

void GameServer::stop()
{
	_running = false;
}

bool GameServer::isRunning() const
{
	return _running;
}