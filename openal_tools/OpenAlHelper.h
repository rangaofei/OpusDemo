//
// Created by wxk5301 on 2018/9/14.
//

#ifndef OPUSDEMO_OPENALHELPER_H
#define OPUSDEMO_OPENALHELPER_H

#include <OpenAL/OpenAL.h>
#include "../wav_tools/WavTools.h"

class OpenAlHelper {

public:
    const ALsizei BUFFER_NUM = 1024 * 2;

    void showAllDevice();

    void openDevice();

    OpenAlHelper();

    void play(char *data,int length);

private:
    const ALchar *devices;
    const ALchar *defaultDevice;
    ALCdevice *device;
    ALCcontext *context;
    ALboolean g_bEAX;
    ALenum error;
    ALuint g_Buffers[1024 * 2];

    char pcmData[1024 * 1024 * 20];

    ALuint source;
    int data_length;
    int freq;

    void checkError();


};


#endif //OPUSDEMO_OPENALHELPER_H
