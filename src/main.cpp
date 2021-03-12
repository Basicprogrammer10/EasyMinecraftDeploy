#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include <windows.h>

void debugPrint(std::string text, int colorcode) {
    std::cout << "\x1B[" << colorcode << "m" << text << "\033[0m" << std::endl;
}

bool saveFile(std::string filePath, std::string content) {
    std::fstream file;
    file.open(filePath, std::ios::out);
    if (!file) {
        return false;
    }
    file << content;
    file.close();
    return true;
}

bool makeDir(const char *filePath) {
    int check;
    check = mkdir(filePath);
    if (check)
        return false;
    return true;
}

inline bool exists(const std::string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char **argv) {
    std::string serverUri = "https://launcher.mojang.com/v1/objects/1b557e7b033b583cd9f66746b7a9ab1ec1673ced/server.jar";
    std::string folder = "Server";
    DWORD l_mode;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdout, &l_mode);
    SetConsoleMode(hStdout, l_mode | 0x0004 | 0x0008);

    debugPrint("[*] Welcome to Basicprogrammer10's Easy Minecraft Deploy!\n", 32);

    if (argv[1] != nullptr)
        folder = argv[1];

    if (!exists("Server")) {
        debugPrint("[*] Creating folder \x1B[34m" + folder, 36);
        makeDir(folder.c_str());
        debugPrint("[*] Folder Created", 32);
        debugPrint("[*] Starting Download of Server.jar", 36);
        const std::string command = "curl " + serverUri + " -o " + folder + "/server.jar >nul 2>&1";
        int status = system(command.c_str());
        if (status != 0) {
            debugPrint("[*] Error Downloading \x1B[34mServer.jar", 31);
            return -1;
        }
        debugPrint("[*] Download Complete", 32);
        debugPrint("[*] Accepting EULA", 36);
        saveFile(folder + "/eula.txt",
                 "#Genarated by https://github.com/Basicprogrammer10/EasyMinecraftDeploy\neula=TRUE");
        debugPrint("[*] Complete", 32);
        return 0;
    }

    debugPrint("[*] Starting Server", 32);
    std::cout << "\x1B[35m";
    const std::string runCommand = "cd " + folder + " && java -jar server.jar --nogui";
    system(runCommand.c_str());

    std::cout << "\x1B[33m";
    system("pause");

    return 0;
}