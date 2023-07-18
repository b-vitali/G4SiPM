#ifndef G4MYPRINT_HH
#define G4MYPRINT_HH

#include "G4String.hh"

namespace G4MyPrint {
    extern int windows_size;
    extern int buffer;
    extern bool odd;
    extern bool print;

    void start_print(G4String s);
    void running_print(G4String s);
    void finish_print(G4String s);
}

#endif
