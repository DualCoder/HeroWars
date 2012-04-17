#ifndef _TEAM_H
#define _TEAM_H
#include <memory>

#include "Player.h"

using namespace std;


/**
* Class that represent a team, vector of players
*/
class Team
{
public:
	Team();
	~Team();
	void addPlayer(shared_ptr<Player> player);
	bool sendChatMessage(char* message); //send a chat message to this team (iterate on player team and send message)

	//!TODO vectors of players and everything related to a team
};
#endif