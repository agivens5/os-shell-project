#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

// Helper for error messages
static string sysErr() { return string(strerror(errno)); }

// cd - Change directory
void cmd_cd(const vector<string>& args) {
    if (args.size() < 2) {
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd))) cout << cwd << endl;
        else cerr << "cd: " << sysErr() << endl;
        return;
    }
    if (chdir(args[1].c_str()) != 0) {
        cerr << "cd: " << args[1] << ": " << sysErr() << endl;
        return;
    }
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd))) setenv("PWD", cwd, 1);
}

// pwd - Print working directory
void cmd_pwd(const vector<string>& args) {
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd))) cout << cwd << endl;
    else cerr << "pwd: " << sysErr() << endl;
}

// ls - List directory contents
void cmd_ls(const vector<string>& args) {
    const string path = (args.size() >= 2) ? args[1] : ".";
    DIR* dir = opendir(path.c_str());
    if (!dir) { cerr << "ls: cannot access '" << path << "': " << sysErr() << endl; return; }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string name = entry->d_name;
        if (name[0] == '.') continue;
        string fullPath = path + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
            cout << name << "/" << endl;
        else
            cout << name << endl;
    }
    closedir(dir);
}

// mkdir - Create directories
void cmd_mkdir(const vector<string>& args) {
    if (args.size() < 2) { cerr << "mkdir: missing operand" << endl; return; }
    for (size_t i = 1; i < args.size(); i++) {
        if (mkdir(args[i].c_str(), 0755) != 0)
            cerr << "mkdir: cannot create '" << args[i] << "': " << sysErr() << endl;
    }
}

// rm - Remove files
void cmd_rm(const vector<string>& args) {
    if (args.size() < 2) { cerr << "rm: missing operand" << endl; return; }
    for (size_t i = 1; i < args.size(); i++) {
        if (remove(args[i].c_str()) != 0)
            cerr << "rm: cannot remove '" << args[i] << "': " << sysErr() << endl;
    }
}

// Tokenizer function
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

        // COMMANDS
        if (tokens[0] == "quit") {
            running = false;
        }
        else if (tokens[0] == "echo") {
            for (int i = 1; i < tokens.size(); i++) {
                cout << tokens[i] << " ";
            }
            cout << endl;
        }
        else if (tokens[0] == "clr") {
            cout << "\033[2J\033[H";
        }
        else if (tokens[0] == "touch") {
            for (int i = 1; i < tokens.size(); i++) {
                ofstream file(tokens[i]);
                file.close();
            }
        }
        else if (tokens[0] == "cat") {
            for (int i = 1; i < tokens.size(); i++) {
                ifstream file(tokens[i]);
                string line;

                if (!file) {
                    cout << "File not found\n";
                    continue;
                }

                while (getline(file, line)) {
                    cout << line << endl;
                }

                file.close();
            }
        }
 	else if (tokens[0] == "cd")    cmd_cd(tokens);
        else if (tokens[0] == "pwd")   cmd_pwd(tokens);
        else if (tokens[0] == "ls")    cmd_ls(tokens);
        else if (tokens[0] == "mkdir") cmd_mkdir(tokens);
        else if (tokens[0] == "rm")    cmd_rm(tokens);
        else {
            cout << "Unknown command" << endl;
        }
    }

    cout << "Shell exited." << endl;
    return 0;
}