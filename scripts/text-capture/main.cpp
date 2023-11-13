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

    // To Do: loadNextWorkingFile should be separate from checking if we need to reset or pick up where we left off

    while (true) {

        if (!working_file.loadNextWorkingFile(log_file)) {
            break;
        }

        working_file.processNewElement(log_file);

        // Pause for user to verify that all proceeded as planned

    }

    return 0;

}
