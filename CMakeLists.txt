cmake_minimum_required(VERSION 3.9)
project(OpusDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(OpusDemo encoder.cpp decoder.cpp RTPHelper.cpp ./wav_tools/WavTools.cpp)
target_link_libraries(OpusDemo opus jrtp)