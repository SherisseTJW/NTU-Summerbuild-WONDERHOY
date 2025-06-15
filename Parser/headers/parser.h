#ifndef BEATMAPPARSER_H
#define BEATMAPPARSER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "sections/general.h"
#include "sections/metadata.h"
#include "sections/diffculty.h"
#include "event.h"
#include "timing.h"

namespace beatmap{

    class Beatmap{
        beatmap::GeneralSection generalSection;
        beatmap::MetadataSection metadataSection;
        beatmap::DifficultySection difficultySection;
    };

    Beatmap parseBeatmap(std::string path);

}

#endif