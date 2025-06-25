#ifndef difficulty_h
#define difficulty_h

#include "../section.h"

namespace beatmap{
    class DifficultySection : public beatmap::Section {
        double hpDrainRate;
        double circleSize;
        double overallDifficulty;
        double approachRate;
        double sliderMultiplier;
        double sliderTickRate;
        
        public:
            void loadAttributes(std::map<std::string, std::string>);
    };
}

#endif