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
                if (StringA::IsValidUTF8(path))
                {
                    pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
                }
                else
                {
                    pathW = StringA::StringToWstring(path, System::StringEncoding::ANSI);
                }
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
#endif
                return fatt;
            }
        };
    }
}

#endif