#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

void logAction(const string &action) {
    ofstream log("activity_log.txt", ios::app);
    if (log.is_open()) {
        time_t now = time(0);
        char *dt = ctime(&now);
        string timeStr(dt);
        timeStr.pop_back();
        log << "[" << timeStr << "] " << action << endl;
    }
}

void listFiles() {
    cout << "\n📁 Files and Directories in: " << fs::current_path() << "\n";
    try {
        for (auto &entry : fs::directory_iterator(fs::current_path())) {
            if (entry.is_directory())
                cout << "[DIR]  " << entry.path().filename().string() << endl;
            else
                cout << "       " << entry.path().filename().string() << endl;
        }
        logAction("Listed files in " + fs::current_path().string());
    } catch (const fs::filesystem_error &e) {
        cerr << "Error listing files: " << e.what() << endl;
        logAction("Error listing files in " + fs::current_path().string());
    }
}

void changeDir(const string &folder) {
    try {
        fs::current_path(folder);
        cout << "📂 Changed directory to: " << fs::current_path() << endl;
        logAction("Changed directory to " + fs::current_path().string());
    } catch (const fs::filesystem_error &) {
        cout << "❌ Directory not found: " << folder << endl;
        logAction("Failed to change directory: " + folder);
    }
}

void createFile(const string &filename) {
    ofstream file(filename);
    if (file.is_open()) {
        cout << "✅ File created: " << filename << endl;
        logAction("Created file: " + filename);
    } else {
        cout << "❌ Error creating file.\n";
        logAction("Failed to create file: " + filename);
    }
}

void deleteFile(const string &filename) {
    try {
        if (fs::remove(filename)) {
            cout << "🗑️  File deleted: " << filename << endl;
            logAction("Deleted file: " + filename);
        } else {
            cout << "❌ File not found.\n";
            logAction("Failed to delete file: " + filename);
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error deleting file: " << e.what() << endl;
        logAction("Error deleting file: " + filename);
    }
}

void copyFile(const string &src, const string &dest) {
    try {
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
        cout << "📄 Copied: " << src << " → " << dest << endl;
        logAction("Copied file from " + src + " to " + dest);
    } catch (const fs::filesystem_error &e) {
        cout << "❌ Error copying file: " << e.what() << endl;
        logAction("Failed to copy from " + src + " to " + dest);
    }
}

void searchFile(const string &key) {
    cout << "\n🔍 Search results for \"" << key << "\":\n";
    bool found = false;
    try {
        for (auto &entry : fs::recursive_directory_iterator(fs::current_path())) {
            if (entry.path().filename().string().find(key) != string::npos) {
                cout << entry.path().string() << endl;
                found = true;
            }
        }
        if (!found) cout << "No files found.\n";
        logAction("Searched for: " + key);
    } catch (const fs::filesystem_error &e) {
        cerr << "Error searching files: " << e.what() << endl;
        logAction("Error searching for: " + key);
    }
}

void makeDir(const string &folder) {
    try {
        if (fs::create_directory(folder)) {
            cout << "📁 Directory created: " << folder << endl;
            logAction("Created directory: " + folder);
        } else {
            cout << "❌ Directory already exists or failed to create.\n";
            logAction("Failed to create directory: " + folder);
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error creating directory: " << e.what() << endl;
        logAction("Error creating directory: " + folder);
    }
}

void removeDir(const string &folder) {
    try {
        if (fs::remove_all(folder)) {
            cout << "🗑️  Directory deleted: " << folder << endl;
            logAction("Deleted directory: " + folder);
        } else {
            cout << "❌ Directory not found or could not be deleted.\n";
            logAction("Failed to delete directory: " + folder);
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Error deleting directory: " << e.what() << endl;
        logAction("Error deleting directory: " + folder);
    }
}

int main() {
    cout << "===== 🧠 SMART LINUX FILE EXPLORER =====\n";
    cout << "Current Directory: " << fs::current_path() << "\n";
    logAction("Program started.");

    string input, cmd, arg1, arg2;

    while (true) {
        cout << "\n>>> ";
        getline(cin, input);
        stringstream ss(input);
        ss >> cmd;
        getline(ss, arg1, ' ');
        getline(ss, arg1, ' ');
        getline(ss, arg2);

        arg1.erase(0, arg1.find_first_not_of(" "));
        arg2.erase(0, arg2.find_first_not_of(" "));

        if (cmd == "ls") listFiles();
        else if (cmd == "cd") changeDir(arg1.empty() ? "." : arg1);
        else if (cmd == "create") createFile(arg1);
        else if (cmd == "delete") deleteFile(arg1);
        else if (cmd == "copy") copyFile(arg1, arg2);
        else if (cmd == "search") searchFile(arg1);
        else if (cmd == "mkdir") makeDir(arg1);
        else if (cmd == "rmdir") removeDir(arg1);
        else if (cmd == "exit") {
            cout << "👋 Goodbye!\n";
            logAction("Program exited.");
            break;
        } else if (cmd.empty()) {
            continue;
        } else {
            cout << "❓ Invalid command. Try: ls, cd, create, delete, copy, search, mkdir, rmdir, exit\n";
        }
    }

    return 0;
}



