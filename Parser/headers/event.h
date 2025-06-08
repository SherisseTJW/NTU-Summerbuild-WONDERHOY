#ifndef BEATMAPEVENT_H
#define BEATMAPEVENT_H

#include <string>
#include <vector>
#include <iostream>

namespace beatmap{
    std::vector<std::string> split(const std::string& s, const std::string& delimiter);

    class InvalidEventException : public std::exception {
        private:
            std::string message;
        public:

            // Constructor accepting const char*
            InvalidEventException(const char* msg) :
            message(msg) {}

            // Override what() method, marked
            // noexcept for modern C++
            const char* what() const noexcept {
                return message.c_str();
            }
    };

    class Event{
        int startTime;

        public:
            static beatmap::Event parseEvent(std::string line);

        protected:
            Event(int startTime){
                this->startTime = startTime;
            }
    };

    class BackgroundEvent : public Event{
        std::string filename;
        int x;
        int y;
        public:
            BackgroundEvent(int start, std::string filname, int x, int y) : Event(start){
                
                this->filename = filename;
                this->x = x;
                this->y = y;  
            }
    };

    class VideoEvent : public Event{
        std::string filename;
        int x;
        int y;
        public:
            VideoEvent(int start, std::string filname, int x, int y) : Event(start){
                this->filename = filename;
                this->x = x;
                this->y = y;  
            }
    };

    class BreakEvent : public Event{
        int endTime;
        public:
            BreakEvent(int start, int endTime) : Event(start){
                this->endTime = endTime;
            }
    };
    
}

#endif