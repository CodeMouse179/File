//     +--------------------------------------------------------------------------------+
//     |                                       File                                     |
//     |  Introduction : System.IO.File in C++                                          |
//     |  Modified Date : 2022/12/5                                                     |
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
#include <unistd.h>     //readlink, close
#include <sys/stat.h>   //stat
#include <fcntl.h>      //open
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

            //return 1 on success, return 0 on fail.
            static int Create(const std::string& path)
            {
#ifdef SYSTEM_WINDOWS
                std::wstring pathW;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
#else
                pathW = StringA::StringToWstring2(path);
#endif
                HANDLE fileHandle = CreateFileW(pathW.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (fileHandle == INVALID_HANDLE_VALUE) return 0;
                BOOL closeRet = CloseHandle(fileHandle);
                if (!closeRet) return 0;
                return 1;
#endif
#ifdef SYSTEM_LINUX
                int fd = open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if (fd == -1) return 0;
                int ret = close(fd);
                if (ret == -1) return 0;
                return 1;
#endif
            }

            static bool Delete(const std::string& path)
            {
#ifdef SYSTEM_WINDOWS
                std::wstring pathW;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
#else
                pathW = StringA::StringToWstring2(path);
#endif
                BOOL ret = DeleteFileW(pathW.c_str());
                return ret;
#endif
#ifdef SYSTEM_LINUX
                int ret = unlink(path.c_str());
                if (ret == -1) return false;
                return true;
#endif
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

            static std::vector<unsigned char> ReadAllBytes(const std::string& path)
            {
                std::vector<unsigned char> data;
#ifdef SYSTEM_WINDOWS
                std::wstring pathW;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
#else
                pathW = StringA::StringToWstring2(path);
#endif
                //open a existing file:
                HANDLE fileHandle = CreateFileW(pathW.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (fileHandle == INVALID_HANDLE_VALUE) return data;
                //get file size:
                LARGE_INTEGER large;
                BOOL sizeRet = GetFileSizeEx(fileHandle, &large);
                if (!sizeRet)
                {
                    //close file before return:
                    CloseHandle(fileHandle);
                    return data;
                }
                //new buffer:
                int bufferSize = large.QuadPart;
                unsigned char* buffer = new unsigned char[bufferSize];
                if (buffer == nullptr)
                {
                    //close file before return:
                    CloseHandle(fileHandle);
                    return data;
                }
                ZeroMemory(buffer, bufferSize);
                //read bytes:
                DWORD read;
                BOOL readRet = ReadFile(fileHandle, buffer, bufferSize, &read, NULL);
                if (!readRet)
                {
                    delete[] buffer;
                    //close file before return:
                    CloseHandle(fileHandle);
                    return data;
                }
                //close file normally:
                BOOL closeRet = CloseHandle(fileHandle);
                if (!closeRet)
                {
                    delete[] buffer;
                    return data;
                }
                //copy value:
                for (int i = 0; i < read; i++)
                {
                    data.push_back(buffer[i]);
                }
                //delete buffer:
                delete[] buffer;
                //read success!
#endif
#ifdef SYSTEM_LINUX
                //open a existing file:
                int fd = open(path.c_str(), O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if (fd == -1) return data;
                //get file size:
                struct stat att;
                int statRet = stat(path.c_str(), &att);
                if (statRet != 0)
                {
                    //close file before return:
                    close(fd);
                    return data;
                }
                //new buffer:
                int bufferSize = att.st_size;
                unsigned char* buffer = new unsigned char[bufferSize];
                if (buffer == nullptr)
                {
                    //close file before return:
                    close(fd);
                    return data;
                }
                memset(buffer, 0, bufferSize);
                //read bytes:
                ssize_t readRet = read(fd, buffer, bufferSize);
                if (readRet == -1)
                {
                    delete[] buffer;
                    //close file before return:
                    close(fd);
                    return data;
                }
                //close file normally:
                int closeRet = close(fd);
                if (closeRet == -1)
                {
                    delete[] buffer;
                    return data;
                }
                //copy value:
                for (int i = 0; i < readRet; i++)
                {
                    data.push_back(buffer[i]);
                }
                //delete buffer:
                delete[] buffer;
                //read success!
#endif
                return data;
            }

            static std::string ReadAllText(const std::string& path)
            {
                std::vector<unsigned char> data = File::ReadAllBytes(path);
                std::string s;
                for (int i = 0; i < data.size(); i++)
                {
                    s.push_back((char)data[i]);
                }
                return s;
            }

            static bool WriteAllBytes(const std::string& path, const std::vector<unsigned char>& bytes)
            {
#ifdef SYSTEM_WINDOWS
                std::wstring pathW;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                pathW = StringA::StringToWstring(path, System::StringEncoding::UTF8);
#else
                pathW = StringA::StringToWstring2(path);
#endif
                //create a new file or overwrite existing file:
                HANDLE fileHandle = CreateFileW(pathW.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (fileHandle == INVALID_HANDLE_VALUE) return false;
                //write bytes:
                DWORD written;
                BOOL writeRet = WriteFile(fileHandle, bytes.data(), bytes.size(), &written, NULL);
                if (!writeRet)
                {
                    //close file before return:
                    CloseHandle(fileHandle);
                    return false;
                }
                //close file normally:
                BOOL closeRet = CloseHandle(fileHandle);
                if (!closeRet) return false;
                //write success:
                return true;
#endif
#ifdef SYSTEM_LINUX
                //create a new file or overwrite existing file:
                int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
                if (fd == -1) return false;
                //write bytes:
                ssize_t writeRet = write(fd, bytes.data(), bytes.size());
                if (writeRet == -1)
                {
                    //close file before return:
                    close(fd);
                    return false;
                }
                //close file normally:
                int ret = close(fd);
                if (ret == -1) return false;
                //write success:
                return true;
#endif
            }

            static bool WriteAllText(const std::string& path, const std::string& contents)
            {
                std::string utf8String;
#ifdef SYSTEM_IO_FILE_ONLY_UTF8
                utf8String = contents;
#else
                if (!StringA::IsValidUTF8(contents))
                {
                    utf8String = StringA::ConvertString(contents, System::StringEncoding::ANSI, System::StringEncoding::UTF8);
                }
                else
                {
                    utf8String = contents;
                }
#endif
                std::vector<unsigned char> data;
                for (int i = 0; i < utf8String.size(); i++)
                {
                    data.push_back((unsigned char)utf8String[i]);
                }
                return File::WriteAllBytes(path, data);
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

            template<typename T>
            static std::basic_string<T> Combine(const std::basic_string<T>& path1, const std::basic_string<T>& path2)
            {
                if (path1.empty()) return System::String<T>::Empty();
                if (path2.empty()) return path1;
                std::basic_string<T> path;
#ifdef SYSTEM_WINDOWS
                if (path1.back() == (T)'/' || path1.back() == (T)'\\')
                {
                    if (path2.front() == (T)'/' || path2.front() == (T)'\\')
                    {
                        path = path1 + path2.substr(1, path2.size() - 1);
                    }
                    else
                    {
                        path = path1 + path2;
                    }
                }
                else
                {
                    if (path2.front() == (T)'/' || path2.front() == (T)'\\')
                    {
                        path = path1 + path2;
                    }
                    else
                    {
                        path = path1;
                        path.push_back((T)'\\');
                        path += path2;
                    }
                }
#endif
#ifdef SYSTEM_LINUX
                if (path1.back() == (T)'/')
                {
                    if (path2.front() == (T)'/')
                    {
                        path = path1 + path2.substr(1, path2.size() - 1);
                    }
                    else
                    {
                        path = path1 + path2;
                    }
                }
                else
                {
                    if (path2.front() == (T)'/')
                    {
                        path = path1 + path2;
                    }
                    else
                    {
                        path = path1;
                        path.push_back((T)'/');
                        path += path2;
                    }
                }
#endif
                return path;
            }
        };
    }
}

#endif