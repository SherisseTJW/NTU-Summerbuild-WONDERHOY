#include "../headers/parser.h"

beatmap::Beatmap beatmap::parseBeatmap(std::string path){
    std::string line;
    std::ifstream fileStream(path);
    if(fileStream.bad()){
        throw beatmap::BeatmapNotExistException("Beatmap does not exist!");
    }
    std::string currentSection = "";
    std::map<std::string,std::string> attributes;
    beatmap::GeneralSection generalSection;
    beatmap::MetadataSection metadataSection;
    beatmap::DifficultySection difficultySection;
    std::vector<beatmap::Event> events;
    std::vector<beatmap::TimingPoints> timings;
    std::vector<beatmap::HitObject*> hitObjects;
    while(std::getline(fileStream, line)){
        if(line.c_str()[0] == '/' && line.c_str()[1] == '/'){
            continue;
        }
        if(line.empty()){
            if(currentSection != ""){
                if(currentSection == "General"){
                    generalSection.loadAttributes(attributes);
                }
                else if(currentSection == "Metadata"){
                    metadataSection.loadAttributes(attributes);
                }
                else if(currentSection == "Difficulty"){
                    difficultySection.loadAttributes(attributes);
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
            else if(currentSection == "Events"){
                try{
                    events.push_back(beatmap::Event::parseEvent(line));
                }catch(beatmap::InvalidEventException e){
                    continue;
                }
            }
            else if(currentSection == "TimingPoints"){
                beatmap::TimingPoints timing =  beatmap::TimingPoints(line);
                timings.push_back(timing);
            }
            else if(currentSection == "HitObjects"){
                beatmap::HitObject* object =  beatmap::HitObject::parseHitObjects(line);
                hitObjects.push_back(object);
            }
            else if(!currentSection.empty()){
                size_t separator = line.find(":");
                if(separator != std::string::npos){
                    std::string key = line.substr(0, separator);
                    std::string value = line.substr(separator+1);
                    if(value.c_str()[0] == ' '){
                        value = value.substr(1);
                    }
                    attributes[key] = value;
                }
                
            }
        }
    }
    beatmap::Beatmap beatmap = Beatmap(generalSection, metadataSection, difficultySection, events, timings, hitObjects);
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

void beatmap::MetadataSection::loadAttributes(std::map<std::string, std::string> attrs) {
    for (const auto& pair : attrs) {
        if (pair.first == "Title") {
            title = pair.second;
        }
        else if (pair.first == "TitleUnicode") {
            titleUnicode = pair.second;
        }
        else if (pair.first == "Artist") {
            artist = pair.second;
        }
        else if (pair.first == "ArtistUnicode") {
            artistUnicode = pair.second; 
        }
        else if (pair.first == "Creator") {
            creator = pair.second;
        }
        else if (pair.first == "Version") {
            version = pair.second;
        }
        else if (pair.first == "Source") {
            source = pair.second;
        }
        else if (pair.first == "Tags") {
            tags = pair.second; 
        }
        else if (pair.first == "BeatmapID") {
            beatmapID = std::stoi(pair.second);
        }
        else if (pair.first == "BeatmapSetID") {
            beatmapSetID = std::stoi(pair.second);
        }
    }
}

void beatmap::DifficultySection::loadAttributes(std::map<std::string, std::string> attrs){
    for (const auto& pair : attrs) {
        if (pair.first == "HpDrainRate") {
            hpDrainRate = std::stod(pair.second);
        }
        else if (pair.first == "CircleSize") {
            circleSize = std::stod(pair.second);
        }
        else if (pair.first == "OverallDifficulty") {
            overallDifficulty = std::stod(pair.second);
        }
        else if (pair.first == "ApproachRate") {
            approachRate = std::stod(pair.second);
        }
        else if (pair.first == "SliderMultiplier") {
            sliderMultiplier = std::stod(pair.second);
        }
        else if (pair.first == "SliderTickRate") {
            sliderTickRate = std::stod(pair.second);
        }
    }
}

beatmap::HitObject::Judgement beatmap::Beatmap::getJudgement(int _time, beatmap::HitObject* hitObject, bool followed){
    beatmap::HitObject::Judgement judgement;
    if(hitObject->getType() == beatmap::HitObject::SLIDER){
        judgement = hitObject->setJudgement(_time, followed);
    }
    judgement = hitObject->setJudgement(_time);
    judgements[judgement]++;
    switch(judgement){
        case beatmap::HitObject::PERFECT:
        case beatmap::HitObject::GREAT:
        case beatmap::HitObject::GOOD:
        case beatmap::HitObject::BAD:
            currCombo += 1;
            if(maxCombo < currCombo){
                maxCombo = currCombo;
            }
            break;
        default:
            currCombo = 0;
    }
    return judgement;
}
