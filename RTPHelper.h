//
// Created by 冉高飞 on 2018/8/24.
//

#ifndef OPUSDEMO_RTPHELPER_H
#define OPUSDEMO_RTPHELPER_H

#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtpudpv4transmitter.h>

#define OPUS 101

#define RTP_PACKET_MAX_LENGTH 1400
using namespace jrtplib;

class RTPHelper {

public:
    RTPHelper(uint8_t hostIP[], uint16_t port);

public:

    int initRTPSession();

    int sendRTPPacket(unsigned char *data, size_t length);

    void destroyRTPSession();

private:
    void checkError(int errCode);

    int status;
    RTPSession rtpSession;
    RTPSessionParams sessionParams;
    RTPUDPv4TransmissionParams transmissionParams;
    char sendBuf[RTP_PACKET_MAX_LENGTH + 3];
};


#endif //OPUSDEMO_RTPHELPER_H
