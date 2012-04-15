#ifndef _GAME_SERVER_H
#define _GAME_SERVER_H
#include "NetworkListener.h"
#include "StateManager.h"

#include <memory>

class GameServer
{
public:
	GameServer(const char* serverName) : _serverName(serverName), _running(false), _adress(NULL), _listener(NULL), _gameStateManager(NULL), _frameRate(60){ }
	~GameServer();
	void Initialize(ENetAddress& serverAdress, const char* baseKey, unsigned int frameRate = 60);
	void start();
	void stop();
	bool isRunning() const;

private:
	const char* _serverName;
	const char* _baseKey;
	bool _running;
	ENetAddress* _adress;
	shared_ptr<NetworkListener> _listener;
	shared_ptr<StateManager> _gameStateManager;
	unsigned int _frameRate; //by default at 60
};
#endif