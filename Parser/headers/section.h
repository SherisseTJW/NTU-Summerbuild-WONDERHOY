#ifndef BEATMAPSECTION_H
#define BEATMAPSECTION_H

#include <map>
#include <string>

namespace beatmap{
    class Section{
        public:
            void loadAttributes(std::map<std::string, std::string>);
    };

}

#endif