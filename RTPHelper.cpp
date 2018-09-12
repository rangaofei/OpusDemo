//
// Created by 冉高飞 on 2018/8/24.
//

#include <iostream>
#include "RTPHelper.h"
#include <jrtplib3/rtperrors.h>

using namespace std;

int RTPHelper::initRTPSession() {
    return 0;
}

void RTPHelper::checkError(int errCode) {
    if (errCode < 0) {
        cout << "error " << RTPGetErrorString(errCode) << endl;
    }
}

int RTPHelper::sendRTPPacket(unsigned char *data, size_t length) {
    if (!data) {
        cerr << "数据为空" << endl;
        return -1;
    }
    if (length > RTP_PACKET_MAX_LENGTH) {
        cerr << "元数据长度大于MTU了" << endl;
        return -2;
    }

//    memset(sendBuf, 0, RTP_PACKET_MAX_LENGTH+3);
    memcpy(&sendBuf, data, length);
    status = rtpSession.SendPacket(sendBuf, length);
    checkError(status);
    return 0;
}

void RTPHelper::destroyRTPSession() {
    rtpSession.BYEDestroy(RTPTime(10, 0), 0, 0);

}

RTPHelper::RTPHelper(uint8_t *hostIP, uint16_t port = 8006) {
    sessionParams.SetOwnTimestampUnit(1.0 / 90000.0);
    sessionParams.SetAcceptOwnPackets(true);
    sessionParams.SetUsePredefinedSSRC(true);
    transmissionParams.SetPortbase(8004);
    status = rtpSession.Create(sessionParams, &transmissionParams);
    checkError(status);
    RTPIPv4Address addr(hostIP, port);
    status = rtpSession.AddDestination(addr);
    checkError(status);
    rtpSession.SetDefaultMark(true);
    rtpSession.SetDefaultPayloadType(OPUS);
    rtpSession.SetTimestampUnit(1.0 / 90000.0);
    rtpSession.SetDefaultTimestampIncrement(3600);
    memset(sendBuf, 0, RTP_PACKET_MAX_LENGTH + 3);
}

