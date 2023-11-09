// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "capture.hpp"

int main() {

    clear_terminal();

    // Generate new log file
    LogFile *log_file = new LogFile;

    cout << "curr_date_time: " << log_file->getCurrDateTime() << endl;

    WorkingFile *working_file = new WorkingFile;
    working_file->autoInitializeFiles();

    string user_input = captureUserString("Testing");
    cout << user_input << endl;

    return 0;

}
