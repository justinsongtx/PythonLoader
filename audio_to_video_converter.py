
import numpy as np
import mmap
import os

class AudioToVideoConverter:

    index = 1

    def __init__(self):
        path = "/dev/shm/my_shared_memory"
        with open(path, "r+b") as f:
            f.write(b"this is a share memory file")
            self.mm = mmap.mmap(f.fileno(), 0)

        
        path = "/dev/shm/audio_video_shm"
        with open(path, "r+b") as f:
            f.write(b"this is a share memory file")
            f.close()

        print("python writeToShared called 1")
        with open(path, "r+b") as f:
            f.write(b"this is a share memory file")
            self.mmVideo = mmap.mmap(f.fileno(), length=3200, access=mmap.ACCESS_WRITE)

        path = "bookSample copy.txt"
        with open(path, "r+b") as f:
            self.srcFile = mmap.mmap(f.fileno(), 0)


        pass

    def add(self):
        print("==+ add")
        self.index += 1
    
    def get(self):
        print("==+ get")
        return self.index
    
    def convert(self, audio_file):
        print("conver called start")

        # 读取音频文件到内存
        with open(audio_file, "rb") as f:
            audio_data = mmap.mmap(f.fileno(), 0, mmap.MAP_PRIVATE, mmap.PROT_READ)

        # 假设音频转视频为数据加上10
        video_data = np.frombuffer(audio_data, dtype='uint8') + 10
        
    
        # 将视频数据写入内存映射
        video_mem = mmap.mmap(-1, len(video_data), mmap.MAP_PRIVATE | mmap.PROT_WRITE, mmap.MAP_ANONYMOUS | mmap.MAP_SHARED)
        video_mem.write(video_data.tobytes())
        video_mem.seek(0)

        print("conver called end")
                
        return video_mem
    
    def readFromSharedMemory(self):
        
        try:
            # 执行操作
            data = self.mm.read()
            # with open("pythonReadRet2.txt", "ab") as variable_name:
            #     variable_name.write(data)
            
            # variable_name.close()

            print("python readFromShared successfully")
        except Exception as e:
            print(e)

    def writeToSharedMemory(self):
        print("python writeToShared called start")


        # self.mmVideo.seek(0)
        # self.srcFile.seek(0)
        # file1_size = len(self.srcFile)
        # print("python writeToShared len " + str(file1_size))
        byteData = self.srcFile.read(32000)
        # print("python writeToShared" + str(byteData))

        self.mmVideo.write(b"this is a share memory file1")
        print("python writeToShared called end")


    # def writeToSharedMemory2(self):
    #     print("python writeToShared called start")
        
    #     with open("/dev/shm/video_shm", "r+b") as f:
    #         print("python writeToShared called 1")
    #         mm = mmap.mmap(f.fileno(), 0)
    #         print("python writeToShared called 2")
    #         mm.seek(0)
    #         mm.write(int(456).to_bytes(4, byteorder='little'))
    #     print("python writeToShared called end")
        

    # def readFromSharedMemory2(self):
    #     print("readFromSharedMemory2 start")

    #     try:
            
    #         name = "/my_shared_memory"
    #         SIZE = 4096

    #         # 1. 打开共享内存对象
    #         shm_fd = os.open(name, os.O_RDONLY)
    #         if shm_fd == -1:
    #             print("无法打开共享内存对象")
    #             exit(1)

    #         # 2. 将共享内存区域映射到进程的地址空间
    #         ptr = mmap.mmap(shm_fd, SIZE, mmap.MAP_SHARED, mmap.PROT_READ)

    #         # 3. 读取共享内存区域中的数据
    #         message = ptr.readline().decode().rstrip('\x00')
    #         print("python 已从共享内存读取数据: " + message)

    #         # 4. 解除共享内存区域与进程地址空间的映射
    #         ptr.close()

    #         # 5. 关闭共享内存对象
    #         os.close(shm_fd)
    #     except Exception as e:
    #         print(e)

    #     print("readFromSharedMemory2 end")



# av = AudioToVideoConverter()
# av.writeToSharedMemory()
# av.readFromSharedMemory()
# a = 0

# a+=1