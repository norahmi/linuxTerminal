#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

    void printCurrentPath(fs::path path){
        std::cout << "\n\n-->[" << path.string() << "]\n$";
        return;
    }

    // show the current working dir
    void pwd(fs::path currentPath){
        std::cout << currentPath.string();
        printCurrentPath(currentPath);
        return;
    }

    // lists directory's content
    void ls(fs::path currentPath){
        int counter = 0;
        for (const auto & entry : fs::directory_iterator(currentPath)){
            std::cout << entry.path().filename().string();
            counter++;
            counter % 4 == 0 ? std::cout << "\n" : std::cout << "\t\t";
        }
        printCurrentPath(currentPath);
        return;
    }

    // changes working directory
    fs::path cd(std::string directoryName, fs::path currentPath) {
        if(directoryName == "..")
            currentPath = currentPath.parent_path();

        else{
            fs::directory_entry newEntry{currentPath.string() + "/" + directoryName};
            if(newEntry.is_directory())
                currentPath = currentPath.append(directoryName);
            else std::cout << "Not a directory";
        }

        if(!fs::exists(currentPath)) std::cout << "No such file or directory"; 
        printCurrentPath(currentPath.string()); 
        return currentPath;
    }

    bool nameInUse(std::string name, fs::path currentPath){
        for (const auto & entry : fs::directory_iterator(currentPath)){
            if(entry.path().filename().string() == name) {
                return true;
            }
        }
        return false;
    }

    void mkdir(std::string dirName, fs::path currentPath){
        fs::path temp = currentPath;
        if(nameInUse(dirName, currentPath))
            std::cout << "cannot create directory : Name already in use";
        else 
            fs::create_directory(currentPath.append(dirName)) ? std::cout << "Directory created" : std::cout << "An error occured";
        printCurrentPath(temp);
        return;
    }

    void rm(std::string name, fs::path currPath, bool isFile){
        if(fs::exists(currPath.append(name))){
            uintmax_t n{fs::remove_all(currPath)};
            isFile ? std::cout << "Deleted " << n << " files" : std::cout << "Deleted " << n << " directories";
            printCurrentPath(currPath.parent_path());
            return;
        }

        std::cout << "No such file or directory";
        printCurrentPath(currPath);
        return;
    }

    void createNewfile(std::string fileName, fs::path currPath){
        if(nameInUse(fileName, currPath)){
            std::cout << "cannot create directory : Name already in use";
            printCurrentPath(currPath);
            return;
        }
        std::fstream file;
        file.open(currPath.append(fileName).string(), std::ios::out);
        !file ? std::cout << "An error occured while creating the file" : 
                std::cout << "File " << fileName << " was successfully created.";
        file.close();
        printCurrentPath(currPath.parent_path());
        return;
    }

    void copy(std::string fileToCopy, fs::path currPath){
        std::string newFileName;
        std::cin >> newFileName;
        fs::path path = currPath;

        if(nameInUse(fileToCopy, currPath)){
            if(!nameInUse(newFileName, currPath)){
                fs::path pathBase = currPath;
                std::filesystem::copy(currPath.append(fileToCopy), pathBase.append(newFileName), 
                                        std::filesystem::copy_options::recursive);
                std::cout << "successfully copied";
            }
            else
                std::cout << "cannot copy with name: file already exists";
        }
        else std::cout << "no such file or directory";
        printCurrentPath(path);
        return;
    }

    int main()
    {
        fs::path currentPath = fs::current_path().parent_path().parent_path().append("workingdir");
        std::cout << "Welcome to MyConsole version 1\nEnter 'q' to exit\n";
        printCurrentPath(currentPath);
        std::string input;
        std::cin >> input;

        while(input != "q"){

            if(input == "pwd")
                pwd(currentPath);
            else if(input == "ls")
                ls(currentPath);
            else if(input == "cd"){
                std::cin >> input;
                currentPath = cd(input, currentPath);}
            else if(input == "mkdir"){
                std::cin >> input;
                mkdir(input, currentPath);
            }
            else if(input == "rmdir"){
                std::cin >> input;
                rm(input, currentPath, false);
            }
            else if(input == "rm"){
                std::cin >> input;
                rm(input, currentPath, true);
            }
            else if(input == "touch"){
                std::cin >> input;
                createNewfile(input, currentPath);
            }
            else if(input == "cp"){
                std::cin >> input;
                copy(input, currentPath);
            }
            else if(input == "tree"){
                static_cast<void>(std::system("tree"));
                printCurrentPath(currentPath);
            }
            std::cin >> input;
            std::cout << "\n";
            // fs::directory_entry currEntry{currentPath};
            // currEntry.refresh();
        }

        return 0;

    }