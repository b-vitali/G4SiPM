#include "G4ios.hh"
#include "G4String.hh"
#include "G4MyPrint.hh"

namespace G4MyPrint {
    int windows_size = 60;
    int buffer;
    bool odd;
    bool print = true;

    void start_print(G4String s) {
        if (!print) return;

        if (s.length() % 2) odd = true;
        else odd = false;

        if (odd) s = " " + s + " ... Start! ";
        else s = " " + s + " ... Start!! ";

        buffer = (windows_size - s.length()) / 2;
        G4cout << G4endl;
        G4cout << std::string(windows_size, '*') << G4endl;
        G4cout << std::string(buffer, '*') << s << std::string(buffer, '*') << G4endl;
    }

    void line_print(){
        G4cout<<std::string(windows_size, '*')<<G4endl;
    }

    void running_print(G4String s) {
        if( s.length() % 2 ) odd = true;
        else odd = false;

        if(odd) s = " "+ s;
        else s = " "+s + " ";

        buffer = ( windows_size-s.length() )/2;
	    G4cout<<"*"<<s<<std::string(2*buffer-2, ' ')<<"*"<<G4endl;
    }

    void finish_print(G4String s) {
        if (!print) return;

        if (s.length() % 2) odd = true;
        else odd = false;
        
        if(odd) s = " "+s + " ... Done!! ";
        else s = " "+s + " ... Done!!! ";

        buffer = (windows_size - s.length()) / 2;
        G4cout << std::string(buffer, '*') << s << std::string(buffer, '*') << G4endl;
        G4cout << std::string(windows_size, '*') << G4endl;
    }
}
