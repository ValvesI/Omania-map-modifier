#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <map>
#include "notetypes.h"

using namespace std;

map<int, vector<note>> getNotes(string map_path, string output_path, vector<slider>& sliders);


int getChordCount(const vector<note>& chord);


note* getNoteAtTimeX(map<int, vector<note>>& mapNotes, int& time, int& x);


vector <note*> getSurroundingNotes(map<int, vector<note>>& mapNotes, int& time, int& x);


void atributeSurroundingNotes(map<int, vector<note>>& mapNotes, note& currentNote);


void atributeMapSurroundingNotes(map<int, vector<note>>& mapNotes);


int getNoteCount(map<int, vector<note>>& mapNotes);


int getJackCount(map<int, vector<note>>& mapNotes);


int getSliderCount(map<int, vector<note>>& mapNotes);


void randomEraseNote(vector<note>& chord);


void outPutMapNotes(map<int, vector<note>>& mapNotes, string output_path);


vector<int> getMapColumns(map<int, vector<note>>& mapNotes);


vector <int> getFreeColumns(int time, map<int, vector<note>>& mapNotes);


int getRandomColumn(vector<int>& freeColumns);


vector<int> getJackFreeColumns(int& time, map<int, vector<note>>& mapNotes);


void getSliderFreeColumns(vector<int>& freeColumns, vector<slider>& sliders, int time);


vector<int> getChordReleaseTimes(map<int,vector<note>>& mapNotes, int time);


int getRandomReleaseTime(vector<int>& releaseTimes);


bool verifyTimeXSlider(int column, int time, vector<slider>& sliders);


bool verifyReleaseTimeIsFree(int column, int releaseTime, map<int, vector<note>>& mapNotes, vector<slider>& sliders);


bool verifySliderDurationFree(int column, int time, int releaseTime, map<int, vector<note>>& mapNotes, vector<slider>& sliders);


void eraseNotesFromChord(map<int, vector<note>>& mapNotes, bool keepTrills = false, bool tillEspecific = false, int spcChord = 0);


void buffMap(map<int, vector<note>>& mapNotes, vector<slider>& sliders, int baseChordSize, bool avoidJacks = false);


void logDebug(const string& message);


void printInfo(map<int, vector<note>>& mapNotes, map<int, vector<note>>& modified, bool printNotes = false, bool printJackTimings = false);


void AwaitInput(string input);


void printHelp();

/*Destroys all patterns of the song and normalizes the notes starting from left to right*/
void normalizeNotes(map<int, vector<note>>& mapNotes);
#endif