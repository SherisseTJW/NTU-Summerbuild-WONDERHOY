#include "../headers/hit-object.h"

beatmap::HitObject* beatmap::HitObject::parseHitObjects(std::string& line){
    std::vector<std::string> params = beatmap::split(line, ",");

    int type = std::stoi(params[3]);

    if(beatmap::checkFlag(type, 0)){
       return new beatmap::HitObject(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            beatmap::HitObject::HIT_CIRCLE
        );
    }
    else if(beatmap::checkFlag(type, 1)){
        std::vector<std::string>  slideParams = beatmap::split(params[5],  "|");
        beatmap::HitObject::CurveType curveType;
        std::vector<beatmap::Coord> anchorPoints;
        switch(slideParams[0].c_str()[0]){
            case 'B':
                curveType = beatmap::HitObject::BEZIER;
                break;
            case 'C':
                curveType = beatmap::HitObject::CENTRIPETAL;
                break;
            case 'L':
                curveType = beatmap::HitObject::LINEAR;
                break;
            case 'P':
                curveType = beatmap::HitObject::PERFECT_CIRCLE;
                break;
            default:
                throw beatmap::InvalidCurveTypeException("Invalid Curve Type!");
        }
        for(int i = 1; i < slideParams.size(); i ++){
            std::vector<std::string> xy = beatmap::split(slideParams[i], ":");
            beatmap::Coord coord = beatmap::Coord(std::stoi(xy[0]), std::stoi(xy[1]));
            anchorPoints.push_back(coord);
        }
        return new beatmap::Slider(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            curveType,
            anchorPoints,
            std::stoi(params[6]),
            std::stod(params[7])
        );
        
    }
    else if(beatmap::checkFlag(type,3)){
        return new beatmap::Spinning(
            std::stoi(params[0]),
            std::stoi(params[1]),
            std::stoi(params[2]),
            std::stoi(params[4]),
            std::stoi(params[5])
        );
    }
    else{
        throw beatmap::InvalidObjectTypeException("Invalid HitObject!");
    }
}

beatmap::Slider::Slider(int _x, int _y, int _time, int _hitSound, 
    beatmap::HitObject::CurveType _curveType, std::vector<beatmap::Coord> _anchorPoints, int _slides, double _length) : 
    beatmap::HitObject(_x, _y, _time, _hitSound, beatmap::HitObject::SLIDER){
    this->curveType = _curveType;
    this->anchorPoints = _anchorPoints;
    this->slides = _slides;
    this->length = _length;
}

beatmap::Spinning::Spinning(int _x, int _y, int _time, int _hitSound, int _endTime) : beatmap::HitObject(_x, _y, _time, _hitSound, beatmap::HitObject::SPINNER){
    this->endTime = _endTime;
}

beatmap::HitObject::Judgement beatmap::HitObject::setJudgement(int timeArg){
    int diff = abs(timeArg - this->time);
    if(diff < 42){
        return beatmap::HitObject::PERFECT;
    }
    else if(42 <= diff && diff < 83){
        return beatmap::HitObject::GREAT;
    }
    else if(83 <= diff && diff < 108){
        return beatmap::HitObject::GOOD;
    }
    else if(108 <= diff && diff < 125){
        return beatmap::HitObject::BAD;
    }
    return beatmap::HitObject::MISS;
}

beatmap::HitObject::Judgement beatmap::HitObject::setJudgement(int timeArg, bool followed){
    beatmap::HitObject::Judgement judgement = setJudgement(timeArg);
    if(type == beatmap::HitObject::SLIDER && !followed && judgement != beatmap::HitObject::MISS){
        switch(judgement){
            case beatmap::HitObject::PERFECT:
                judgement = beatmap::HitObject::GREAT;
            case beatmap::HitObject::GREAT:
                judgement = beatmap::HitObject::GOOD;
                break;
            case beatmap::HitObject::GOOD:
                judgement = beatmap::HitObject::BAD;
                break;
            default:   
                judgement = beatmap::HitObject::MISS;
        }
    }
    return judgement;
}