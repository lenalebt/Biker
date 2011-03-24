#include <iostream>
#include "src/Database/Parser/osmparser.hpp"
#include "src/Database/osmdatabase.hpp"
#include "src/Database/osmdatabasewriter.hpp"

using namespace std;

#define TESTPROGRAM
#ifdef TESTPROGRAM
    int main(int argc, char** argv)
    {
        cout << "OSM-Routing-Testprogramm" << endl;
        OSMInMemoryDatabase test;
        cout << "Parsen..." << endl;
        test.openDatabase("data/hagen-teil.osm");
        cout << "Parsen fertig!" << endl;
        
        cout << "Warte 5 Sekunden..." << endl;
        sleep(5);
        
        cout << "Datenbank schließen und Speicher freigeben..." << endl;
        test.closeDatabase();
        
        cout << "Warte 5 Sekunden..." << endl;
        sleep(5);
        
        return 0;
    }

#else

#endif
