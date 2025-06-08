#ifndef METADATASECTION_H
#define METADATASECTION_H

#include "../section.h"

namespace beatmap{
    class MetadataSection: public beatmap::Section{
        std::string title;
        std::string titleUnicode;
        std::string artist;
        std::string artistUnicode;
        std::string creator;
        std::string version;
        std::string source;
        std::string tags;
        int beatmapID;
        int beatmapSetID;

        public:
            void loadAttributes(std::map<std::string, std::string>);
    };
}

#endif