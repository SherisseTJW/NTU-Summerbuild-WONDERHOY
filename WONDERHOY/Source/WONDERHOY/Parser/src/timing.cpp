#include "../headers/timing.h"

beatmap::TimingPoints::TimingPoints(std::string line){
    std::vector<std::string> params = beatmap::split(line, ",");
    time = std::stoi(params[0]);
    beatLength = std::stod(params[1]);
    meter = std::stoi(params[2]);
    int sample = std::stoi(params[3]);
    switch(sample){
        case 0:
            sampleSet = beatmap::SampleSet::BEATMAP_DEFAULT;
            break;
        case 1:
            sampleSet = beatmap::SampleSet::NORMAL;
            break;
        case 2:
            sampleSet = beatmap::SampleSet::SOFT;
            break;
        case 3:
            sampleSet = beatmap::SampleSet::DRUM;
            break;
    }
    sampleIndex = std::stoi(params[4]);
    volume = std::stoi(params[5]);
    uninherited = params[6] == "1";
    effects = std::stoi(params[7]);
}