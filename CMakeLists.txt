cmake_minimum_required(VERSION 3.9)
project(OpusDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(OpusDemo encoder.cpp RTPHelper.cpp ./wav_tools/WavTools.cpp)

add_executable(OpusDecoder decoder.cpp)

target_link_libraries(OpusDemo opus jrtp)

target_link_libraries(OpusDecoder opus jrtp)