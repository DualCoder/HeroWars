#ifndef _PARTY_H
#define _PARTY_H
#include <string>

#include "Team.h"

/**
* Class that represent a party (team, players, etc...) and its context (which map, how many players, etc...)
*/
class Party
{
private: 
	enum TeamColor
	{
		TC_BLUE = 0,
		TC_RED = 1
	};
public:
	Party();
	void setBlueTeam(Team& t);
	void setRedTeam(Team& t);
	void sendMessageToAll(std::string message);

private:
	void setTeam(Team& t, TeamColor c);

private:
	Team _teams[2];
};
#endif