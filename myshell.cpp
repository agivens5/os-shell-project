#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits.h>
#include <pwd.h>
#include <grp.h>

extern char **environ;

using namespace std;

// Splits user input into command + arguments
vector<string> tokenize(string line) {
    vector<string> tokens;
    string word;
    stringstream ss(line);

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

int main() {
    bool running = true;
    string line;

    cout << "Basic OS Shell Started" << endl;

    while (running) {
        cout << "myshell> ";
        getline(cin, line);

        vector<string> tokens = tokenize(line);

        if (tokens.size() == 0) {
            continue;
        }

        // quit command
        if (tokens[0] == "quit") {
            running = false;
        }

        // echo command
        else if (tokens[0] == "echo") {
            for (int i = 1; i < tokens.size(); i++) {
                cout << tokens[i];
                if (i < tokens.size() - 1) {
                    cout << " ";
                }
            }
            cout << endl;
        }

        // clr command
        else if (tokens[0] == "clr") {
            cout << "\033[2J\033[H";
        }

        // help command
        else if (tokens[0] == "help") {
            cout << "Basic OS Shell Help Manual" << endl;
            cout << "Available commands:" << endl;
            cout << "  quit              - Exit the shell" << endl;
            cout << "  echo [text]       - Display text on the screen" << endl;
            cout << "  clr               - Clear the screen" << endl;
            cout << "  touch [file]      - Create an empty file" << endl;
            cout << "  cat [file]        - Display file contents" << endl;
            cout << "  cp [src] [dest]   - Copy a file" << endl;
            cout << "  cd [path]         - Change directory" << endl;
            cout << "  pwd               - Print current directory" << endl;
            cout << "  ls [path]         - List directory contents" << endl;
            cout << "  mkdir [dir]       - Create a directory" << endl;
            cout << "  rm [file]         - Remove a file" << endl;
            cout << endl;
        }

        // pwd command
        else if (tokens[0] == "pwd") {
            char cwd[PATH_MAX];

            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                cout << cwd << endl;
            }
            else {
                cout << "Error getting current directory" << endl;
            }
        }

        // cd command
        else if (tokens[0] == "cd") {
            if (tokens.size() == 1) {
                char cwd[PATH_MAX];

                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    cout << cwd << endl;
                }
                else {
                    cout << "Error getting current directory" << endl;
                }
            }
            else {
                if (chdir(tokens[1].c_str()) != 0) {
                    cout << "Directory not found" << endl;
                }
                else {
                    char cwd[PATH_MAX];

                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        setenv("PWD", cwd, 1);
                    }
                }
            }
        }

        // ls command
        else if (tokens[0] == "ls") {
            string path = ".";

            if (tokens.size() > 1) {
                path = tokens[1];
            }

            DIR* directory = opendir(path.c_str());

            if (directory == NULL) {
                cout << "Could not open directory" << endl;
            }
            else {
                struct dirent* entry;

                while ((entry = readdir(directory)) != NULL) {
                    cout << entry->d_name << endl;
                }

                closedir(directory);
            }
        }

        // mkdir command
        else if (tokens[0] == "mkdir") {
            if (tokens.size() < 2) {
                cout << "Usage: mkdir directory_name" << endl;
            }
            else {
                for (int i = 1; i < tokens.size(); i++) {
                    if (mkdir(tokens[i].c_str(), 0755) != 0) {
                        cout << "Could not create directory: " << tokens[i] << endl;
                    }
                }
            }
        }

        // rm command
        else if (tokens[0] == "rm") {
            if (tokens.size() < 2) {
                cout << "Usage: rm file_name" << endl;
            }
            else {
                for (int i = 1; i < tokens.size(); i++) {
                    if (remove(tokens[i].c_str()) != 0) {
                        cout << "Could not remove file: " << tokens[i] << endl;
                    }
                }
            }
        }

        // touch command
        else if (tokens[0] == "touch") {
            if (tokens.size() < 2) {
                cout << "Usage: touch file_name" << endl;
            }
            else {
                for (int i = 1; i < tokens.size(); i++) {
                    ofstream file(tokens[i], ios::app);

                    if (!file) {
                        cout << "Could not create file: " << tokens[i] << endl;
                    }

                    file.close();
                }
            }
        }

        // cat command
        else if (tokens[0] == "cat") {
            if (tokens.size() < 2) {
                cout << "Usage: cat file_name" << endl;
            }
            else {
                for (int i = 1; i < tokens.size(); i++) {
                    ifstream file(tokens[i]);
                    string fileLine;

                    if (!file) {
                        cout << "File not found: " << tokens[i] << endl;
                        continue;
                    }

                    while (getline(file, fileLine)) {
                        cout << fileLine << endl;
                    }

                    file.close();
                }
            }
        }

        // cp command
        else if (tokens[0] == "cp") {
            if (tokens.size() != 3) {
                cout << "Usage: cp source destination" << endl;
            }
            else {
                ifstream sourceFile(tokens[1]);
                ofstream destFile(tokens[2]);

                if (!sourceFile) {
                    cout << "Source file not found" << endl;
                }
                else if (!destFile) {
                    cout << "Could not create destination file" << endl;
                }
                else {
                    string fileLine;

                    while (getline(sourceFile, fileLine)) {
                        destFile << fileLine << endl;
                    }
                }

                sourceFile.close();
                destFile.close();
            }
        }

        //mv command
        else if (tokens[0] == "mv") {


                    if (tokens.size() != 3) {
                        cout << "Usage: mv source destination" << endl;
                    }
                    else {
                        if (rename(tokens[1].c_str(), tokens[2].c_str()) != 0) {
                            cout << "Could not move file/rename file" << endl;
                        }
                    }
                }

        //wc command
        else if (tokens[0] == "wc") {
            if (tokens.size() != 2) {
                cout << "Usage: wc file_name" << endl;
            }
            else {
                ifstream file(tokens[1]);

                

                if (!file) {
                    cout << "File not found: " << tokens[1] << endl;
                }
                else {
                    string word;
                    string fileLine;
                    int lines = 0;
                    int words = 0;
                    int characters = 0;
                    while (getline(file, fileLine)) {
                        lines++;
                        characters += fileLine.length() + 1;

                        stringstream ss(fileLine);
                        while (ss >> word) {
                            words++;
                        }
                    }

                cout << "Lines: " << lines << endl;
                cout << "Words: " << words << endl;
                cout << "Characters: " << characters << endl;


                file.close();
            }
        }
    }

        // rmdir command
        else if (tokens[0] == "rmdir") {
            if (tokens.size() < 2) {
                cout << "Usage: rmdir directory_name" << endl;
            } else {
                for (int i = 1; i < tokens.size(); i++) {
                    if (rmdir(tokens[i].c_str()) != 0) {
                        cout << "Could not remove directory: " << tokens[i] << endl;
                    }
                }
            }
        }

        // dir command
        else if (tokens[0] == "dir") {
            string path = ".";
            if (tokens.size() > 1) path = tokens[1];
            DIR* directory = opendir(path.c_str());
            if (directory == NULL) {
                cout << "Could not open directory" << endl;
            } else {
                struct dirent* entry;
                while ((entry = readdir(directory)) != NULL) {
                    string name = entry->d_name;
                    if (name[0] == '.') continue;
                    string fullPath = path + "/" + name;
                    struct stat st;
                    if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
                        cout << name << "/" << endl;
                    else
                        cout << name << endl;
                }
                closedir(directory);
            }
        }

        // chmod command
        else if (tokens[0] == "chmod") {
            if (tokens.size() != 3) {
                cout << "Usage: chmod permissions file_name" << endl;
            } else {
                int mode = stoi(tokens[1], nullptr, 8);
                if (chmod(tokens[2].c_str(), mode) != 0) {
                    cout << "Could not change permissions: " << tokens[2] << endl;
                }
            }
        }

        // chown command
        else if (tokens[0] == "chown") {
            if (tokens.size() != 3) {
                cout << "Usage: chown owner file_name" << endl;
            } else {
                struct passwd* pw = getpwnam(tokens[1].c_str());
                if (pw == NULL) {
                    cout << "User not found: " << tokens[1] << endl;
                } else {
                    if (chown(tokens[2].c_str(), pw->pw_uid, -1) != 0) {
                        cout << "Could not change owner: " << tokens[2] << endl;
                    }
                }
            }
        }

        // grep command
        else if (tokens[0] == "grep") {
            if (tokens.size() != 3) {
                cout << "Usage: grep pattern file_name" << endl;
            } else {
                ifstream file(tokens[2]);
                if (!file) {
                    cout << "File not found: " << tokens[2] << endl;
                } else {
                    string fileLine;
                    while (getline(file, fileLine)) {
                        if (fileLine.find(tokens[1]) != string::npos) {
                            cout << fileLine << endl;
                        }
                    }
                    file.close();
                }
            }
        }

        //environ command
        else if (tokens[0] == "environ") {
            int i  = 0;

            while (environ[i] != NULL) {
                cout << environ[i] << endl;
                i++;
            }
        }
        
        // unknown command
        else {
            cout << "Unknown command" << endl;
        }
    }

    cout << "Shell exited." << endl;
    return 0;
}