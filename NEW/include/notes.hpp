#pragma once

class BeatMap;

class Note{
public:
    int x;
    int time;
    int column;
    bool hit;
    BeatMap* map;



    void updateColumn();
};

class Slider : public Note{};