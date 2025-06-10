#include "../headers/event.h"

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