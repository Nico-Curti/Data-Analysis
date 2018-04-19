#pragma once

#if defined (_WIN32)
const char sep = '\\';
#define GetCurrentDir _getcwd
#define Popen _popen
#include <Windows.h>
#include <direct.h>
#else
const char sep = '/';
#define GetCurrentDir getcwd
#define Popen popen
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#endif

#include <iostream>
#include <map>
#include <unordered_set>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif

std::vector<std::string> split(std::string const& , char );
std::vector<std::string> split(const std::string &, const std::string &);
inline std::string joinString(const char &, const std::string &, const std::string &);
template<typename... Args> inline std::string joinString(const char &, const std::string &, const std::string &, Args... );
template<typename T> inline void w_idx(T &, std::vector<std::string> &, int , int );
template<typename... Args, typename T> inline void w_idx(T &, std::vector<std::string> &, int , Args ...);
void parse_yml(const std::string &, std::map<std::string, std::map<std::string, std::string>> &);
inline char str2num(const std::string &);
std::map<std::string, std::string> parse_fasta(const std::string &);
template<typename T> inline std::string type_name();

namespace os
{
    inline std::string capture_output(const char* );
    inline std::string local_path();
    template<typename... Args> void awk(const std::string &, const char &, std::string output = "", int i1 = 0, Args ...args);
    inline bool dir_exists(const std::string& );
    inline bool file_exists(const std::string &);
    inline std::vector<std::string> files_in_directory(const std::string &);
    inline unsigned int path_size(const std::string &);
    inline void copy(const std::string &, const std::string &);
    inline void hide_file(const std::string &);
    inline std::vector<std::string> get_subdir(const std::string& );
    void ascii2bin(const std::string &, const std::string &, const char &);
}



/**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-***_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
*_**                                                                                                                                                                                            *_**
*_**    :::::::::::  ::::    ::::   :::::::::   :::         ::::::::::  ::::    ::::   ::::::::::  ::::    :::  :::::::::::      :::      :::::::::::  :::::::::::   ::::::::   ::::    :::     *_**
*_**        :+:      +:+:+: :+:+:+  :+:    :+:  :+:         :+:         +:+:+: :+:+:+  :+:         :+:+:   :+:      :+:        :+: :+:        :+:          :+:      :+:    :+:  :+:+:   :+:     *_**
*_**        +:+      +:+ +:+:+ +:+  +:+    +:+  +:+         +:+         +:+ +:+:+ +:+  +:+         :+:+:+  +:+      +:+       +:+   +:+       +:+          +:+      +:+    +:+  :+:+:+  +:+     *_**
*_**        +#+      +#+  +:+  +#+  +#++:++#+   +#+         +#++:++#    +#+  +:+  +#+  +#++:++#    +#+ +:+ +#+      +#+      +#++:++#++:      +#+          +#+      +#+    +:+  +#+ +:+ +#+     *_**
*_**        +#+      +#+       +#+  +#+         +#+         +#+         +#+       +#+  +#+         +#+  +#+#+#      +#+      +#+     +#+      +#+          +#+      +#+    +#+  +#+  +#+#+#     *_**
*_**        #+#      #+#       #+#  #+#         #+#         #+#         #+#       #+#  #+#         #+#   #+#+#      #+#      #+#     #+#      #+#          #+#      #+#    #+#  #+#   #+#+#     *_**
*_**    ###########  ###       ###  ###         ##########  ##########  ###       ###  ##########  ###    ####      ###      ###     ###      ###      ###########   ########   ###    ####     *_**
*_**                                                                                                                                                                                            *_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**
-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**--**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**_**/



std::vector<std::string> split(std::string const& original, char separator)
{
    std::vector<std::string> results;
    std::string::const_iterator start = original.begin(), end = original.end(), next = std::find(start, end, separator);
    while (next != end)
    {
        results.push_back(std::string(start, next));
        start = next + 1;
        next = std::find(start, end, separator);
    }
    results.push_back(std::string(start, next));
    return results;
}

std::vector<std::string> split(const std::string &txt, const std::string &del)
{
    std::vector<std::string> token;
    std::size_t pos = txt.find_first_of(del), start = 0, end = txt.size();
    while(pos != std::string::npos)
    {
        if(pos) token.push_back(txt.substr(start, pos));
        start += pos + 1;
        pos = txt.substr(start, end).find_first_of(del);
    }
    if(start != end) token.push_back(txt.substr(start, pos));
    return token;
}

inline std::string joinString(const char &sep, const std::string &tk1, const std::string &tk2)
{   return tk1 + sep + tk2; }
template<typename... Args> inline std::string joinString(const char &sep, const std::string &tk1, const std::string &tk2, Args... args)
{   return tk1 + sep + joinString(sep, tk2, args...);   }


template<typename T> inline void w_idx(T &os, std::vector<std::string> &row, int i1, int i2)
{   os << row[i1] << " " << row[i2] << std::endl;   }
template<typename... Args, typename T> inline void w_idx(T &os, std::vector<std::string> &row, int i1, Args ...args)
{   os << row[i1] << " " << w_idx(os, row, args...); return;}

void parse_yml(const std::string &filename, std::map<std::string, std::map<std::string, std::string>> &yml)
{
    std::ifstream is(filename);
    if(!is){std::cerr << "Parse Yml error! File not found. Given: " << filename << std::endl; exit(1);}
    std::string row, tag1, tag2, key;
    std::size_t pos;

    while(std::getline(is, row))
    {
        row = row.substr(0, row.find_first_of("#")); // rimozione di eventuali commenti
        row.erase(0, row.find_first_not_of(" \t")); // rimozione di spazi vuoti iniziali extra
        if(row.size() == 0) // se non ci sono + elementi vado avanti
            continue;
        pos = row.find_first_of(":");
        if(pos == row.size() - 1)
        {
            key = row.substr(0, pos);
            continue;
        }

        tag1 = row.substr(0, pos);
        tag2 = row.substr(pos + 1, row.size());
        tag2.erase(std::remove(tag2.begin(), tag2.end(), '\''), tag2.end()); // rimuovo eventuali double-quotes
        tag2.erase(0, tag2.find_first_not_of(" \t")); // rimozione di spazi iniziali vuoti extra
        yml[key][tag1] = tag2;
    }
    is.close();
    return;
}

inline char str2num(const std::string &str)
{
    if(!isdigit(str[0])) return 's';
    else
    {
        int f = 0, dgt = 1;
        bool pt = false;
        for(int i = 1; i < (int)str.size(); ++i)
        {
            if(isdigit(str[i])) ++dgt;
            else if(str[i] == '.') pt = true;
            else if(str[i] == 'f') ++f;
            else break;
        }
        if(dgt + pt + f == (int)str.size()) // numerical string
            return (pt && !f) ? 'd' : (pt && f) ? 'f' : 'i';
        else
            return 's';
    }
}

std::map<std::string, std::string> parse_fasta(const std::string &filename)
{
    std::ifstream is(filename);
    if(!is){std::cerr << "Parse fasta error! Fasta file not found. Given " << filename << std::endl; exit(1);}
    std::string row, name, content;
    std::map<std::string, std::string> fasta;
    while(std::getline(is, row).good())
    {
        if(row.empty() || row[0] == '>')
        {
            if(!name.empty())
            {
                fasta[name] = content;
                name.clear();
            }
            if(!row.empty())
                name = row.substr(1);
            content.clear();
        }
        else if(!name.empty())
        {
            if(row.find(' ') != std::string::npos)
            {
                name.clear();
                content.clear();
            }
            else
                content += row;
        }
    }
    if(!name.empty())
        fasta[name] = content;
    return fasta;
}

template<typename T> inline std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}


namespace os
{

    inline std::string capture_output(const char* cmd)
    {
        // Pay Attention : if you run this function in WIN32 cmd must be related to Command Prompt (NO POWERSHELL!)
        std::string result;
#if !defined (__clang__)
        std::array<char, FILENAME_MAX> buffer;
#ifdef _MSC_VER
        std::shared_ptr<FILE> pipe(Popen(cmd, "r"), _pclose);
#else
        std::shared_ptr<FILE> pipe(Popen(cmd, "r"), pclose);
#endif
            if (!pipe) throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), FILENAME_MAX, pipe.get()) != nullptr)
                result += buffer.data();
            }
#else
        std::cerr << "Unsupported architecture!" << std::endl;
        exit(1);
#endif
        return result;
    }

    inline std::string local_path()
    {
        char cCurrentPath[FILENAME_MAX];

        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            std::cerr << "Path not found!" << std::endl;
            exit(1);
        }

        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
        return (std::string)cCurrentPath;
    }

    template<typename... Args> void awk(const std::string &input, const char &sep, std::string output, int i1, Args ...args)
    {
        std::string row;
        std::vector<std::string> token;
        std::ifstream is(input);
        std::ofstream os(output);
        while(std::getline(is, row))
        {
            token = split(row, sep);
            if(token.size())
            {
                if(output != "")
                {
                    os << token[i1] << " ";
                    w_idx(os, token, args...);
                }
                else
                {
                    std::cout << token[i1] << " ";
                    w_idx(std::cout, token, args...);
                }
            }
            else
            {
                os << std::endl;
                std::cout << std::endl;
            }
        }
        is.close();
        os.close();
        return;
    }


    inline bool dir_exists(const std::string& dirName_in)
    {
#if defined (_WIN32)
        DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;  //something is wrong with your path!

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;   // this is a directory!

        return false;    // this is not a directory!
#else
        DIR* dir = opendir(dirName_in.c_str());
        if( dir )
        {
            closedir(dir);
            return true;
        }
        else if (ENOENT == errno)
        {
            return false;
        }
        return false;
#endif
    }

    inline bool file_exists(const std::string &file)
    {
        std::ifstream file_to_check(file.c_str());
        bool ret = (file_to_check.is_open()) ? true : false;
        file_to_check.close();
        return ret;
    }

    inline std::vector<std::string> files_in_directory(const std::string &directory)
    {
        std::vector<std::string> files;
#if defined (_WIN32)
        WIN32_FIND_DATA fileData;
        HANDLE hFind;
        if (!((hFind = FindFirstFile((directory + "*").c_str(), &fileData)) == INVALID_HANDLE_VALUE))
            while (FindNextFile(hFind, &fileData))
                if (file_exists(directory + fileData.cFileName))
                    files.push_back(fileData.cFileName);

        FindClose(hFind);
#else
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(directory.c_str())) == nullptr)
        {
            std::cerr << "Error(" << errno << ") opening " << directory << std::endl;
            exit(1);
        }
        while ((dirp = readdir(dp)) != nullptr)
            if(directory + std::string(dirp->d_name) != "." && directory + std::string(dirp->d_name) != ".." && !dir_exists(dirp->d_name))
                files.push_back(std::string(dirp->d_name));
        closedir(dp);
#endif
        //std::cout << files.size() << " files found in " << directory << std::endl;
        return files;
    }

    inline unsigned int path_size(const std::string &path)
    {
#ifdef _WIN32
        WIN32_FIND_DATA data;
        int size = 0;
        std::string fname = path + "\\*.*";
        HANDLE h = FindFirstFile(fname.c_str(),&data);
        if(h != INVALID_HANDLE_VALUE)
        {
            do {
                if( (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
                {
                    // make sure we skip "." and "..".  Have to use strcmp here because
                    // some file names can start with a dot, so just testing for the
                    // first dot is not suffient.
                    if( strcmp(data.cFileName,".") != 0 &&strcmp(data.cFileName,"..") != 0)
                    {
                        // We found a sub-directory, so get the files in it too
                        fname = path + sep + data.cFileName;
                        // recurrsion here!
                        size += path_size(fname);
                    }

                }
                else
                {
                    LARGE_INTEGER sz;
                    // All we want here is the file size.  Since file sizes can be larger
                    // than 2 gig, the size is reported as two DWORD objects.  Below we
                    // combine them to make one 64-bit integer.
                    sz.LowPart = data.nFileSizeLow;
                    sz.HighPart = data.nFileSizeHigh;
                    size += (unsigned int)sz.QuadPart;

                }
            }while( FindNextFile(h,&data) != 0);
            FindClose(h);
        }
        return size;
#else
        std::string size = capture_output(("du -sb " + path).c_str());
        return std::stoi(size.substr(0, size.find_first_of("/")));
#endif
    }

    inline void copy(const std::string &source, const std::string &dest)
    {
        if(file_exists(source))
        {
            if( 0 != std::system(("cp " + source + " " + dest).c_str()) )
                exit(1);
        }
        else
        {
            std::cerr << "cp: impossibile eseguire stat di " << source << ": File o directory non esistente" << std::endl;
            exit(1);
        }
        return;
    }

    inline void hide_file(const std::string &filename)
    {
#ifdef _WIN32
        if(file_exists(filename))
        {
            DWORD attributes = GetFileAttributes(filename.c_str());
            SetFileAttributes(filename.c_str(), attributes + FILE_ATTRIBUTE_HIDDEN);
        }
        else
        {
            std::cerr << "mv: impossibile eseguire stat di " << filename << ": File o directory non esistente" << std::endl;
            exit(1);
        }
#else
        if(file_exists(filename))
        {
            if( 0 != std::system(("mv " + filename + " /." + filename).c_str()) )
                exit(1);
        }
        else
        {
            std::cerr << "mv: impossibile eseguire stat di " << filename << ": File o directory non esistente" << std::endl;
            exit(1);
        }
#endif
        return;
    }

    inline std::vector<std::string> get_subdir(const std::string& path)
    {
        std::vector<std::string> directories;
#ifdef _WIN32
        WIN32_FIND_DATA findfiledata;
        HANDLE hFind = INVALID_HANDLE_VALUE;
        char fullpath[MAX_PATH];
        GetFullPathName(path.c_str(), MAX_PATH, fullpath, 0);
        std::string fp(fullpath);

        hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if ((findfiledata.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && (findfiledata.cFileName[0] != '.'))
                    directories.push_back(findfiledata.cFileName);
            } while (FindNextFile(hFind, &findfiledata) != 0);
        }
#else
        DIR *dir = opendir(path.c_str());
        dirent *entry = readdir(dir);
        while (entry != nullptr)
        {
            if (entry->d_type == DT_DIR && (std::string)entry->d_name != "." && (std::string)entry->d_name != "..")
                directories.push_back(entry->d_name);
            entry = readdir(dir);
        }
        closedir(dir);
#endif
        return directories;
    }

    void ascii2bin(const std::string &input, const std::string &output, const char &sep)
    {
        std::ifstream is(input);
        if(!is){std::cerr << "Ascii2bin error! File not found. Given : " << input << std::endl; exit(1);}
        std::ofstream os;
        os.open(output, std::ios::out | std::ios::binary);
        std::vector<std::string> token;
        std::string row;
        int i; float f; double d;

        while(std::getline(is, row))
        {
            token = split(row, sep);
            for(auto &tk : token)
            {
                switch(str2num(tk))
                {
                    case 's': { os.write(&tk[0], tk.size()); break;}
                    case 'd': { d = std::stod(tk); os.write( (const char *) &d, sizeof( double )); break;}
                    case 'f': { f = std::stof(tk); os.write( (const char *) &f, sizeof( float )); break;}
                    case 'i': { i = std::stoi(tk); os.write( (const char *) &i, sizeof( int )); break;}
                }
            }
        }

        is.close();
        os.close();
        return;
    }
} // end of namespace


