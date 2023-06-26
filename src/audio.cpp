//
//  audio.cpp
//  TJE_XCODE
//
//  Created by manu on 26/6/23.
//  Copyright © 2023 manu. All rights reserved.
//

/*#include <stdio.h>
#include "audio.h"
#include <string>

std::map<std::string, Audio*> Audio::sAudiosLoaded;

Audio::Audio()
{
    sample = 0;
}

Audio::~Audio()
{
    if (sample != 0) {
        BASS_SampleFree(sample);
        sample = 0;
    }
}

HCHANNEL Audio::play(float volume)
{
    HCHANNEL channel = BASS_SampleGetChannel(sample, FALSE);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
    BASS_ChannelPlay(channel, TRUE);
    return channel;
}

bool Audio::Init()
{

    return BASS_Init(-1, 44100, 0, NULL, NULL);
}

Audio* Audio::Get(const char* filename)
{
    std::string key(filename);
    if (sAudiosLoaded.find(key) != sAudiosLoaded.end()) {
        return sAudiosLoaded[key];
    }

    HCHANNEL hSampleChannel; // Handler to store one channel

    Audio* audio = new Audio();

    // Load sample from disk
    // flags: BASS_SAMPLE_LOOP, BASS_SAMPLE_3D, ...
    audio->sample = BASS_SampleLoad(
        false,              // From internal memory
        filename,     // Filepath
        0,                // Offset
        0,                // Length
        3,                // Max playbacks
        0                 // Flags
    );

    if (audio->sample == 0) // Error loading
        return nullptr;

    sAudiosLoaded[key] = audio;

    return audio;
}

HCHANNEL Audio::Play(const char* filename)
{
    Audio* audio = Get(filename);
    printf("getting audio");
    if (audio != NULL) {
        return audio->play(1.0f);
    }
    return 0;
}

bool Audio::Stop(HCHANNEL channel)
{
    return BASS_ChannelStop(channel);
}
*/
