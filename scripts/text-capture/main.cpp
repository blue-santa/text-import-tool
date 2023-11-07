// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "builder.hpp"

int main() {

    clear_terminal();

    // Generate new log file
    LogFile *log_file = new LogFile;

    json lessonJson;

    // To Do:
    // Generalize everything so that this can be applied to more curriculums than just UFLI

    // TO DO:
    // Create a class object that contains event variables, including
    // curr_date time
    // input_dir
    // output_dir
    // lessonJson

    // Capture current date time
    // string curr_date_time = generate_curr_date_time();

    // // To Do:
    // // Need input/output class object

    // // To Do:
    // // Log file object
    // // - Auto generate new log file
    // // - call log_file.set_paths
    // //   - This manages everything with getting the old paths and creating the new

    // // To Do:
    // // Pass in log_file_path
    // json log_file = capture_log_file();

    // // Update log_file

    // // Capture the input directory
    // fs::path input_dir = capture_input_dir(log_file);

    // if (input_dir == base_path) {
    //     cerr << "Failed to load log_file.json" << endl;
    //     return 0;
    // }

    // // Generate output_dir
    // fs::path output_dir = generate_output_dir(log_file);

    // // 

    // // capture_page_num(lessonJson);

    // // capture_lesson_num(lessonJson);

    // // cout << lessonJson << endl;

    // // TO DO:
    // // Check that input_dir and output_dir are not equal
    // // Also check that the log file has been updated

    return 0;

}
