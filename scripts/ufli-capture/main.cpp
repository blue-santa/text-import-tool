// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "builder.hpp"
#include "single_declarations.hpp"

int main() {

    clear_terminal();

    json lessonJson;

    // TO DO:
    // Create a class object that contains event variables, including
    // curr_date time
    // input_dir
    // output_dir
    // lessonJson

    // Capture current date time
    string curr_date_time = generate_date_and_time();

    // TO DO:
    // Convert to function. Add to class object.
    // Set output paths
    string output_dir_str = base_path.string() + "/" + curr_date_time + "/";
    fs::path output_dir = output_dir_str;

    cout << "output_dir: " << output_dir.string() << endl;

    // Capture the input directory
    fs::path input_dir = generate_input_dir(base_path, curr_date_time);

    if (input_dir == base_path) {
        cerr << "Failed to load log_file.json" << endl;
        return 0;
    }

    // 

    // capture_page_num(lessonJson);

    // capture_lesson_num(lessonJson);

    // cout << lessonJson << endl;

    // TO DO:
    // Check that input_dir and output_dir are not equal
    // Also check that the log file has been updated

    return 0;

}
