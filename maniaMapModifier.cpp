#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <random>
#include "functions.h"

int main (int argc, char* argv[]){
    if (argc < 1){exit(0);}
    string file_path = argv[1];
    string out_path = "out-map.osu";
    vector<slider> sliders;
    map<int, vector<note>> mapNotes = getNotes(file_path, out_path, sliders);
    atributeMapSurroundingNotes(mapNotes);
    map<int, vector<note>> mapNotesBackup = mapNotes;
    printHelp();
    // //eraseNotesFromChord(mapNotes, false, false);
    // buffMap(mapNotes, sliders, 1, true);
    // outPutMapNotes(mapNotes, out_path);
    // //printInfo(bools: printNotes, printJackTimings)
    // printInfo(mapNotesBackup, mapNotes, false, false);

    string input;
    while (true){
        getline(cin, input);
        if (input == "exit"){
            exit(0);
        }
    
    
        if (input == "info"){
            printInfo(mapNotesBackup, mapNotes, false, false);
        }
    
        if (input == "buff"){
            cout << "Buffing map..." << endl;
            buffMap(mapNotes, sliders, 1, true);
            outPutMapNotes(mapNotes, out_path);
            cout << "Map buffing complete" << endl;
        }
    
        if (input == "nerf"){
            cout << "nerfing map..." << endl;
            eraseNotesFromChord(mapNotes, false, false);
            outPutMapNotes(mapNotes, out_path);
            cout << "Map nerfing complete" << endl;
        }
        
        if (input == "clear"){
            system("cls");
            printHelp();
        }

        if (input == "output"){
            outPutMapNotes(mapNotes, out_path);
        }

    }
    
    

    return 0;
}
