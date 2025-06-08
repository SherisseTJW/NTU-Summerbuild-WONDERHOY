#include "../headers/event.h"

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

beatmap::Event beatmap::Event::parseEvent(std::string line){
    std::vector<std::string> params = beatmap::split(line, ",");
    if(params[0] == "0"){
        if(params.size() == 3){
            return beatmap::BackgroundEvent(std::stoi(params[1]), params[2], 0, 0);
        }
        return beatmap::BackgroundEvent(std::stoi(params[1]), params[2], std::stoi(params[3]), std::stoi(params[4]));
    }
    else if(params[0] == "Video" || params[0] == "1"){
        if(params.size() == 3){
            return beatmap::VideoEvent(std::stoi(params[1]), params[2], 0, 0);
        }
        return beatmap::VideoEvent(std::stoi(params[1]), params[2], std::stoi(params[3]), std::stoi(params[4]));
    }
    else if(params[1] == "2"){
        return beatmap::BreakEvent(std::stoi(params[1]), std::stoi(params[2]));
    }
    throw beatmap::InvalidEventException("Invalid Event!");
}