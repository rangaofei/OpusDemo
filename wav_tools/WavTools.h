//
// Created by 冉高飞 on 2018/5/1.
//

#ifndef RESAMPLEDEMO_WAVTOOLS_H
#define RESAMPLEDEMO_WAVTOOLS_H

#include <iostream>
#include <fstream>

#define STATE_SUCCESS 0
#define STATE_ERR_NOT_WAV 1

const char RIFF[] = "RIFF";
const char WAVE[] = "WAVE";
const char FMT[] = "fmt ";
const char DATA[] = "data";

/**
 * 格式
 */
struct FmtChunk {
    char fmt[4];
    uint32_t chunk_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bit_per_sample;
};

struct DataChunk {
    char data[4];
    uint32_t chunk_size;
};
struct WavHeader {
    char riff[4];
    uint32_t chunk_size;
    char format[4];
    FmtChunk fmtChunk;
    DataChunk dataChunk;
};

class WavTools {
public:
    WavTools();

    ~WavTools();

    WavHeader *getInfoFromFIle(FILE *file);

    /**
     * 获取格式信息
     * @param in_stream 读取文件流
     * @param fmtChunk  文件格式结构体，将会被填满
     * @return 读取成功返回 #STATE_SUCCESS，读取失败返回#STATE_ERR_NOT_WAV
     */
    int getWavFormat(std::ifstream *in_stream, FmtChunk *fmtChunk, bool show);

    int writeWavInfoToFile(std::ofstream *outstream, WavHeader *header);

    int seekToRealData(std::ifstream *in_stream);

private:
    WavHeader *wavHeader;
    uint32_t chunk_size;

    /*
     * 输出格式信息
     */
    void printWAVInfo(FmtChunk *fmtChunk);
};


#endif //RESAMPLEDEMO_WAVTOOLS_H
