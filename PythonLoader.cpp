//  2023.5.21 by justinsong

#include <PythonLoader.h>
#include <fcntl.h>
#include <sys/mman.h>

PythonLoader::PythonLoader(string dirPath, string fileName, string className) {
    _dirPath = dirPath;
    _fileName = fileName;
    _className = className;

    initEnv();
    loadObj();
}

PythonLoader::~PythonLoader() {
    unloadObj();
    unInitEnv();
}

void PythonLoader::initEnv() {
    // 调用Python脚本进行音频转视频
    Py_Initialize();

    const char *version = Py_GetVersion();
    printf("Python Version %s\n", version);

    // ~/anaconda3/bin/python
    
    // 将Python模块的路径添加到sys.path中
    PyObject* sys_module = PyImport_ImportModule("sys");
    PyObject* sys_path = PyObject_GetAttrString(sys_module, "path");
    PyList_Append(sys_path, PyUnicode_FromString(_dirPath.c_str()));
    Py_DECREF(sys_module);
    Py_DECREF(sys_path);
}

void PythonLoader::unInitEnv() {
    Py_Finalize();
}

bool PythonLoader::loadObj() {
    // 加载python文件
    _pModule = PyImport_ImportModule(_fileName.c_str());
    if (_pModule == NULL) {
        printf("loadObj failed, import module failed\n");
        PyErr_Print();
        return false;
    }

    // 加载类 & 实例化
    _pClass = PyObject_GetAttrString(_pModule, _className.c_str());
    _pInstance = PyObject_CallObject(_pClass, NULL);

    if (_pInstance == NULL) {
        printf("loadObj failed, _pInstance create failed\n");
        return false;
    }

    printf("loadObj succeeded\n");
    return true;
}

void PythonLoader::unloadObj() {
    if (_pModule) {
        Py_DECREF(_pModule);
    }

    if (_pClass) {
        Py_DECREF(_pClass);
    }

    if (_pInstance) {
        Py_DECREF(_pInstance);
    }
}

//////////////////////////////测试方法//////////////////////////////////

void PythonLoader::add() {
    // 方法调用
    PyObject* pMethod = PyObject_GetAttrString(_pInstance, "add");
    PyObject* pRet = PyObject_CallObject(pMethod, NULL);

    // 释放python对象
    Py_DECREF(pRet);
    Py_DECREF(pMethod);
}

int PythonLoader::get() {
    // 方法调用
    PyObject* pMethod = PyObject_GetAttrString(_pInstance, "get");
    PyObject* pRet = PyObject_CallObject(pMethod, NULL);
    if (pRet == NULL) {
        PyErr_Print();
        return -1;
    }

    // 返回值转换
    long res = PyLong_AsLong(pRet);
    if (res == -1 && PyErr_Occurred()) {
        PyErr_Print();
        return -1;
    }

    // 释放python对象
    Py_DECREF(pRet);
    Py_DECREF(pMethod);

    return (int)res;
}

void PythonLoader::pythonRead() {
    // 方法调用
    PyObject* pMethod = PyObject_GetAttrString(_pInstance, "readFromSharedMemory");
    PyObject* pRet = PyObject_CallObject(pMethod, NULL);

    // 释放python对象
    Py_DECREF(pRet);
    Py_DECREF(pMethod);
}

void PythonLoader::pythonWrite() {
    // 方法调用
    PyObject* pMethod = PyObject_GetAttrString(_pInstance, "writeToSharedMemory");
    PyObject* pRet = PyObject_CallObject(pMethod, NULL);

    // 释放python对象
    Py_DECREF(pRet);
    Py_DECREF(pMethod);
}

void PythonLoader::pythonRead2() {
    // 方法调用
    PyObject* pMethod = PyObject_GetAttrString(_pInstance, "readFromSharedMemory2");
    PyObject* pRet = PyObject_CallObject(pMethod, NULL);

    // 释放python对象
    Py_DECREF(pRet);
    Py_DECREF(pMethod);
}


void PythonLoader::writeToShareMemory() {
    printf("C++ writeToSharedMemory started\n");
    
    int fd;
    fd = shm_open("audio_video_shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("error open shm_region\n");
        return ;
    }
    ftruncate(fd, 10);
    void* ptr = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("error map\n");
        return ;
    }

    int* data = reinterpret_cast<int*>(ptr);
    *data = 123;

    printf("C++ writeToSharedMemory 123 end\n");
}

void PythonLoader::readFromShareMemory() {
    printf("C++ readFromSharedMemory started\n");
    int fd = shm_open("audio_video_shm", O_RDONLY, 0666);
    void* ptr = mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
    int* data = reinterpret_cast<int*>(ptr);
    std::cout << "c++ read " << *data << std::endl;
    printf("C++ readFromSharedMemory end\n");
}

int PythonLoader::test() {
    // 创建共享内存对象
    shared_memory_object shm(open_or_create, "myshm", read_write);

    // 设置共享内存大小
    shm.truncate(1024);

    // 映射共享内存到当前进程地址空间
    mapped_region region(shm, read_write);

    // 获取共享内存数据指针
    char* data = static_cast<char*>(region.get_address());

    // 向共享内存写入数据
    std::strcpy(data, "hello from C++");

    // 等待Python进程读取数据
    std::cout << "waiting for Python process..." << std::endl;

    std::cin.get();

    // 从共享内存读取Python进程写入的数据
    std::cout << "C++ received: " << data << std::endl;

    // 将C++数据写入共享内存
    std::strcpy(data, "hello from C++ again");

    // 通知Python进程读取数据
    std::cout << "C++ wrote: " << data << std::endl;
    std::cin.get();

    // 删除共享内存对象
    shared_memory_object::remove("myshm");

    return 0;
}
