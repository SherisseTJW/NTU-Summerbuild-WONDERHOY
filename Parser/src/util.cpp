#include "../headers/util.h"

std::vector<std::string> beatmap::split(const std::string& s, const std::string& delimiter){
        std::vector<std::string> res;
        size_t pos = 0;
        size_t oldPos = 0;
        while((pos = s.find(delimiter, oldPos)) != std::string::npos){
            res.push_back(s.substr(oldPos, pos));
            oldPos = pos + delimiter.length();
        }
        if(oldPos != s.size()){
            res.push_back(s.substr(oldPos));
        }
        return res;
} 

bool beatmap::checkFlag(int type, int pos){
        return type & pow(2,pos) != 0;
}