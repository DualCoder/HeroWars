/*
HeroWars server for League of Legends 
Copyright (C) 2012  Intline9

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

title: authd Server
author: C0dR

*/
#include "stdafx.h"
#include "TCPServer.h"
#include "Settings.h"
#include "Log.h"

Ptr<TCPServer> _server;

bool initialize()
{
	Settings::initialize("settings.cfg");
	string port = Settings::getValue("lport");
	string dbtype = Settings::getValue("dbtype");
	if(port.empty() || dbtype.empty())
	{
		Logging->writeError("Unable to load settings. Setting up default values. Have a look into settings.cfg\n");
		Settings::SetDefaultValues();
		return false;
	}
	DatabaseType type = DT_MYSQL;
	if(dbtype == "mysql")
		type = DT_MYSQL;
	else if(dbtype == "postgre" || dbtype == "postgresql")
		type = DT_POSTGRESQL;

	_server = newPtr<TCPServer>(type);
	return _server->initialize(port);
}

void run()
{
	_server->run();
}

void PrintHeader()
{
	printf("HeroWars authd server %s\n",SERVER_VERSION);
	printf("Copyright (C) 2012 C0dR\n\n");
}

int main(int argc, char argv)
{


	PrintHeader();
	if(!initialize())
	{
		Logging->writeError("Failed to initialize!\n");
		if(_server) _server->shutdown();
		Settings::save();
		return -1;
	}
	run();
	_server->shutdown();
	Settings::save();
	return 0;
}

