#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include "functions.h"

//I was stupid when started this sorry
using namespace std;

// class note{
//     public:
//         int x;
//         int y;
//         int time;
//         int type;
//         int idk;
//         int releaseTime;
//         note* nextNote = nullptr;
//         note* prevNote = nullptr;
//         bool isJack = false;
//         bool createdNote;

//         note(int x, int y, int time, int type, int idk, int releaseTime)
//         :x(x), y(y), time(time), type(type), idk(idk), releaseTime(releaseTime){}
// };

// class slider{
//     public:
//     int x;
//     int time;
//     int releaseTime;

//     slider(int x, int time, int releaseTime)
//     :x(x), time(time), releaseTime(releaseTime){}
// };

//return a map with all the hitobjects in a beatmap
 map<int, vector<note>> getNotes(string map_path, string output_path, vector<slider>& sliders){
    map<int, vector<note>> mapNotes;
    vector <note> currentLine;
    int last_time = -1;
    bool hitObjectsSection = false;

    
    ifstream osu_map(map_path);
    if (!osu_map.is_open()){
        cout << "failed to open map" << endl;
        return mapNotes;
    }

    ofstream out_map(output_path);
 

    int current_line = 0;
    string line;

    while (getline(osu_map, line)){
        if (line.substr(0,7) == "Version"){
            out_map << "Version:MMM Map" << endl;
        }
        else {
            out_map << line << endl;
        }
        


        if (line == "[HitObjects]"){
            hitObjectsSection = true;
            cout << "Found hitobjects section" << endl;
            while (getline(osu_map,line)){
                
                //cout << line << endl;
                stringstream divided_note(line);
                vector <int> note_values;
                string svalue;
                
                while (getline(divided_note, svalue, ',')){
                    int value = stoi(svalue);
                    note_values.push_back(value);
                }
                if (note_values[2] != last_time && last_time != -1){
                    mapNotes[last_time] = currentLine;
                    currentLine.clear();
                }
                currentLine.push_back(note(note_values[0], note_values[1], note_values[2], note_values[3], note_values[4], note_values[5]));
                if (note_values[5] > 0){
                    sliders.push_back(slider(note_values[0], note_values[2], note_values[5]));}
                last_time = note_values[2];
                
                 
            }
            break;

            
        } 
        
        
    }
    //in case it can't find the hitobjects section of the file
    if(hitObjectsSection == false){
            cout << "couldn't find the HitObjects section" << endl;
            return mapNotes;
        }

    //adds the notes of the last line of the file
    if (last_time != -1) {
        mapNotes[last_time] = currentLine;
    }

    

    osu_map.close();

    return mapNotes;
 }

//finds the amount of notes in a given a chord (why did i create this)
int getChordCount(const vector<note>& chord){
    int count = chord.size();
    return count;
}


//returns the pointer of the note at a given time and column
note* getNoteAtTimeX(map<int, vector<note>>& mapNotes, int& time, int& x) {
    note* note = nullptr;
    for (auto& entry : mapNotes) {
        if (entry.first == time) {
            for (int i = 0; i < entry.second.size(); i++) {
                if (entry.second[i].x == x) {
                    note = &entry.second[i];
                    return note;
                }
            }
        }
    }
    return note;
}

//Returns the surrouding notes of a given note
vector <note*> getSurroundingNotes(map<int, vector<note>>& mapNotes, int& time, int& x){
    int prev_timing;
    int next_timing;
    vector <note*> surroundingNotes = {nullptr, nullptr};

    auto timing_key = mapNotes.find(time);


    if (timing_key == mapNotes.end()) {
        return surroundingNotes; // Tempo não encontrado
    }

    //Timing anterior
    if(timing_key != mapNotes.begin()){
    prev_timing = prev(timing_key)->first;
    surroundingNotes[0] = getNoteAtTimeX(mapNotes, prev_timing, x);
    
    

    //Timing posterior
    if (timing_key != mapNotes.end()){
    next_timing = next(timing_key)->first;
    surroundingNotes[1] = getNoteAtTimeX(mapNotes, next_timing, x);
    
    }

    return surroundingNotes;
    }
    //return surroundingNotes;
}
//atributes the surrouding notes of a given note
void atributeSurroundingNotes(map<int, vector<note>>& mapNotes, note& currentNote){
    vector<note*> surroundingNotes = getSurroundingNotes(mapNotes, currentNote.time, currentNote.x);
    currentNote.prevNote = surroundingNotes[0];
    currentNote.nextNote = surroundingNotes[1];
    //Temporary way of doing so, will be changed for a time based approach
            if(currentNote.nextNote != nullptr){
                    currentNote.isJack = true;
            }
}
//atributes the surrouding notes for every note in the beatmap
void atributeMapSurroundingNotes(map<int, vector<note>>& mapNotes){
    vector<note*> surroundingNotes;
    for (auto& entry : mapNotes){
        for (int i = 0; i < entry.second.size(); i++){
            note& currentNote = entry.second[i];
            atributeSurroundingNotes(mapNotes, currentNote);

            //Temporary way of doing so, will be changed for a time based approach
            if(currentNote.nextNote != nullptr){
                    currentNote.isJack = true;
            }


        }

    }
}


//returns the number of notes in the beatmap
int getNoteCount(map<int, vector<note>>& mapNotes){
    int noteCount = 0;
    for (auto& entry : mapNotes){
        noteCount += entry.second.size();
    }
    return noteCount;
}


//currently considering the previous and next note of the same x position
int getJackCount(map<int, vector<note>>& mapNotes){
    int jackCount = 0;
    for (auto& entry : mapNotes){
        for (int i = 0; i < entry.second.size(); i++){
            if (entry.second[i].isJack == true){
                jackCount++;
            }
        }
    }

    return jackCount;
}

//returns the number of slider in the map
int getSliderCount(map<int, vector<note>>& mapNotes){
    int count = 0;
    for (auto& entry : mapNotes){
        vector<note> chord = entry.second;
        for (auto& note : chord){
            if (note.releaseTime != 0){
                count++;
            }
        }
    }
    return count;
}



//erases a random note from a chord
void randomEraseNote(vector<note>& chord){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, chord.size() - 1);
    int randomIndex = dis(gen);
    chord.erase(chord.begin() + randomIndex);
}
    
//outPut the map notes into the beatmap file
void outPutMapNotes(map<int, vector<note>>& mapNotes, string output_path){
    ofstream out_map(output_path, ios::app);
    for (auto& entry : mapNotes){
        for (int i = 0; i < entry.second.size(); i++){
            note currentNote = entry.second[i];
            if(currentNote.releaseTime != 0){
            out_map << currentNote.x << "," << currentNote.y << "," << currentNote.time << "," << currentNote.type << "," << currentNote.idk << "," << currentNote.releaseTime <<":0:0:0:0:"<< endl;
            } 
            else {
            out_map << currentNote.x << "," << currentNote.y << "," << currentNote.time << "," << currentNote.type << "," << currentNote.idk << "," << "0:0:0:0:"<< endl;
            }
        }
    }
    out_map.close();
}
    
 //Return the columns of a beatmap  
vector<int> getMapColumns(map<int, vector<note>>& mapNotes) {
    vector<int> columns;

    for (auto& entry : mapNotes) {
        for (const auto& currentNote : entry.second) {
            if (find(columns.begin(), columns.end(), currentNote.x) == columns.end()) {
                columns.push_back(currentNote.x);
            }
        }
    }

    sort(columns.begin(), columns.end());
    return columns;
}


//Verify the unused columns of a specific chord/timing
vector <int> getFreeColumns(int time, map<int, vector<note>>& mapNotes){
    
    vector <int> freeColumns;
    vector <int> mapColumns = getMapColumns(mapNotes);

    auto timing = mapNotes.find(time);
    if (timing  == mapNotes.end()){
        cout << "Couldn't find given time" << endl;
        return freeColumns;
 
    }
    vector <note> chord = timing->second;
    vector <int> usedColumns;


    //getting the used columns of the chord;
    for (int i = 0; i < chord.size(); i++){
        usedColumns.push_back(chord[i].x);
    }

    //comparing the usedColumns with the columns of the map
    for (int i = 0; i < mapColumns.size(); i++){
        bool isFree = true;

        for (int j = 0; j < usedColumns.size(); j++){
            if (mapColumns[i] == usedColumns[j]){
                isFree = false;
                break;
            }

        }    
        if (isFree == true){
            freeColumns.push_back(mapColumns[i]);
        }
        
    }    
    return freeColumns;
}

int getRandomColumn(vector<int>& freeColumns){
    if (freeColumns.empty()){
        cout << "Warning getRandomColumn vector is empty" << endl;
        return 1;
    }
    if (freeColumns.size() > 0){
    random_device rd;
    mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<> dis(0, freeColumns.size() - 1);
    int randomIndex = dis(gen);
    return freeColumns[randomIndex];
    }
    return 1;
}


vector<int> getJackFreeColumns(int& time, map<int, vector<note>>& mapNotes){
    vector<int> freeColumns;
    vector<int> chordColumns = getFreeColumns(time, mapNotes);

    if (chordColumns.empty()){
        return freeColumns;
    }
    vector<note> chord = mapNotes.find(time)->second;
    auto timing = mapNotes.find(time);

    //Verify if it is the last timing of the map before proceeding
    //to avoid trespassing the limits of mapNotes
    if (timing == mapNotes.begin() || timing == prev(mapNotes.end())){
        //cout << "couldn't proceed with FreeJack columns" << endl;
        return freeColumns;
    }
    auto prevTiming = prev(timing);
    auto nextTiming = next(timing);
    vector<note>& prevChord = prevTiming->second;
    vector<note>& nextChord = nextTiming->second;

    for (int i = 0; i < chordColumns.size(); i++) {
    bool isFree = true;

    // Usando set para melhorar a busca
    unordered_set<int> occupiedColumns;

    // Adicionando as colunas ocupadas pelos acordes anterior e posterior
    for ( auto& note : prevChord) {
        occupiedColumns.insert(note.x);
    }
    for ( auto& note : nextChord) {
        occupiedColumns.insert(note.x);
    }

    // Verificando se a coluna atual está ocupada
    if (occupiedColumns.find(chordColumns[i]) != occupiedColumns.end()) {
        isFree = false;
    }

    if (isFree) {
        freeColumns.push_back(chordColumns[i]);
    }
}

    return freeColumns;
}
void getSliderFreeColumns(vector<int>& freeColumns, vector<slider>& sliders, int time){
    for (auto& slider : sliders){
        for (int& column : freeColumns){
            if (slider.x == column){
                if (slider.time <= time && slider.releaseTime >= time){
                    freeColumns.erase(remove(freeColumns.begin(),freeColumns.end(),column), freeColumns.end());
                }
            }
        }
    }

}

vector<int> getChordReleaseTimes(map<int,vector<note>>& mapNotes, int time){
    auto timing = mapNotes.find(time);
    vector<note>& chord = timing->second;
    vector<int> releaseTimes;
    for (note& it : chord){
        releaseTimes.push_back(it.releaseTime);
    }
    return releaseTimes;
}


int getRandomReleaseTime(vector<int>& releaseTimes){
    int releaseTime = getRandomColumn(releaseTimes);
    return releaseTime;
}

//verify if a given time has a slider in it (returns true if a slider is found)
bool verifyTimeXSlider(int column, int time, vector<slider>& sliders){
    bool hasSlider = false;
    for (const auto& slider : sliders){
        if (slider.time <= time && slider.releaseTime >= time && slider.x == column){
            hasSlider = true;
        }
    }
    return hasSlider;
}


bool verifyReleaseTimeIsFree(int column, int releaseTime, map<int, vector<note>>& mapNotes, vector<slider>& sliders){
    bool isFree = true;
    auto timing = mapNotes.find(releaseTime);
    vector<note> chord = timing->second;
    if (verifyTimeXSlider(column, releaseTime, sliders)){
        isFree = false;
    }
    for (auto& note : chord){
        if (note.x == column){
            isFree = false;
        }
    }

    return isFree;
}


bool verifySliderDurationFree(int column, int time, int releaseTime, map<int, vector<note>>& mapNotes, vector<slider>& sliders) {
    auto timing = mapNotes.find(time);
    if (timing == mapNotes.end()) return true; // Tempo inicial não existe no mapa

    while (time <= releaseTime) {
        const vector<note>& chord = timing->second;

        // Verificar se há algo na coluna
        for (const auto& note : chord) {
            if (note.x == column) return false;
        }

        // Verificar sliders
        if (verifyTimeXSlider(column, time, sliders)) {
            return false;
        }

        // Avançar para o próximo timing
        timing = next(timing);
        if (timing == mapNotes.end()) break; // Parar se chegar ao fim
        time = timing->first; // Atualizar tempo com o próximo timing
    }

    return true; // Está livre
}


void eraseNotesFromChord(map<int, vector<note>>& mapNotes, bool keepTrills, bool tillEspecific, int spcChord){
    for (auto& entry : mapNotes){
        auto timing = mapNotes.find(entry.first);
        vector<note>& chord = entry.second;
        vector<note>& nextChord = next(timing)->second; 
        if (getChordCount(chord) == getChordCount(nextChord) && keepTrills){
        } else if (tillEspecific){
            while(getChordCount(chord) > spcChord){
                randomEraseNote(chord);
            } 
        } 
        else if (getChordCount(chord) > 1){
            
            randomEraseNote(chord);
        
        }
    }
}



void buffMap(map<int, vector<note>>& mapNotes, vector<slider>& sliders, int baseChordSize, bool avoidJacks){
    for (auto& entry : mapNotes){
        
        vector<note>& chord = entry.second;
        int chordTime = entry.first;
        int randomColumn;
        int randomReleaseTime;
        vector<int> releaseTimes = getChordReleaseTimes(mapNotes, chordTime);
        vector<int> freeColumns = getFreeColumns(chordTime, mapNotes);
        if (chord.size() > baseChordSize && chord.size() < getMapColumns(mapNotes).size() && !freeColumns.empty()){

            if(avoidJacks){
                freeColumns = getJackFreeColumns(chordTime, mapNotes);
                if (freeColumns.empty()){
                    continue;
                }
                }


            getSliderFreeColumns(freeColumns, sliders, chordTime);
            if (freeColumns.empty()){
                continue;}

            randomColumn = getRandomColumn(freeColumns);
            randomReleaseTime = getRandomReleaseTime(releaseTimes);
            note newNote = note(randomColumn, 192, chordTime, 1, 0, randomReleaseTime);

            //If the release time of the new note is different from 0 it verifies if the column of
            //the note is free alongside it's duration to avoid overlapping notes
            if (newNote.releaseTime != 0){
                newNote.type = 128;
                if(verifySliderDurationFree(randomColumn, newNote.time, newNote.releaseTime, mapNotes, sliders)){
                    sliders.push_back(slider(randomColumn, chordTime, randomReleaseTime));
                }
                else{
                    while (!verifySliderDurationFree(randomColumn, newNote.time, newNote.releaseTime, mapNotes, sliders)){
                        if (freeColumns.empty()){
                            freeColumns = getJackFreeColumns(newNote.time, mapNotes);
                            getSliderFreeColumns(freeColumns, sliders, newNote.time);
                            if (!freeColumns.empty()){
                                newNote.x = getRandomColumn(freeColumns);
                                newNote.releaseTime = 0;
                                newNote.type = 0;
                                break;
                            } 
                            break;
                            }
                        freeColumns.erase(std::remove(freeColumns.begin(), freeColumns.end(), randomColumn), freeColumns.end());
                        if (!freeColumns.empty()){
                            randomColumn = getRandomColumn(freeColumns);
                            newNote.x = randomColumn;}
                    }
                    sliders.push_back(slider(randomColumn, chordTime, randomReleaseTime));
                }
                }



            newNote.createdNote = true;
            chord.push_back(newNote);
            note& newNoteRef = chord.back();
            atributeSurroundingNotes(mapNotes, newNoteRef);
        }
    }
        
} 
    

// Helper function to log debugging information
void logDebug(const string& message) {
    cout << "[DEBUG] " << message << endl;
}



void printInfo(map<int, vector<note>>& mapNotes, map<int, vector<note>>& modified, bool printNotes, bool printJackTimings){
    int timingCount = 0;
    int noteCount = 0;
    int randomNumber = 6590;
    note* testNote = &mapNotes.find(randomNumber)->second[0];
    
        for (const auto& entry : mapNotes){
        vector<note> notes = entry.second;
        if (printNotes){
            cout << "Time: " << notes[0].time << " || Chord: " << getChordCount(notes) << endl;
            }
        if (printJackTimings){
            for (int i = 0; i < notes.size(); i++){
                if (notes[i].isJack){
                    cout << "Jack at time: " << notes[i].time <<" || Position: "<< notes[i].x << endl;
                }
            }
        }
        timingCount++;
        noteCount += getChordCount(notes);
        }
    
    cout <<"================================================\n"
     << "Timing count: " << timingCount << endl
     <<"\nNote count: " << getNoteCount(mapNotes) <<" --> " << getNoteCount(modified)<< endl
     <<"First note time: " << mapNotes.begin()->first << endl
     <<"Last note time: " << mapNotes.rbegin()->first << endl
     <<"Total play time: " << mapNotes.rbegin()->first - mapNotes.begin()->first << endl
     <<"Jack notes count: " << getJackCount(mapNotes) << " --> " << getJackCount(modified) << endl
     <<"Slider count:" << getSliderCount(mapNotes) << " --> " << getSliderCount(modified) << endl ; 
    
    

    cout << "\nMap columns:"<< endl;
        vector<int> columns = getMapColumns(mapNotes);
        for (int i = 0; i < columns.size(); i++){
            cout << columns[i] << " ";
        }

}   


void printHelp(){
    cout << "Available commands:" << endl
    << "info: prints the map info" << endl
    << "buff: buffs the map" << endl
    << "nerf: nerfs the map" << endl
    << "exit: closes the application" << endl;

}

void normalizeNotes(map<int, vector<note>>& mapNotes){
    for (auto& entry : mapNotes){
        vector<note>* chord = &entry.second;
        for (int i = 0; i < chord->size(); i++){
            if
        }
    }
}