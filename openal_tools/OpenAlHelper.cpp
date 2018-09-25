
#include "OpenAlHelper.h"
#include <stdio.h>
#include <zconf.h>

using namespace std;


void OpenAlHelper::showAllDevice() {
    if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_TRUE) {
        devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
        defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        cout << "Devices:" << devices
             << "\nDefault Device:" << defaultDevice << endl;
    }

}

void OpenAlHelper::openDevice() {

    decSaka = opus_decoder_create(48000, 1, &error);
    {

    }
    auto *wavTools = new WavTools();
    ifstream in_stream;
    in_stream.open("../test.wav", ios::binary);
    fmtChunk = static_cast<FmtChunk *>(malloc(sizeof(FmtChunk)));
    wavTools->getWavFormat(&in_stream, fmtChunk, true);
    wavTools->seekToRealData(&in_stream);
    device = alcOpenDevice(nullptr);
    if (!device) {
        return;
    }
    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context)) {
        cout << "make context error !!" << endl;
        return;
    }
    alGetError();
    alGenBuffers(BUFFER_NUM, g_Buffers);
    if (checkError("alGenBuffers")) {
        return;
    }

    alGenSources(1, &source);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    if (checkError("AL_LOOPING")) {
        return;
    }
//    alSourcef(source, AL_SOURCE_TYPE, AL_STREAMING);
//    if (checkError("AL_SOURCE_TYPE")) {
//        return;
//    }
//    alSourceQueueBuffers(source, BUFFER_NUM, g_Buffers);
    if (checkError("alSourceQueueBuffers")) {
        return;
    }
    alGetSourcei(source, AL_BUFFERS_QUEUED, &bufferSize);
    cout << "======= buffer size" << bufferSize << endl;
    alSourcePlay(source);
    alGetSourcei(source, AL_SOURCE_STATE, &bufferSize);
//    while (readLength == AL_PLAYING) {
//        alGetSourcei(source, AL_SOURCE_STATE, &readLength);
//    }

}

OpenAlHelper::OpenAlHelper() {}

void OpenAlHelper::play(SyncQueue<OpusBody *> *queue) {
    bufferSize = 0;
    alGetSourcei(source, AL_BUFFERS_QUEUED, &bufferSize);
    if (bufferSize == 0) {
        for (unsigned int g_Buffer : g_Buffers) {
            OpusBody *opusBody;
            cout << "开始" << endl;
            queue->take(opusBody);
            int frameSize = opus_decode(decSaka, opusBody->memory, opusBody->size, pcmData, 4000, 0);
            fillBuffer(g_Buffer, reinterpret_cast<uint8_t *>(pcmData), frameSize * 2);

            delete opusBody;
        }
    }
    alSourceQueueBuffers(source, BUFFER_NUM, g_Buffers);
    alGetSourcei(source, AL_SOURCE_STATE, &status);
    if (status != AL_PLAYING) {
        cout << "start play===" << endl;
        alSourcePlay(source);
    }
    bufferSize = 0;
    while (bufferSize == 0) {
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &bufferSize);
        if (bufferSize > 0) {
            alSourceUnqueueBuffers(source, bufferSize, g_Buffers);
            for (int i = 0; i < bufferSize; i++) {
                OpusBody *opusBody;
                queue->take(opusBody);
                int frameSize = opus_decode(decSaka, opusBody->memory, opusBody->size, pcmData, 4000, 0);
                fillBuffer(g_Buffers[i], reinterpret_cast<uint8_t *>(pcmData), frameSize * 2);
                alSourceQueueBuffers(source, bufferSize, g_Buffers);
                cout << "readLength" << bufferSize << ",size====" << opusBody->size << ",frameSize=" << frameSize
                     << endl;
                delete opusBody;
            }
            bufferSize = 0;
        }
        alGetSourcei(source, AL_SOURCE_STATE, &status);
        if (status != AL_PLAYING) {
            cout << "start play===" << endl;
            alSourcePlay(source);
        }
    }

}

void OpenAlHelper::fillBuffer(ALuint index, uint8_t *buf, uint32_t len) {
    if (fmtChunk->num_channels == 1) {
        if (fmtChunk->bit_per_sample == 8)
            alBufferData(index, AL_FORMAT_MONO8, buf, len, fmtChunk->sample_rate);
        else if (fmtChunk->bit_per_sample == 16)
            alBufferData(index, AL_FORMAT_MONO16, buf, len, fmtChunk->sample_rate);
    } else if (fmtChunk->num_channels == 2) {
        if (fmtChunk->bit_per_sample == 8)
            alBufferData(index, AL_FORMAT_STEREO8, buf, len, fmtChunk->sample_rate);
        else if (fmtChunk->bit_per_sample == 16)
            alBufferData(index, AL_FORMAT_STEREO16, buf, len, fmtChunk->sample_rate);
    }


}

int OpenAlHelper::checkError(const char *msg) {
    error = alGetError();
    switch (error) {
        case AL_NO_ERROR:
            cout << msg << ":no error" << endl;
            break;
        case AL_INVALID_NAME:
            cout << msg << ":a bad name (ID) was passed to an OpenAL function" << endl;
            break;
        case AL_INVALID_ENUM:
            cout << msg << ":an invalid value was passed to an OpenAL function" << endl;
            break;
        case AL_INVALID_VALUE:
            cout << msg << ":the requested operation is not valid" << endl;
            break;
        case AL_INVALID_OPERATION:
            cout << msg << ":the requested operation resulted in OpenAL running out of memory" << endl;
            break;
        case AL_OUT_OF_MEMORY:
            cout << msg << ":an invalid enum value was passed to an OpenAL function" << endl;
            break;
        default:
            cout << msg << ":unknown" << endl;
            break;
    }
    return error;
}
