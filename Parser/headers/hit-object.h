#ifndef BEATMAPHITOBJECT_H
#define BEATMAPHITOBJECT_H

#include <string>
#include <vector>
#include "util.h"

namespace beatmap{

    class HitObject{
        int x;
        int y;
        int time;
        int hitSound;

        public:
            HitObject(int _x, int _y, int _time, int _hitSound):
                x(_x),
                y(_y),
                time(_time),
                hitSound(_hitSound)
                {}

            static beatmap::HitObject parseHitObjects(std::string line);
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

}

#endif