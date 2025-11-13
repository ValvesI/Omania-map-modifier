#include <string>
#include <vector>
#include "notes.h"

class BeatMap{
    public:
        std::string title;
        std::string author;
        std::vector<std::vector<Note*>*> Chords;
        std::vector<Note*> notes;
        std::vector<Slider*> sliders;
        std::vector<int> bpm;
        int length;
        int noteCount;
        int sliderCount;
        int od;
        int hp;
        int keyCount;

        //Resets all the map's patterns
        void normalize();

        //Erases all the map's notes
        void reset();

        //Saves the notes maps in the output path
        bool save();

        //Reloads 

};