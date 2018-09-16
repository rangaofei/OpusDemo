//
// Created by 冉高飞 on 2018/5/4.
//

#include <opus/opus.h>
#include <iostream>
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtppacket.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <fstream>
#include "openal_tools/OpenAlHelper.h"

using namespace jrtplib;

using namespace std;
int status;

int err;

void checkError(int status) {
    if (status < 0) {
        cout << RTPGetErrorString(status) << endl;
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
    opus_int16 pcmData[2000];
    RTPSession session;
    RTPUDPv4TransmissionParams transmissionParams;
    RTPSessionParams sessionParams;

    sessionParams.SetOwnTimestampUnit(1 / 9000.0);
    sessionParams.SetAcceptOwnPackets(true);
    transmissionParams.SetPortbase(8006);

    status = session.Create(sessionParams, &transmissionParams);

    cout << "启动成功" << endl;
    checkError(status);

    OpusDecoder *dec;
    dec = opus_decoder_create(48000, 1, &err);

    auto *openAlHelper = new OpenAlHelper();
    openAlHelper->showAllDevice();
    openAlHelper->openDevice();
    while (true) {
        session.BeginDataAccess();

        if (session.GotoFirstSourceWithData()) {
            do {
                RTPPacket *rtpPacket;
                while ((rtpPacket = session.GetNextPacket()) != nullptr) {
                    cout << "Get packet:" << rtpPacket->GetPayloadLength() << endl;
                    int frameSize =
                            opus_decode(dec,
                                        rtpPacket->GetPayloadData(),
                                        rtpPacket->GetPayloadLength(),
                                        pcmData, 2000, 0);
                    //处理数据
                    cout << "decode data is " << frameSize << endl;
                    openAlHelper->play(reinterpret_cast<char *>(pcmData), frameSize*2);
//                    fstream outfile("../data.wav", ios::app | ios::binary);
//                    if (!outfile) {
//                        cout << "创建文件错误" << endl;
//                    }
//                    outfile.write(reinterpret_cast<const char *>(pcmData), frameSize * 2);
//                    outfile.close();
                    session.DeletePacket(rtpPacket);
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


