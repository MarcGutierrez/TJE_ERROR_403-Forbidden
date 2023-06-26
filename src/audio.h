//
//  audio.h
//  TJE_XCODE
//
//  Created by manu on 24/5/23.
//  Copyright © 2023 manu. All rights reserved.
//

#pragma once

#include "extra/bass.h"
#include "framework.h"
#include <map>
#include <string>

class Audio  {
public:

// Handler for BASS_SampleLoad
HSAMPLE sample;

Audio();    // Sample = 0 here
~Audio();     // Free sample (BASS_SampleFree)

// Play audio and return the channel
HCHANNEL play(float volume);

    // Manager stuff
    // The map!
    static std::map<std::string, Audio*>
sAudiosLoaded;

    // Initialize BASS
static bool Init();
    // Get from manager map
static Audio* Get( const char* filename );
    // Play Manager API
static HCHANNEL Play( const char* filename );
static HCHANNEL Play3D( const char* filename,
Vector3 position); //se llama una vez solo, no en el update
    // Stop sounds
static bool Stop( HCHANNEL channel );
};


