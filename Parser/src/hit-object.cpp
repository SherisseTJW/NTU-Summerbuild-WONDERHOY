#include "../headers/hit-object.h"

beatmap::HitObject beatmap::HitObject::parseHitObjects(std::string line){
    std::vector<std::string> params = beatmap::split(line, ",");

    int type = std::stoi(params[3]);

    if(beatmap::checkFlag(type, 0)){
        beatmap::HitObject object = beatmap::HitObject(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            beatmap::HIT_CIRCLE
        );
        return object;
    }
    else if(beatmap::checkFlag(type, 1)){
        std::vector<std::string>  slideParams = beatmap::split(params[5],  "|");
        beatmap::CurveType curveType;
        std::vector<beatmap::Coord> anchorPoints;
        switch(slideParams[0].c_str()[0]){
            case 'B':
                curveType = beatmap::BEZIER;
                break;
            case 'C':
                curveType = beatmap::CENTRIPETAL;
                break;
            case 'L':
                curveType = beatmap::LINEAR;
                break;
            case 'P':
                curveType = beatmap::PERFECT_CIRCLE;
                break;
            default:
                throw beatmap::InvalidCurveTypeException("Invalid Curve Type!");
        }
        for(int i = 1; i < slideParams.size(); i ++){
            std::vector<std::string> xy = beatmap::split(slideParams[i], ":");
            beatmap::Coord coord = beatmap::Coord(std::stoi(xy[0]), std::stoi(xy[1]));
            anchorPoints.push_back(coord);
        }
        beatmap::Slider slider = beatmap::Slider(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            curveType,
            anchorPoints,
            std::stoi(params[6]),
            std::stod(params[7])
        );
        return slider;
    }
    else if(beatmap::checkFlag(type,3)){
        beatmap::Spinning object = beatmap::Spinning(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            std::stoi(params[5])
        );
        return object;
    }
    else{
        throw beatmap::InvalidObjectTypeException("Invalid HitObject!");
    }
}

beatmap::Slider::Slider(int _x, int _y, int _time, int _hitSound, beatmap::CurveType _curveType, std::vector<beatmap::Coord> _anchorPoints, int _slides, double _length) : beatmap::HitObject(_x, _y, _time, _hitSound, beatmap::SLIDER){
    this->curveType = _curveType;
    this->anchorPoints = _anchorPoints;
    this->slides = _slides;
    this->length = _length;
}

beatmap::Spinning::Spinning(int _x, int _y, int _time, int _hitSound, int _endTime) : beatmap::HitObject(_x, _y, _time, _hitSound, beatmap::SPINNER){
    this->endTime = _endTime;
}