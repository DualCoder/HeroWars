#ifndef _PLAYER_H
#define _PLAYER_H
/**
* Class that represent a party (players, etc...) and its context (which map, how many players, etc...)
*/

class Player
{
public:
	Player();
	~Player();
	bool sendChatMessage(char* message); //send a chat message to this player

	//!TODO champions , userID, team pointer , etc...
};
#endif