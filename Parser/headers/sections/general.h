#include "../section.h"

namespace beatmap{

    enum SampleSet{
        NORMAL,
        SOFT,
        DRUM
    };

    enum OverlayPosition{
        NOCHANGE,
        BELOW,
        ALOW
    };

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
    };
}