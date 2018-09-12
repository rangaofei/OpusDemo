#include <iostream>
#include <opus/opus.h>
#include <fstream>
#include "RTPHelper.h"
#include "wav_tools/WavTools.h"
#include <opus/opus.h>

using namespace std;
const int frame_size = 960;
const int channels = 1;

int main() {
    char header[44];
    auto *pcmData = new opus_int16[frame_size * channels];
    auto *pcmData_2 = new opus_int16[frame_size * channels + 2000];
    auto *opusData = new unsigned char[3000];
    int error;

    auto *wavTools = new WavTools();
    OpusEncoder *enc;
    OpusDecoder *dec;
    uint8_t host[] = {192, 168, 1, 5};
    RTPHelper rtpHelper(host, 8006);
    enc = opus_encoder_create(48000, channels, OPUS_APPLICATION_VOIP, &error);
    if (error != OPUS_OK) {
        printf("encoder init error");
        return -1;
    }

    dec = opus_decoder_create(48000, channels, &error);
    if (error != OPUS_OK) {
        printf("decoder init error");
        return -1;
    }
    ifstream in_stream;
    ofstream out_stream;

    in_stream.open("../test.wav", ios::binary);

//    out_stream.open("../e.wav", ios::binary);

    if (!in_stream.is_open()) {
        printf("open failed");
        return 0;
    }
    auto *fmtChunk = static_cast<FmtChunk *>(malloc(sizeof(FmtChunk)));
    wavTools->getWavFormat(&in_stream, fmtChunk);
    wavTools->seekToRealData(&in_stream);
//    return 0;
//    in_stream.seekg(0, ios::beg);
//    in_stream.read(header, 44);
//    out_stream.write(header, 44);

    while (in_stream.read(reinterpret_cast<char *>(pcmData), frame_size * channels * sizeof(opus_int16))) {
        int length = opus_encode(enc, pcmData, frame_size, opusData, 3000);
        if (length <= 0) {
            printf("error");
            break;
        }
        printf("encode length:%d\n", length);
        rtpHelper.sendRTPPacket(opusData, length);
//        int out_size = opus_decode(dec, opusData, length, pcmData_2, frame_size, 0);
//        printf("out length:%d\n", out_size);
//        out_stream.write(reinterpret_cast<const char *>(pcmData_2), out_size * channels * sizeof(opus_int16));
    }
    opus_encoder_destroy(enc);
    in_stream.close();
    out_stream.close();
    rtpHelper.destroyRTPSession();
    return 0;
}