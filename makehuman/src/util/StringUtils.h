#pragma once

#include <algorithm>
#include <string>

inline bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
	auto it = toMatch.begin();
	return mainStr.size() >= toMatch.size() &&
	       std::all_of(std::next(mainStr.begin(),mainStr.size() - toMatch.size()), mainStr.end(), [&it](const char & c){
		       return c == *(it++)  ;
	       } );
}

inline bool endsWithCaseInsensitive(std::string mainStr, std::string toMatch)
{
	auto it = toMatch.begin();
	return mainStr.size() >= toMatch.size() &&
	       std::all_of(std::next(mainStr.begin(),mainStr.size() - toMatch.size()), mainStr.end(), [&it](const char & c){
		       return ::tolower(c) == ::tolower(*(it++))  ;
	       } );
	
	
}

inline std::string removeExtension(const std::string & filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot); 
}
