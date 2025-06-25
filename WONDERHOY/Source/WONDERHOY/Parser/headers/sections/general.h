#ifndef GENERALSECTION_H
#define GENERALSECTION_H

#include "../section.h"
#include "../util.h"

namespace beatmap{

    class GeneralSection : public beatmap::Section {
        std::string audioFilename = "";
        int audioLeadin = 0;
        std::string audioHash = "";
        int previewTime = -1;
        int countDown = 1;
        beatmap::SampleSet sampleSet = beatmap::SampleSet::NORMAL;
        double stackLeniency = 0.7;
        int mode = 0;
        bool letterboxInBreaks = false;
        bool storyFireInFront = true;
        bool useSkinSprites = false;
        bool alwaysShowPlayfield = false;
        beatmap::OverlayPosition overlayPosition = beatmap::OverlayPosition::NOCHANGE;
        std::string skinPreference = "";
        bool epilepsyWarning = false;
        int countdownOffset = 0;
        bool specialStyle = false;
        bool wideScreenStoryboard = false;
        bool samplesMatchPlaybackRate = false;

        public:
            void loadAttributes(std::map<std::string, std::string>);

            int getAudioLeadIn(){
                return this->audioLeadin;
            }

            int getPreviewTime(){
                return this->previewTime;
            }

            std::pair<int,int> getMapResolution(){
                if(this->wideScreenStoryboard){
                    return beatmap::wideStoryBoard;
                }
                return beatmap::defaultStoryBoard;
            }
    };
}

#endif