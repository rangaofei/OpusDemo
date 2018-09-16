cmake_minimum_required(VERSION 3.9)
project(OpusDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(OpusDemo encoder.cpp RTPHelper.cpp ./wav_tools/WavTools.cpp)

add_executable(OpusDecoder decoder.cpp openal_tools/OpenAlHelper.cpp openal_tools/OpenAlHelper.h wav_tools/WavTools.cpp)

find_package(OpenAL)

if (${OPENAL_FOUND})
    message(STATUS find OpenAL)
else ()
    message(STATUS not found)
endif ()

target_link_libraries(OpusDemo opus jrtp)

target_link_libraries(OpusDecoder opus jrtp ${OPENAL_LIBRARY})