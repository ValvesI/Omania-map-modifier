#include <vector>
#include "beatmap.hpp"
#include "notes.hpp"
#include <string>
#include <fstream>
#include <iostream>


void BeatMap::load(const std::string& filepath){
    std::cout <<"AAAA";
    std::ifstream file(filepath);
    notes = parseNotes(filepath);
}

std::vector<Note> BeatMap::parseNotes(const std::string& filePath){
    std::ifstream file(filePath);
    std::vector<Note> notes;

    if (!file.is_open()) {
        std::cout << "ERRO AO ABRIR O MAPA" << std::endl;
        // opcional: lançar exceção, logar erro, etc.
        return notes;
    }
    std::cout <<"Mapa abrido " << std::endl;
    // Supondo linha no formato: time x type
    int time, x;
    int type;

    while (file >> time >> x >> type) {
        Note note;
        note.time = time;
        std::cout <<"X: " << x;
        note.x = x;
        note.updateColumn();
        note.map = this;
        notes.push_back(note);
    }

    return notes;
}


std::vector<Note> BeatMap::normalize(){
    for (auto &note : this->notes){
        note.column = getFreeColumn(note.time);
        note.updateColumn();
    }
    return notes;

    for (auto& note : notes){
        std::cout << "x: " << note.column << std::endl;
    }




}

int BeatMap::getFreeColumn(int time) {
    auto chord = getChord(time);

    uint32_t mask = 0;

    for (auto* note : chord) {
        mask |= (1 << note->column);
    }

    for (int col : columns) {
        if (!(mask & (1 << col)))
            return col;
    }

    return -1;
}

std::vector<Note*> BeatMap::getChord(int time){
    std::vector<Note*> chord;
    for(auto& note : notes){
        if (note.time == time)
            chord.push_back(&note);
    }

    return chord;
}