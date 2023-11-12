// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "capture.hpp"

int main() {

    clear_terminal();

    // Generate new log file
    LogFile log_file;

    cout << "curr_date_time: " << log_file.getCurrDateTime() << endl;

    WorkingFile working_file;

    working_file.autoInitializeFiles(log_file);

    working_file.setActive(log_file);

    // string user_input = captureUserString("Testing");

    return 0;

}
