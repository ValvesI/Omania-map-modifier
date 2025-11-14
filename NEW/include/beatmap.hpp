#pragma once
#include <string>
#include <vector>

#include "notes.hpp"

struct TimingPoint {
    int offset;
    double bpm;
    bool inherited;
};

class BeatMap {
public:
    // Metadata
    std::string title;
    std::string artist;
    std::string creator;
    std::string version;
    std::string audioFile;

    // Difficulty
    int keyCount;
    double od;
    double hp;
    std::vector<int> columns;

    // Timing
    std::vector<TimingPoint> timingPoints;

    // Hit objects
    std::vector<Note> notes;
    std::vector<Slider> sliders;
    

    // Initialization
    void load(const std::string& filepath);

    // Parsing
    std::vector<Note> parseNotes(const std::string& filePath);
    std::vector<Slider> parseSliders();
    std::vector<std::vector<Note*>> parseChords();
    std::vector<std::vector<Note*>> getChords();
    std::vector<Note*> getChord(int time);
    std::vector<int> getColumns();
    

    // Pattern tools
    std::vector<Note> normalize();
    void reset();

    //======== General Purposes ==========//
    
    // Returns the first free column in the given time
    int getFreeColumn(int time);

    // Export
    bool save(const std::string& outPath);
};
