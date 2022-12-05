﻿#define SYSTEM_IO_FILE_ONLY_UTF8
#include "File.hpp"
using namespace System::IO;

int main()
{
    std::string workDirectory = File::CurrentDirectory();
#ifdef SYSTEM_WINDOWS
    bool exists1 = File::Exists(workDirectory + U8("\\res\\TestFile1.txt"));
    bool exists2 = File::Exists(workDirectory + U8("\\res\\测试文件1.txt"));
    File::Create(workDirectory + U8("\\res\\Windows系统文件.txt"));
    File::Delete(workDirectory + U8("\\res\\Windows系统文件.txt"));
#endif
#ifdef SYSTEM_LINUX
    bool exists1 = File::Exists(workDirectory + U8("/res/TestFile1.txt"));
    bool exists2 = File::Exists(workDirectory + U8("/res/测试文件1.txt"));
    File::Create(workDirectory + U8("/res/Linux系统文件.txt"));
    File::Delete(workDirectory + U8("/res/Linux系统文件.txt"));
#endif
    StringA::WriteLine(std::to_string(exists1));
    StringA::WriteLine(std::to_string(exists2));
    StringA::ReadLine();
    return 0;
}