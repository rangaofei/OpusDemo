//
// Created by 冉高飞 on 2018/5/4.
//
/**
 * 在主线程启动接收，接收到后直接填充buffer
 */
#include <opus/opus.h>
#include <iostream>
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <fstream>
#include <thread>
#include <zconf.h>
#include "openal_tools/OpenAlHelper.h"
#include "openal_tools/SyncQueue.h"

using namespace jrtplib;

using namespace std;

opus_int16 pcmData[4000];
RTPSession session;
OpenAlHelper *openAlHelper;

int status;

int err;

bool running = true;

SyncQueue<OpusBody *> opusPacket(200);


void checkError(int status) {
    if (status < 0) {
        cout << RTPGetErrorString(status) << endl;
    }
}

void Consume() {

    openAlHelper->play(&opusPacket);
//    while (running) {
//        OpusBody *opusBody;
//        opusPacket.take(opusBody);
//        cout << "获取到opus数据" << opusBody->size << endl;
//
//
//        int frameSize = opus_decode(decSaka, opusBody->memory, opusBody->size, pcmData, 4000, 0);
//        cout << "decode data is " << frameSize << endl;
//        fstream outfile("../data.wav", ios::app | ios::binary);
//        if (!outfile) {
//            cout << "创建文件错误" << endl;
//        }
//        outfile.write(reinterpret_cast<const char *>(pcmData), frameSize * 2);
//        outfile.close();
//        openAlHelper->play(reinterpret_cast<char *>(pcmData), frameSize * 2);
//    }
}

void Produce() {
    while (running) {
        session.BeginDataAccess();
        if (session.GotoFirstSourceWithData()) {
            do {
                RTPPacket *rtpPacket;
                while ((rtpPacket = session.GetNextPacket()) != nullptr) {
                    cout << "Get packet:" << rtpPacket->GetPayloadLength() << endl;
                    auto *opusBody = (OpusBody *) malloc(sizeof(OpusBody));
                    opusBody->size = rtpPacket->GetPayloadLength();
                    if (opusBody->memory == nullptr) {
                        cout << "alloc mem error" << endl;
                        break;
                    }
                    memcpy(opusBody->memory, rtpPacket->GetPayloadData(), opusBody->size);
                    opusPacket.put(opusBody);
                }
            } while (session.GotoNextSourceWithData());
        }

        session.EndDataAccess();
#ifndef RTP_SUPPORT_THREAD
        status = session.Poll();
        checkError(status);
#endif // RTP_SUPPORT_THREAD
    }
}

int main() {

    ifstream in_stream;
    in_stream.open("../test.wav", ios::binary);

    char header[44];
    fstream outfile("../data.wav", ios::app | ios::binary);
    if (!outfile) {
        cout << "创建文件错误" << endl;
    }
    in_stream.seekg(0, std::ios::beg);
    in_stream.read(header, 44);
    outfile.write(header, 44);
    in_stream.close();
    outfile.close();


    RTPUDPv4TransmissionParams transmissionParams;
    RTPSessionParams sessionParams;

    sessionParams.SetOwnTimestampUnit(1 / 9000.0);
    sessionParams.SetAcceptOwnPackets(true);
    transmissionParams.SetPortbase(8006);

    status = session.Create(sessionParams, &transmissionParams);

    cout << "启动成功" << endl;
    checkError(status);




    openAlHelper = new OpenAlHelper();
    openAlHelper->showAllDevice();
    openAlHelper->openDevice();
    std::thread consumer(Consume);
    std::thread produce(Produce);
//    consumer.join();
    produce.join();
}


