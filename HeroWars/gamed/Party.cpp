#include "Party.h"
using namespace std;

Party::Party()
{
	memset(_teams,0,sizeof(_teams));
}

void Party::setBlueTeam(Team& t)
{
	setTeam(t,TC_BLUE);
}

void Party::setRedTeam(Team& t)
{
	setTeam(t,TC_RED);
}

void Party::setTeam(Team& t, TeamColor c)
{
	_teams[c] = t;
}

void Party::sendMessageToAll(string message)
{

}