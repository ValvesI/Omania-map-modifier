#include <iostream>
#include "beatmap.hpp"


int main (){
    BeatMap teste;
    teste.load("teste.osu");
    teste.normalize();
}