// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "capture.hpp"

int main() {

    clearTerminal();

    // Generate new log file
    LogFile log_file;

    // Declare new working file
    WorkingFile working_file;

    // Test if any files are missing and, if so, auto generate them
    working_file.autoInitializeFiles(log_file);

    // Process files until the range is met or user interrupts
    while (true) {

        // Load to the next working file
        if (!working_file.loadNextWorkingFile(log_file)) {
            break;
        }

        // Process the next element
        working_file.processNewElement(log_file);

        // Pause for user to verify that all proceeded as planned
        string prompt = "Please verify that the most recent file is correct on disk";
        captureUserString(prompt);

    }

    return 0;

}
