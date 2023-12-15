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

            // Alert the user that there is an error in the current working file
            cerr << "Error loading file: " << working_file.returnCurrFileNum() << endl;;

            break;
        }

        // Process header_text
        bool success = false;
        while (!success) {

            // Process the header and determine whether to repeat while loop
            success = working_file.processHeader(log_file);

        }

        // Process lesson_title
        success = false;
        while(!success) {

            // Process the lesson title and determine whether to repeat while loop
            success = working_file.processLessonTitle(log_file);

        }

        // Process instructional_notes
        success = false;
        while (!success) {
            success = working_file.processInstructionalNotes(log_file);
        }

        // Process phonemic_awareness

        // Process phonological awareness

        // Process visual_drill

        // Process auditory_drill

        // Process blending_drill

        // Process new_concept

        // Process word_work

        // Process irregular words

        // Process connected text

        // Process decodable text

        // Process word work chains

        // Process high frequency words

        // Process word lists

        // Write the working file to disk
        // working_file.writeCurrentWorkingFile(log_file);

    }

    return 0;

}
