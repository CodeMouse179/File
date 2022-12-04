//     +--------------------------------------------------------------------------------+
//     |                                       File                                     |
//     |  Introduction : System.IO.File in C++                                          |
//     |  Modified Date : 2022/12/4                                                     |
//     |  License : MIT                                                                 |
//     |  Source Code : https://github.com/CodeMouse179/File                            |
//     |  Readme : https://github.com/CodeMouse179/File/blob/main/README.md             |
//     |                                                                                |
//     |                             Designer : CodeMouse179                            |
//     |  Email : codemouse179@gmail.com                                                |
//     |  Github : https://github.com/CodeMouse179                                      |
//     |  Bilibili : https://space.bilibili.com/3461577785215838                        |
//     +--------------------------------------------------------------------------------+

#ifndef SYSTEM_IO_FILE_HPP
#define SYSTEM_IO_FILE_HPP

#include "String.hpp"

#ifdef SYSTEM_LINUX
#include <unistd.h>     //readlink
#include <sys/stat.h>   //stat
#endif

namespace System
{
    namespace IO
    {
        struct FileAttributes
        {
        public:
            bool Invalid;
            bool IsDirectory;
            bool IsArchive;

            FileAttributes()
            {
                this->Invalid = false;
                this->IsDirectory = false;
                this->IsArchive = false;
            }
        };

        class File
        {
        private:
            //static class
            File()
            {
            }

        public:
            static bool Copy(const std::string& sourceFileName, const std::string& destFileName)
            {
                return false;
            }

            static bool Copy(const std::string& sourceFileName, const std::string& destFileName, bool overwrite)
            {
                return false;
            }

            static int Create(const std::string& path)
            {
                return 0;
            }

            static bool Delete(const std::string& path)
            {
                return false;
            }

            static bool Exists(const std::string& path)
            {
                FileAttributes fatt = File::GetAttributes(path);
                if (fatt.Invalid) return false;
                return fatt.IsArchive;
            }

            static FileAttributes GetAttributes(const std::string& path)
            {
                FileAttributes fatt;
#ifdef SYSTEM_WINDOWS
                std::wstring pathW;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
#else
                pathW = StringA::StringToWstring2(path);
#endif
                DWORD att = GetFileAttributesW(pathW.c_str());
                if (att == INVALID_FILE_ATTRIBUTES)
                {
                    fatt.Invalid = true;
                }
                else
                {
                    if (att & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        fatt.IsDirectory = true;
                    }
                    if (att & FILE_ATTRIBUTE_ARCHIVE)
                    {
                        fatt.IsArchive = true;
                    }
                }
#endif
#ifdef SYSTEM_LINUX
                struct stat att;
                int ret = stat(path.c_str(), &att);
                if (ret != 0)
                {
                    fatt.Invalid = true;
                }
                else
                {
                    if (S_ISDIR(att.st_mode))
                    {
                        fatt.IsDirectory = true;
                    }
                    if (S_ISREG(att.st_mode))
                    {
                        fatt.IsArchive = true;
                    }
                }
#endif
                return fatt;
            }

        public:
            static std::string CurrentDirectory()
            {
                std::string result;
#ifdef SYSTEM_WINDOWS
                const int bufferSize = 1024;
                wchar_t buffer[bufferSize];
                DWORD ret = GetModuleFileNameW(NULL, buffer, bufferSize);
                if (ret == 0) return StringA::Empty();
                std::wstring temp = buffer;
                size_t pos = temp.find_last_of(W('\\'));
                if (pos != std::string::npos)
                {
                    temp = temp.substr(0, pos);
                }
                result = StringA::WstringToString(temp, System::StringEncoding::UTF8);
#endif
#ifdef SYSTEM_LINUX
                const int bufferSize = 4096;
                char exePath[bufferSize];
                ssize_t ret = readlink("/proc/self/exe", exePath, bufferSize);
                if (ret == -1) return StringA::Empty();
                result = exePath;
                size_t pos = result.find_last_of('/');
                if (pos != std::string::npos)
                {
                    result = result.substr(0, pos);
                }
#endif
                return result;
            }
        };
    }
}

#endif