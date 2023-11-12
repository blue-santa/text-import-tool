// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "capture.hpp"

int main() {

    clearTerminal();

    // Generate new log file
    LogFile log_file;

    WorkingFile working_file;

    working_file.autoInitializeFiles(log_file);

    working_file.setActive(log_file);

    // string user_input = captureUserString("Testing");

    return 0;

}
