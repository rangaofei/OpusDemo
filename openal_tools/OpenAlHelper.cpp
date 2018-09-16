//
// Created by wxk5301 on 2018/9/14.
//

#include "OpenAlHelper.h"

#include <iostream>
#include <fstream>

#include "../wav_tools/WavTools.h"

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
    device = alcOpenDevice(nullptr);
    if (!device) {
        return;
    }
    cout << "init success" << endl;
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);

    //是否支持EAX2.0
    g_bEAX = alIsExtensionPresent("EAX2.0");

    alGetError();
    alGenBuffers(1, g_Buffers);
    if ((error = alGetError()) != AL_NO_ERROR) {
        cout << "alGenBuffers :" << error << endl;
        return;
    }
//    alBufferData(g_Buffers[0], AL_FORMAT_MONO16, pcmData, BUFFER_NUM, 48000);
    alGenSources(1, &source);
//    if ((error = alGetError()) != AL_NO_ERROR) {
//        cout << "alGenSources 1 : " << error << endl;
//        return;
//    }

//
//    if ((error = alGetError()) != AL_NO_ERROR) {
//        cout << "alSourcei AL_BUFFER 0 : " << error << endl;
//    }
//    alSourcePlay(source);

}

void OpenAlHelper::checkError() {

}

OpenAlHelper::OpenAlHelper() {}

void OpenAlHelper::play(char *data, int length) {
    alBufferData(g_Buffers[0], AL_FORMAT_MONO16, data, length, 48000);
    alSourcei(source, AL_BUFFER, g_Buffers[0]);
    alSourcePlay(source);
    cout << "play" << endl;
}
