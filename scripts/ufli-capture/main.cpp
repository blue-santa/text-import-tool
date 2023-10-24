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
    // input_path
    // output_path
    // lessonJson

    // Capture current date time
    string curr_date_time = generate_date_and_time();

    // Capture the input directory
    fs::path input_path = generate_input_path(base_path);

    // 

    // TO DO:
    // Convert to function. Add to class object.
    // Set output paths
    string output_dir_str = base_path.string() + "/" + curr_date_time + "/";
    fs::path output_dir = output_dir_str;

    cout << "output_dir: " << output_dir.string() << endl;

    // capture_page_num(lessonJson);

    // capture_lesson_num(lessonJson);

    // cout << lessonJson << endl;

    // TO DO:
    // Check that input_dir and output_dir are not equal
    // Also check that the log file has been updated

    return 0;

}
