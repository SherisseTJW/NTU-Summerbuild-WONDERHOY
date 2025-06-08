#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "sections/general.h"

namespace beatmap{

    inline std::pair<int,int> defaultStoryBoard = {640, 480};
    inline std::pair<int,int> wideStoryBoard = {854, 480};

    class Beatmap{
        beatmap::GeneralSection generalSection;
    };

    Beatmap parseBeatmap(std::string path);

}

#endif