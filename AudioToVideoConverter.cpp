// 2023.5.21 justinsong

#include "AudioToVideoConverter.h"

#include <stdio.h>
#include <sys/time.h>

#include <algorithm>

int64_t getCurrentTime()  // 直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
{
    struct timeval tv;
    gettimeofday(&tv, NULL);  // 该函数在sys/time.h头文件中
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int min(int a, int b) {
    return a < b ? a : b;
}

AudioToVideoConverter::AudioToVideoConverter(const std::string& audioFilePath, const size_t audioDataSize)
    : audioFilePath(audioFilePath), audioDataSize(audioDataSize), audioDataOffset(0), sharedMemory(NULL) {
    // 打开音频文件
    audioFileDescriptor = open(audioFilePath.c_str(), O_RDONLY);
    if (audioFileDescriptor < 0) {
        std::cerr << "Failed to open audio file: " << audioFilePath << std::endl;
        exit(EXIT_FAILURE);
    }

    // 获取音频文件大小
    struct stat st;
    if (fstat(audioFileDescriptor, &st) < 0) {
        std::cerr << "Failed to get audio file size" << std::endl;
        exit(EXIT_FAILURE);
    }
    audioFileSize = st.st_size;

    // 将音频文件映射到内存中
    audioFileData = (char*)mmap(NULL, audioFileSize, PROT_READ, MAP_SHARED, audioFileDescriptor, 0);
    if (audioFileData == MAP_FAILED) {
        std::cerr << "Failed to map audio file to memory" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 计算音频数据总大小
    audioDataTotalSize = (audioFileSize / audioDataSize) * audioDataSize;
    if (audioFileSize % audioDataSize != 0) {
        audioDataTotalSize += audioDataSize;
    }

}

AudioToVideoConverter::~AudioToVideoConverter() {
    // 关闭文件描述符
    close(audioFileDescriptor);

    // 解除内存映射
    munmap(audioFileData, audioFileSize);

    // 解除共享内存映射
    if (sharedMemory != NULL) {
        munmap(sharedMemory, audioDataSize);
    }
    if (sharedMemoryFileDescriptor > 0) {
        close(sharedMemoryFileDescriptor);
    }
}

void AudioToVideoConverter::convertToVideo() {
    // 打开共享内存
    sharedMemoryFileDescriptor = shm_open("/my_shared_memory", O_RDWR | O_CREAT, 0666);
    if (sharedMemoryFileDescriptor < 0) {
        std::cerr << "Failed to open shared memory" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 设置共享内存大小
    ftruncate(sharedMemoryFileDescriptor, audioDataSize);




    string dirPath = "/home/DIDNET/PythonLoader/";
    string fileName = "audio_to_video_converter";
    string className = "AudioToVideoConverter";
    pLoader = new PythonLoader(dirPath, fileName, className);


    // 映射共享内存到进程空间
    sharedMemory = (char*)mmap(NULL, audioDataSize, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0);
    if (sharedMemory == MAP_FAILED) {
        std::cerr << "Failed to map shared memory to process space" << std::endl;
        exit(EXIT_FAILURE);
    }

    // char* buffer = new char[audioDataSize];
    // std::memcpy(buffer, audioFileData, audioDataSize);

    int i = 0;
    int totalBytes = 0;
    int64_t startTime = getCurrentTime();
    // 循环读取音频数据
    while (audioDataOffset < audioDataTotalSize) {
        // 拷贝音频数据到共享内存中
        int copySize = min(audioDataSize, audioDataTotalSize - audioDataOffset);
        std::memcpy(sharedMemory, audioFileData + audioDataOffset, copySize);
        // std::memcpy(sharedMemory, buffer, copySize);
        audioDataOffset += copySize;

        // 等待Python进程处理
        pLoader->pythonRead();
        
        // readFromSharedMemory

        printf("C++ to Python, index:%d, data size: %d\n", i, copySize);
        totalBytes += copySize;
        i++;
    }

    int64_t endTime = getCurrentTime();
    int64_t cost = (endTime - startTime);
    printf("C++ to Python, total size:%d, cost: %lld\n", totalBytes, cost);

    int a = 0;
    a++;
}

void AudioToVideoConverter::readTest() {

    // 打开共享内存
    sharedMemoryFileDescriptor = shm_open("/audio_video_shm", O_RDONLY, 0666);
    if (sharedMemoryFileDescriptor < 0) {
        std::cerr << "Failed to open shared memory" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 设置共享内存大小
    ftruncate(sharedMemoryFileDescriptor, audioDataSize);

    // 映射共享内存到进程空间
    sharedMemory = (char*)mmap(NULL, audioDataSize, PROT_READ, MAP_SHARED, sharedMemoryFileDescriptor, 0);
    if (sharedMemory == MAP_FAILED) {
        std::cerr << "Failed to map shared memory to process space" << std::endl;
        exit(EXIT_FAILURE);
    }

    string dirPath = "/home/DIDNET/PythonLoader/";
    string fileName = "audio_to_video_converter";
    string className = "AudioToVideoConverter";
    pLoader = new PythonLoader(dirPath, fileName, className);

    int i = 0;
    int totalBytes = 0;

    int copySize = 32000;

    char *buffer = (char *)malloc(audioDataTotalSize);


    int64_t startTime = getCurrentTime();

    // FILE *fptr;

    // fptr = fopen("C++ReadRet.txt", "ab");
    // if (fptr == NULL) {
    //     printf("Error!");
    //     exit(1);
    // }



    // 循环读取音频数据
    while (audioDataOffset < (audioDataTotalSize - audioDataOffset)) {
        pLoader->pythonWrite();

        std::memcpy(buffer, sharedMemory, copySize);

        // fwrite(buffer, 1, audioDataTotalSize, fptr);

        audioDataOffset += copySize;

        printf("C++ to Python, index:%d, data size: %d\n", i, copySize);
        totalBytes += copySize;
        i++;
    }

    // fclose(fptr);
    int64_t endTime = getCurrentTime();
    int64_t cost = (endTime - startTime);
    printf("C++ to Python, total size:%d, cost: %lld\n", totalBytes, cost);

    int a = 0;
    a++;
}
