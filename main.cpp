#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

// Virtual File System class
class VirtualFileSystem {
    fs::path currentDirectory; // To track the current directory

public:
    // Constructor: Set the starting directory
    VirtualFileSystem() {
        currentDirectory = fs::current_path();
        cout << "Starting directory: " << currentDirectory << endl;
    }

    // Change to another directory or go to parent ("..")
    void changeDirectory(const string& path) {
        fs::path newPath = (path == "..") ? currentDirectory.parent_path() : currentDirectory / path;

        if (fs::is_directory(newPath)) {
            currentDirectory = newPath;
            cout << "Directory changed to: " << currentDirectory << endl;
        } else {
            cout << "Error: Directory not found -> " << path << endl;
        }
    }

    // Create a new file
    void createFile(const string& fileName) {
        ofstream file(currentDirectory / fileName);
        if (file) {
            cout << "File created: " << fileName << endl;
            file.close();
        } else {
            cout << "Error: Could not create file -> " << fileName << endl;
        }
    }

    // Create a new directory
    void createDirectory(const string& dirName) {
        if (fs::create_directory(currentDirectory / dirName)) {
            cout << "Directory created: " << dirName << endl;
        } else {
            cout << "Error: Could not create directory -> " << dirName << endl;
        }
    }

    // Delete a file or directory
    void deleteEntry(const string& name) {
        fs::path targetPath = currentDirectory / name;
        if (fs::exists(targetPath)) {
            fs::remove_all(targetPath);
            cout << "Deleted: " << name << endl;
        } else {
            cout << "Error: No such file or directory -> " << name << endl;
        }
    }

    // Rename a file or directory
    void renameEntry(const string& oldName, const string& newName) {
        fs::path oldPath = currentDirectory / oldName;
        fs::path newPath = currentDirectory / newName;

        if (fs::exists(oldPath)) {
            fs::rename(oldPath, newPath);
            cout << "Renamed '" << oldName << "' to '" << newName << "'" << endl;
        } else {
            cout << "Error: No such file or directory -> " << oldName << endl;
        }
    }

    // Move a file or directory to a new location
    void moveEntry(const string& source, const string& destination) {
        fs::path sourcePath = currentDirectory / source;
        fs::path destinationPath = currentDirectory / destination;

        if (fs::exists(sourcePath)) {
            fs::rename(sourcePath, destinationPath);
            cout << "Moved '" << source << "' to '" << destination << "'" << endl;
        } else {
            cout << "Error: Source not found -> " << source << endl;
        }
    }

    // Write content to a file
    void writeFile(const string& fileName, const string& content) {
        ofstream file(currentDirectory / fileName);
        if (file) {
            file << content;
            cout << "Content written to file: " << fileName << endl;
            file.close();
        } else {
            cout << "Error: File not found -> " << fileName << endl;
        }
    }

    // Read content from a file
    void readFile(const string& fileName) {
        ifstream file(currentDirectory / fileName);
        if (file) {
            string line;
            cout << "Content of " << fileName << ":\n";
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else {
            cout << "Error: File not found -> " << fileName << endl;
        }
    }

    // List files and directories in the current directory with size
    void listEntries() {
        cout << "Entries in " << currentDirectory << ":\n";
        for (const auto& entry : fs::directory_iterator(currentDirectory)) {
            if (fs::is_directory(entry)) {
                cout << "Dir  " << entry.path().filename().string() << endl;
            } else {
                cout << "File " << entry.path().filename().string()
                     << " (Size: " << fs::file_size(entry) << " bytes)" << endl;
            }
        }
    }

    // Show free space on the current drive
    void showFreeSpace() {
        auto space = fs::space(currentDirectory);
        cout << "Free space: " << space.available / (1024 * 1024) << " MB" << endl;
    }
};

// Command-line interface
int main() {
    VirtualFileSystem vfs;
    string command, name, name2, content;

    cout << "Virtual File System\n";
    cout << "Commands: mkdir <dir>, touch <file>, ls, cd <dir>, rm <entry>, rename <old> <new>, move <src> <dest>, write <file> <content>, read <file>, free, exit\n";

    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "mkdir") {
            cin >> name;
            vfs.createDirectory(name);
        } else if (command == "touch") {
            cin >> name;
            vfs.createFile(name);
        } else if (command == "ls") {
            vfs.listEntries();
        } else if (command == "cd") {
            cin >> name;
            vfs.changeDirectory(name);
        } else if (command == "rm") {
            cin >> name;
            vfs.deleteEntry(name);
        } else if (command == "rename") {
            cin >> name >> name2;
            vfs.renameEntry(name, name2);
        } else if (command == "move") {
            cin >> name >> name2;
            vfs.moveEntry(name, name2);
        } else if (command == "write") {
            cin >> name;
            getline(cin, content); // Capture the remaining input
            vfs.writeFile(name, content);
        } else if (command == "read") {
            cin >> name;
            vfs.readFile(name);
        } else if (command == "free") {
            vfs.showFreeSpace();
        } else if (command == "exit") {
            cout << "Are you sure you want to exit? (yes/no): ";
            string confirm;
            cin >> confirm;
            if (confirm == "yes") {
                cout << "Exiting Virtual File System. Goodbye!" << endl;
                break;
            }
        } else {
            cout << "Unknown command. Try again." << endl;
        }
    }

    return 0;
}
