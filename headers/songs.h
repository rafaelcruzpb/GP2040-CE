#ifndef _SONGS_H_
#define _SONGS_H_

#include <vector>
#include <buzzerspeaker.h>

const vector<Song>songs = {
    {
        "DEFAULT_INTRO_SONG",
        100,
        {
            D5,AS5,B5,C6,CS6,D6,DS6,E6,
            C7,CS7,D7,DS7,E7,F7,D8,DS8,
        }
    },
    {
        "CONFIG_MODE_SONG",
        150,
        {
            E5,E5,G4,FS5,E5
        }
    },
};

#endif