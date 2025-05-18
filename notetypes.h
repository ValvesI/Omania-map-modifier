#ifndef NOTE_TYPES_H
#define NOTE_TYPES_H

class note{
    public:
        int x;
        int y;
        int time;
        int type;
        int idk;
        int releaseTime;
        note* nextNote = nullptr;
        note* prevNote = nullptr;
        bool isJack = false;
        bool createdNote;

        note(int x, int y, int time, int type, int idk, int releaseTime)
        :x(x), y(y), time(time), type(type), idk(idk), releaseTime(releaseTime){}
};


class slider{
    public:
    int x;
    int time;
    int releaseTime;

    slider(int x, int time, int releaseTime)
    :x(x), time(time), releaseTime(releaseTime){}
};

#endif