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

        // Process header_text
        working_file.processHeader();

        // Process lesson_title
        // working_file.processLessonTitle();

        // Process instructional_notes

        // Process phonemic_awareness

        // Process phonological awareness

        // Process visual_drill

        // Process auditory_drill

        // Process blending_drill

        // Process new_concept

        // Process word_work

        // Process irregular words

        // Process connected text

        // Write the working file to disk
        working_file.writeCurrentWorkingFile(log_file);

        // Pause for user to verify that all proceeded as planned
        string prompt = "Please verify that the most recent file is correct on disk";
        captureUserString(prompt);

    }

    return 0;

}
