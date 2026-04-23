#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

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
        else {
            cout << "Unknown command" << endl;
        }
    }

    cout << "Shell exited." << endl;
    return 0;
}