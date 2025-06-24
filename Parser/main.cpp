#include "headers/parser.h"
using namespace std;

int main(){
    beatmap::Beatmap beatmap =  beatmap::parseBeatmap("test.osu");
    std::vector<beatmap::HitObject*> objects = beatmap.getHitObjects();
    int kek[5] = {0,0,0,0,0};
    for(beatmap::HitObject* object : objects){
        std::cout << object->getTime() << std::endl;
        if(object->getType() == beatmap::HitObject::SLIDER){
            for( beatmap::Coord sad : (object)->getAnchorPoints()){
                std::cout << sad.getX() << ":" << sad.getY() << std::endl;
            }
        }
        int time = object->getTime() + (std::rand() % 252 - 126);
        kek[beatmap.getJudgement(time, object)]++;
        
    }
    int* sad = beatmap.getAllJudgements();
    printf("%d\n", objects.size());
    for(int i = 0; i < 5; i++){
        printf("%d, %d\n", kek[i], sad[i]);
    }
}