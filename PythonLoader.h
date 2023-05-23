//  2023.5.21 by justinsong

#pragma once

#include <Python.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;
using namespace boost::interprocess;

class PythonLoader {
public:
    PythonLoader(string dirPath, string fileName, string className);

    virtual ~PythonLoader();

    void add();

    int get();

    int test();

    void pythonRead();
    void pythonRead2();

    void pythonWrite();

    void writeToShareMemory();

    void readFromShareMemory();

protected:
    void initEnv();

    void unInitEnv();

    bool loadObj();

    void unloadObj();

private:
    string _dirPath;
    string _fileName;
    string _className;

    PyObject *_pModule;
    PyObject *_pClass;
    PyObject *_pInstance;
    
};