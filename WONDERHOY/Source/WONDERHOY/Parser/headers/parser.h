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
#include "hit-object.h"

namespace beatmap{

    class Beatmap{
        beatmap::GeneralSection generalSection;
        beatmap::MetadataSection metadataSection;
        beatmap::DifficultySection difficultySection;
        std::vector<beatmap::Event> events;
        std::vector<beatmap::TimingPoints> timingPoints;
        std::vector<beatmap::HitObject> hitObjects; 

        public:
            Beatmap(
                beatmap::GeneralSection _gs,
                beatmap::MetadataSection _ms,
                beatmap::DifficultySection _ds,
                std::vector<beatmap::Event> _events,
                std::vector<beatmap::TimingPoints> _timingPoints,
                std::vector<beatmap::HitObject> _hitObjects
            ) : generalSection(_gs),
                metadataSection(_ms),
                difficultySection(_ds),
                events(_events),
                timingPoints(_timingPoints),
                hitObjects(_hitObjects)
            {}
            
            std::vector<beatmap::HitObject> getHitObjects(){
                return this->hitObjects;
            }
            
            
    };

    Beatmap parseBeatmap(std::string path);

}

#endif