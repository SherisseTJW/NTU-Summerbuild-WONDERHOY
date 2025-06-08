#include "../headers/parser.h"

beatmap::Beatmap beatmap::parseBeatmap(std::string path){
    std::string line;
    std::ifstream fileStream(path);
    beatmap::Beatmap beatmap = Beatmap();
    std::string currentSection = "";
    std::map<std::string,std::string> attributes;
    beatmap::GeneralSection generalSection;
    while(std::getline(fileStream, line)){
        if(line.empty()){
            if(currentSection != ""){
                if(currentSection == "General"){
                    generalSection.loadAttributes(attributes);
                }
                currentSection = "";
                attributes.clear();
            }
        }
        else{
            size_t sqbrac = line.find("[");
            if( sqbrac != std::string::npos){
                currentSection = line.substr(sqbrac + 1, line.find("]") - sqbrac - 1);
            }
            else if(!currentSection.empty()){
                size_t separator = line.find(":");
                std::string key = line.substr(0, separator);
                std::string value = line.substr(separator+1);
                if(value.c_str()[0] == ' '){
                    value = value.substr(1);
                }
                attributes[key] = value;
            }
        }
    }
    return beatmap;
}

void beatmap::GeneralSection::loadAttributes(std::map<std::string, std::string> attrs) {
    for (const auto& pair : attrs) {
        if (pair.first == "AudioFilename") {
            audioFilename = pair.second;
        }
        else if (pair.first == "AudioLeadin") {
            audioLeadin = std::stoi(pair.second);
        }
        else if (pair.first == "AudioHash") {
            audioHash = pair.second;
        }
        else if (pair.first == "PreviewTime") {
            previewTime = std::stoi(pair.second);
        }
        else if (pair.first == "Countdown") {
            countDown = std::stoi(pair.second);
        }
        else if (pair.first == "SampleSet") {
            if (pair.second == "Normal") sampleSet = beatmap::SampleSet::NORMAL;
            else if (pair.second == "Soft") sampleSet = beatmap::SampleSet::SOFT;
            else if (pair.second == "Drum") sampleSet = beatmap::SampleSet::DRUM;
        }
        else if (pair.first == "StackLeniency") {
            stackLeniency = std::stod(pair.second);
        }
        else if (pair.first == "Mode") {
            mode = std::stoi(pair.second);
        }
        else if (pair.first == "LetterboxInBreaks") {
            letterboxInBreaks = (pair.second == "1");
        }
        else if (pair.first == "StoryFireInFront") {
            storyFireInFront = (pair.second == "1");
        }
        else if (pair.first == "UseSkinSprites") {
            useSkinSprites = (pair.second == "1");
        }
        else if (pair.first == "AlwaysShowPlayfield") {
            alwaysShowPlayfield = (pair.second == "1");
        }
        else if (pair.first == "OverlayPosition") {
            if (pair.second == "NoChange") overlayPosition = beatmap::OverlayPosition::NOCHANGE;
            else if (pair.second == "Below") overlayPosition = beatmap::OverlayPosition::BELOW;
        }
        else if (pair.first == "SkinPreference") {
            skinPreference = pair.second;
        }
        else if (pair.first == "EpilepsyWarning") {
            epilepsyWarning = (pair.second == "1");
        }
        else if (pair.first == "CountdownOffset") {
            countdownOffset = std::stoi(pair.second);
        }
        else if (pair.first == "SpecialStyle") {
            specialStyle = (pair.second == "1");
        }
        else if (pair.first == "WidescreenStoryboard") {
            wideScreenStoryboard = (pair.second == "1");
        }
        else if (pair.first == "SamplesMatchPlaybackRate") {
            samplesMatchPlaybackRate = (pair.second == "1");
        }
    }
}
