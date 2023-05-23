// 2023.5.21 justinsong


#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "PythonLoader.h"
#include <cstring>
#include <fstream>
#include <iostream>

class AudioToVideoConverter {

public:

    AudioToVideoConverter(const std::string& audioFilePath, const size_t audioDataSize);

    ~AudioToVideoConverter();

    void convertToVideo();
    void readTest();

        private :

        std::string audioFilePath;   // 音频文件路径
    int audioFileDescriptor;         // 音频文件描述符
    char* audioFileData;             // 音频文件数据
    size_t audioFileSize;            // 音频文件大小
    size_t audioDataSize;            // 每次读取的音频数据片段大小
    int audioDataOffset;             // 当前读取音频数据的偏移量
    int audioDataTotalSize;          // 音频数据总大小
    int sharedMemoryFileDescriptor;  // 共享内存文件描述符
    char* sharedMemory;              // 共享内存数据指针

    PythonLoader* pLoader;
};