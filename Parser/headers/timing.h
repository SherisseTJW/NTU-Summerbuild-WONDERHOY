#ifndef BEATMAPTIMING_H
#define BEATMAPTIMING_H

#include "util.h"

namespace beatmap{

    class TimingPoints{
        int time;
        double beatLength;
        int meter;
        beatmap::SampleSet sampleSet;
        int sampleIndex;
        int volume;
        bool uninherited;
        int effects;

        public:
            TimingPoints(std::string line);
    };

}

#endif