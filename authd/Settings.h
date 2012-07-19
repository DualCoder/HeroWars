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
#ifndef _SETTINGS_H
#define _SETTINGS_H


class Settings
{
public:
	static bool initialize(char* filename);
	static string getValue(string key);
	static bool getValueAsBool(string key);
	static void setValue(string key, string value);
	static bool save();
	static void SetDefaultValues();

private:
	static bool _initialized;
	static map <string, string> data;
	static char* FileName;
	static bool dataChanged;
};

#endif  //_SETTINGS_H
