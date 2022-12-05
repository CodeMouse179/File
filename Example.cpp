#define SYSTEM_IO_FILE_ONLY_UTF8
#include "File.hpp"
using namespace System::IO;
int main()
{
    std::string workDirectory = File::CurrentDirectory();
#ifdef SYSTEM_WINDOWS
    std::string filePath1 = File::Combine(workDirectory, std::string(U8("\\res\\TestFile1.txt")));
    std::string filePath2 = File::Combine(workDirectory, std::string(U8("\\res\\测试文件1.txt")));
    std::string filePath3 = File::Combine(workDirectory, std::string(U8("\\res\\Windows系统文件.txt")));
#endif
#ifdef SYSTEM_LINUX
    std::string filePath1 = File::Combine(workDirectory, std::string(U8("/res/TestFile1.txt")));
    std::string filePath2 = File::Combine(workDirectory, std::string(U8("/res/测试文件1.txt")));
    std::string filePath3 = File::Combine(workDirectory, std::string(U8("/res/Linux系统文件.txt")));
#endif
    File::WriteAllText(filePath3, U8("你好世界😄"));
    std::string content = File::ReadAllText(filePath3);
    StringA::WriteLine(content);
    bool exists1 = File::Exists(filePath1);
    bool exists2 = File::Exists(filePath2);
    StringA::WriteLine(std::to_string(exists1));
    StringA::WriteLine(std::to_string(exists2));
    StringA::ReadLine();
    return 0;
}