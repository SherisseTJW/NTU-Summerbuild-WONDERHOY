#ifndef BEATMAPPARSER_H
#define BEATMAPPARSER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include "sections/general.h"
#include "sections/metadata.h"
#include "sections/diffculty.h"
#include "event.h"
#include "timing.h"
#include "hit-object.h"

namespace beatmap{

    class BeatmapNotExistException : public std::exception {
        private:
            std::string message;
        public:

            // Constructor accepting const char*
            BeatmapNotExistException(const char* msg) :
            message(msg) {}

            // Override what() method, marked
            // noexcept for modern C++
            const char* what() const noexcept {
                return message.c_str();
            }
    };

    class Beatmap{
        beatmap::GeneralSection generalSection;
        beatmap::MetadataSection metadataSection;
        beatmap::DifficultySection difficultySection;
        std::vector<beatmap::Event> events;
        std::vector<beatmap::TimingPoints> timingPoints;
        std::vector<beatmap::HitObject*> hitObjects; 
        int maxCombo = 0;
        int currCombo = 0;
        int judgements[5] = {0, 0, 0, 0, 0};
        public:
            Beatmap(
                beatmap::GeneralSection _gs,
                beatmap::MetadataSection _ms,
                beatmap::DifficultySection _ds,
                std::vector<beatmap::Event> _events,
                std::vector<beatmap::TimingPoints> _timingPoints,
                std::vector<beatmap::HitObject*> _hitObjects
            ) : generalSection(_gs),
                metadataSection(_ms),
                difficultySection(_ds),
                events(_events),
                timingPoints(_timingPoints),
                hitObjects(_hitObjects)
            {}
            
            std::vector<beatmap::HitObject*> getHitObjects(){
                return (this->hitObjects);
            }

            int getMaxCombo(){
                return this->maxCombo;
            }

            int* getAllJudgements(){
                return this->judgements;
            }

            int getAudioLeadIn(){
                return this->generalSection.getAudioLeadIn();
            }

            beatmap::MetadataSection getMetadataSection(){
                return this->metadataSection;
            }

            std::string getAudioFileName(){
                return std::regex_replace(this->metadataSection.getTitle(), std::regex("[ ()]"), "_");
            }

            beatmap::HitObject::Judgement getJudgement(int time, beatmap::HitObject* hitObject, bool followed = false);
    };

    Beatmap parseBeatmap(std::string path);

}

#endif