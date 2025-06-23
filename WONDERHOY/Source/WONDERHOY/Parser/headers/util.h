#ifndef BEATMAPUTIL_H
#define BEATMAPUTIL_H

#include <vector>
#include <string>
#include <math.h>
#include <iostream>

namespace beatmap{

    inline std::pair<int,int> defaultStoryBoard = {640, 480};
    inline std::pair<int,int> wideStoryBoard = {854, 480};
    
    enum SampleSet{
        BEATMAP_DEFAULT,
        NORMAL,
        SOFT,
        DRUM
    };

    enum OverlayPosition{
        NOCHANGE,
        BELOW,
        ALOW
    };


    std::vector<std::string> split(const std::string& s, const std::string& delimiter);

    bool checkFlag(int type, int pos);
}

#endif