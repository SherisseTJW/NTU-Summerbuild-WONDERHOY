#ifndef BEATMAPHITOBJECT_H
#define BEATMAPHITOBJECT_H

#include <string>
#include <vector>
#include "util.h"

namespace beatmap{

    class InvalidCurveTypeException : public std::exception {
        private:
            std::string message;
        public:

            // Constructor accepting const char*
            InvalidCurveTypeException(const char* msg) :
            message(msg) {}

            // Override what() method, marked
            // noexcept for modern C++
            const char* what() const noexcept {
                return message.c_str();
            }
    };

    class InvalidObjectTypeException : public std::exception {
        private:
            std::string message;
        public:

            // Constructor accepting const char*
            InvalidObjectTypeException(const char* msg) :
            message(msg) {}

            // Override what() method, marked
            // noexcept for modern C++
            const char* what() const noexcept {
                return message.c_str();
            }
    };

    enum ObjectType{
        HIT_CIRCLE,
        SLIDER,
        SPINNER
    };

    class Coord{
        int x;
        int y;

        public:
            Coord(int _x, int _y): x(_x), y(_y){}
            
            int getX(){
                return this->x;
            }

            int getY(){
                return this->y;
            }
    };

    class HitObject{
        int x;
        int y;
        int time;
        int hitSound;
        beatmap::ObjectType type;

        public:
            HitObject(int _x, int _y, int _time, int _hitSound, beatmap::ObjectType _type):
                x(_x),
                y(_y),
                time(_time),
                hitSound(_hitSound),
                type(_type)
                {}

            static beatmap::HitObject parseHitObjects(std::string line);

            beatmap::Coord getCoords(){
                beatmap::Coord coord = beatmap::Coord(x, y);
                return coord; 
            }

            beatmap::ObjectType getType(){
                return this->type;
            }

            int getTime() {
                return this->time;
            }
    };

    class Slider : public HitObject{
        beatmap::CurveType curveType;
        std::vector<beatmap::Coord> anchorPoints;
        int slides;
        double length;

        public:
            Slider(int _x, int _y, int _time, int _hitSound, beatmap::CurveType _curveType, std::vector<beatmap::Coord> _anchorPoints, int _slides, double _length);
    };

    class Spinning : public HitObject{
        int endTime;

        public:
            Spinning(int _x, int _y, int _time, int _hitSound, int _endTime);
    };

}

#endif