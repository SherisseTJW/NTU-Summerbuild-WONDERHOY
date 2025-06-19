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
        public:
            enum ObjectType{
                HIT_CIRCLE,
                SLIDER,
                SPINNER
            };

            enum Judgement{
                MISS,
                BAD,
                GOOD,
                GREAT,
                PERFECT
            };

            enum CurveType{
                BEZIER,
                CENTRIPETAL,
                LINEAR,
                PERFECT_CIRCLE
            };

            HitObject(int _x, int _y, int _time, int _hitSound, beatmap::HitObject::ObjectType _type):
                x(_x),
                y(_y),
                time(_time),
                hitSound(_hitSound),
                type(_type)
                {}

            static beatmap::HitObject parseHitObjects(std::string& line);

            int getTime(){
                return this->time;
            }

            beatmap::Coord getCoords(){
                beatmap::Coord coord = beatmap::Coord(x, y);
                return coord; 
            }

            beatmap::HitObject::ObjectType getType(){
                return this->type;
            }

            beatmap::HitObject::Judgement setJudgement(int time);

            beatmap::HitObject::Judgement setJudgement(int time, bool followed);
            
        private:
            int x;
            int y;
            int time;
            int hitSound;
            beatmap::HitObject::ObjectType type;

    };

    class Slider : public HitObject{
        beatmap::HitObject::CurveType curveType;
        std::vector<beatmap::Coord> anchorPoints;
        int slides;
        double length;

        public:
            Slider(int _x, int _y, int _time, int _hitSound, beatmap::HitObject::CurveType _curveType, std::vector<beatmap::Coord> _anchorPoints, int _slides, double _length);

        beatmap::HitObject::CurveType getCurveType(){
            return this->curveType;
        } 

        std::vector<beatmap::Coord> getAnchorPoints(){
            return this->anchorPoints;
        }

        int geSlides(){
            return this->slides;
        }
    };

    class Spinning : public HitObject{
        int endTime;

        public:
            Spinning(int _x, int _y, int _time, int _hitSound, int _endTime);
    };

}

#endif