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
#include "Settings.h"

bool Settings::_initialized;
bool Settings::dataChanged;
char* Settings::FileName;
map <string, string> Settings::data;

bool Settings::initialize(char* filename)
{
		string s, key, value;
		FileName = filename;

		ifstream ins(filename);
		if(!ins.is_open())
			return false;

		// For each (key, value) pair in the file
		while (getline( ins, s ))
		{
			string::size_type begin = s.find_first_not_of( " \f\t\v" );

			// Skip blank lines
			if (begin == string::npos) continue;

			// Skip commentary
			if (string( "#;" ).find( s[ begin ] ) != string::npos) continue;

			// Extract the key value
			string::size_type end = s.find( '=', begin );
			key = s.substr( begin, end - begin );

			// (No leading or trailing whitespace allowed)
			key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

			// No blank keys allowed
			if (key.empty()) continue;

			// Extract the value (no leading or trailing whitespace allowed)
			begin = s.find_first_not_of( " \f\n\r\t\v", end + 1 );
			end   = s.find_last_not_of(  " \f\n\r\t\v" ) + 1;

			value = s.substr( begin, end - begin );

			// Insert the properly extracted (key, value) pair into the map
			transform(key.begin(), key.end(), key.begin(), tolower);
			data[ key ] = value;
			dataChanged = false;
		}
		_initialized = true;
		return true;
}

string Settings::getValue(string key)
{
	if(!_initialized)
		return "";
	return data[key];
}
bool Settings::getValueAsBool(string key)
{
	return (data.find(key)->second.at(0) == '1' || data.find(key)->second == "true" || data.find(key)->second == "TRUE" || data.find(key)->second == "True");
}

void Settings::setValue(string key, string value)
{
	data[key] = value;
	dataChanged = true;
}

bool Settings::save()
{
	if(!dataChanged) return true;
	map<string,string>::iterator iter;
	ofstream outs(FileName);

	for (iter = data.begin(); iter != data.end(); iter++)
		outs << iter->first << " = " << iter->second << endl;

	return true;
}

void Settings::SetDefaultValues()
{
	data.clear();
	data["dbtype"] = "mysql";
	data["lport"] = "5100";
	data["dbhost"] = "host";
	data["dbport"] = "3306";
	data["dbuser"] = "username";
	data["dbpass"] = "password";
	data["dbdatabase"] = "database";
	dataChanged = true;
}
