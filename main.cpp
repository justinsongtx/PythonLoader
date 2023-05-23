//
//  main.cpp
//  AudioToVideo
//
//  Created by justinsong on 2023/5/18.
//

#include <Python.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "AudioToVideoConverter.h"
#include "PythonLoader.h"

using namespace std;

int runTest();

PyObject* g_pInstance;

int main(int argc, const char* argv[]) {
    int bytesPreFrame = 1280;           // 每帧的audio数据大小，40ms 一帧
    int byteSize = bytesPreFrame * 25;  // 每秒的audio数据大小，1s 25帧，3200 bytes/s
    AudioToVideoConverter* converter = new AudioToVideoConverter(string("bookSample.txt"), byteSize);
    // converter->convertToVideo();
    converter->readTest();
    return 0;
}
