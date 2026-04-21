#pragma once
namespace ARL {

inline void safeToLower(std::string& s)
{
	for(unsigned i = 0; i<s.size(); ++i){
		if(isupper(s[i])){
			s[i] = tolower(s[i]);
		}
	}
}

}
