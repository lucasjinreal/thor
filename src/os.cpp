//
// Created by jintian on 7/14/17.
//
// this library provider some function to
// judge a directory is exist or not
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <valarray>
#include <include/logging.h>
#include "include/str_util.h"
#include "include/os.h"

struct stat info;
typedef struct stat Stat;

using namespace std;

bool thor::os::exists(string path)
{
    // if path exists, return true else return false
    assert(!path.empty());
    if (stat(&path[0u], &info) != 0)
    {
        // path not exist
        return false;
    }
    else
    {
        // this is a file
        return true;
    }
}

bool thor::os::isdir(string path)
{
    // judge is directory or file, if dir return true, if file return false
    assert(!path.empty());
    if (stat(&path[0u], &info) != 0)
    {
        // path not exist
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        // this is a directory
        return true;
    }
    else
    {
        return false;
    }
}

bool thor::os::isfile(string path)
{
    // return a path is file or not
    assert(!path.empty());
    if (stat(&path[0u], &info) != 0)
    {
        // path not exist
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        // this is a directory
        return false;
    }
    else
    {
        return true;
    }
}

// updated for some filesystem can not
// different dir and file

vector<string> thor::os::list_files(string path, bool full_path)
{
    assert(!path.empty());
    DIR *dp;
    struct dirent *dirP;
    struct stat file_info;

    vector<string> files;
    if ((dp = opendir(&path[0u])) == nullptr)
    {
        cerr << "dir not exist....";
    }
    while ((dirP = readdir(dp)) != nullptr)
    {
        // in xfs file system, d_type always be NULL
        if (!dirP->d_type)
        {
            string f_name = path + string("/") + string(dirP->d_name);
            lstat(f_name.c_str(), &file_info);
            if (S_ISDIR(file_info.st_mode))
            {
                continue;
            }
            else
            {
                if (full_path)
                {
                    files.push_back(path + string("/") + string(dirP->d_name));
                }
                else
                {
                    files.emplace_back(string(dirP->d_name));
                }
            }
        }
        else
        {
            if (dirP->d_type == DT_REG)
            {
                if (full_path)
                {
                    files.push_back(path + string("/") + string(dirP->d_name));
                }
                else
                {
                    files.emplace_back(string(dirP->d_name));
                }
            }
        }
    }

    closedir(dp);
    return files;
}

vector<string> thor::os::list_files_recurse(string path, bool full_path)
{
    // return all files recursive
    vector<string> all_files = list_files(path, full_path);
    vector<string> all_dirs = list_dirs(path, full_path);
    for (auto d : all_dirs)
    {
        vector<string> n_res = list_files_recurse(d, full_path);
        all_files.insert(all_files.end(), n_res.begin(), n_res.end());
    }
    return all_files;
}

vector<string> thor::os::list_dirs(string path, bool full_path)
{
    assert(!path.empty());

#ifdef DEBUG
    cout << path << endl;
#endif // DEBUG

    DIR *dp;
    struct dirent *dirP;
    vector<string> files;
    if ((dp = opendir(&path[0u])) == NULL)
    {
        cout << "dir not exist." << endl;
    }

    while ((dirP = readdir(dp)) != NULL)
    {
        if (dirP->d_type == DT_DIR)
        {
            if (full_path)
            {
                files.push_back(path + string("/") + string(dirP->d_name));
            }
            else
            {
                files.push_back(string(dirP->d_name));
            }
#ifdef DEBUG
            cout << path << endl;
#endif // DEBUG
        }
    }

    closedir(dp);
    return files;
}

/**
 * list all files contains file and folders
 * @param path
 * @return
 */
vector<string> thor::os::list_all(string path, bool full_path)
{
    assert(!path.empty());
    DIR *dp;
    struct dirent *dirP;
    vector<string> files;
    if ((dp = opendir(&path[0u])) == NULL)
    {
        cout << "dir not exist." << endl;
    }

    while ((dirP = readdir(dp)) != NULL)
    {
        if (dirP->d_type == DT_REG)
        {
            if (full_path)
            {
                files.push_back(path + string("/") + string(dirP->d_name));
            }
            else
            {
                files.push_back(string(dirP->d_name));
            }
        }
        else if (dirP->d_type == DT_DIR)
        {
            if (full_path)
            {
                files.push_back(path + string("/") + string(dirP->d_name));
            }
            else
            {
                files.push_back(string(dirP->d_name));
            }
        }
    }

    closedir(dp);
    return files;
}

string thor::os::join(string path, string filename)
{
    assert(!path.empty());
    // path maybe /home/jin/doc1 or /home/jin/doc1/
    // make sure drop the last '/'
    string path_string = string(path);
    string joiner = "/";
#ifdef __WIN32
    joiner = "\\";
#endif

#ifdef _UNIX
    // unix machine
    joiner = "/";
#endif

#ifdef __APPLE__
    joiner = "/";
#endif
    vector<string> split_r;

    thor::str_util::SplitString(path_string, split_r, joiner);
    thor::str_util::StripString(split_r, "");
    string path_s = thor::str_util::join_str(joiner, split_r);
    string joined_path = path_string + joiner + filename;
    return joined_path;
}

string thor::os::parent_path(string path)
{
    assert(!path.empty());
    // get dir name of a path
    string joiner = "/";
#ifdef __WIN32
    joiner = "\\";
#endif

#ifdef __unix__
    // unix machine
    joiner = "/";
#endif

#ifdef __APPLE__
    joiner = "/";
#endif
    //    Log("get parent path, this is original " + (string) path);
    vector<string> split_r;
    thor::str_util::SplitString(path, split_r, joiner);
    vector<string> split_drop_file_name(split_r.begin(), split_r.end() - 1);
    string dir = thor::str_util::join_str(joiner, split_drop_file_name);
    //    LogInfo("to see is the parent path is right. " + dir);
    return dir;
}

/**
 * this method will return the filename of a path
 * @param path
 * @return
 */
string thor::os::filename(string path)
{
    assert(!path.empty());
    string path_str = path;
    if (thor::os::exists(path))
    {
        if (thor::os::isdir(path))
        {
            // if dir, return directly
            return path_str;
        }
        else
        {
            string joiner("/");
            string joiner_win("\\");

            std::size_t found = path_str.find(joiner);
            std::size_t found_win = path_str.find(joiner_win);

            if (found != std::string::npos)
            {
                // this is a path from unix
                vector<string> split_r;
                thor::str_util::SplitString(path_str, split_r, joiner);
                return split_r[split_r.size() - 1];
            }
            else if (found_win != std::string::npos)
            {
                // this is path from windows
                vector<string> split_r;
                thor::str_util::SplitString(path_str, split_r, joiner_win);
                return split_r[split_r.size() - 1];
            }
            else
            {
                // this is exactly a file
                return path_str;
            }
        }
    }
    else
    {
        cout << "can not found path or file: " + path_str << endl;
        return "";
    }
}

/**
 * this method will create a folder
 * @param path
 */
int thor::os::do_mkdir(string path, mode_t mode)
{
    assert(!path.empty());
    Stat st;
    int status = 0;
    if (stat(&path[0u], &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(&path[0u], mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }
    return status;
}

/**
 * this method will create a folder recursive
 * @param path
 */
int thor::os::makedirs(string path, mode_t mode)
{
    struct stat st;
    for (std::string::iterator iter = path.begin(); iter != path.end();)
    {
        std::string::iterator newIter = std::find(iter, path.end(), '/');
        std::string newPath =  "./" + std::string(path.begin(), newIter);

        if (stat(newPath.c_str(), &st) != 0)
        {
            if (mkdir(newPath.c_str(), mode) != 0 && errno != EEXIST)
            {
                std::cout << "cannot create folder [" << newPath << "] : " << strerror(errno) << std::endl;
                return -1;
            }
        }
        else if (!S_ISDIR(st.st_mode))
        {
            errno = ENOTDIR;
            std::cout << "path [" << newPath << "] not a dir " << std::endl;
            return -1;
        }
        else
            std::cout << "path [" << newPath << "] already exists " << std::endl;
        iter = newIter;
        if (newIter != path.end())
            ++iter;
    }
    return 0;
}

/**
 * Get the sufix of a file, if it is a file
 * other than return itself
 * @param path
 * @return
 */
string thor::os::suffix(string path)
{
    assert(!path.empty());
    string p = string(path);

    vector<string> split_result;
    thor::str_util::SplitString(path, split_result, ".");

    if (split_result.size() > 1)
    {
        return split_result.back();
    }
    else
    {
        // indicates this is path
        return p;
    }
}

////////////////// add some new APIs ///////////////////////
namespace thor
{
namespace os
{

string abs_path(const string &path)
{
    cerr << "abs_path not implement yet.";
}

std::string GetAbsolutePath(const std::string &prefix,
                            const std::string &relative_path)
{
    if (relative_path.empty())
    {
        return prefix;
    }
    // If prefix is empty or relative_path is already absolute.
    if (prefix.empty() || relative_path[0] == '/')
    {
        return relative_path;
    }

    if (prefix.back() == '/')
    {
        return prefix + relative_path;
    }
    return prefix + "/" + relative_path;
}

} // namespace os
} // namespace thor