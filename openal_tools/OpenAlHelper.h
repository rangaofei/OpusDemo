//
// Created by wxk5301 on 2018/9/14.
//

#ifndef OPUSDEMO_OPENALHELPER_H
#define OPUSDEMO_OPENALHELPER_H

#include <OpenAL/OpenAL.h>
#include <opus/opus.h>
#include "../wav_tools/WavTools.h"
#include "SyncQueue.h"

class OpenAlHelper {

public:
    static const int BUFFER_NUM = 100;
    static const int BUFFER_SIZE = 960 * 4;

    void showAllDevice();

    void openDevice();

    OpenAlHelper();

    void play(SyncQueue<OpusBody *> *queue);

private:
    const ALchar *devices;
    const ALchar *defaultDevice;

    ALCdevice *device;
    ALCcontext *context;
    FmtChunk *fmtChunk;
    ALboolean g_bEAX;

    int bufferSize = 0;
    int status = -100;

    ALenum error;
    ALuint g_Buffers[BUFFER_NUM];

    opus_int16 pcmData[4000];

    uint8_t tmpData[BUFFER_SIZE];

    ALuint source;
    int data_length;
    int freq;

    OpusDecoder *decSaka;

    int checkError(const char *msg);

    void fillBuffer(ALuint index, uint8_t *buf, uint32_t len);


};


#endif //OPUSDEMO_OPENALHELPER_H
