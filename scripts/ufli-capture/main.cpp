// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "builder.hpp"
#include "single_declarations.hpp"

int main() {

    clear_terminal();

    json lessonJson;

    // Capture current date time
    string curr_date_time = generate_date_and_time();

    // Set input and output paths
    string output_path = base_path + curr_date_time + "/";

    cout << output_path << endl;

    // Capture the input directory
    string input_path = generate_input_path(base_path);

    // string capture_prev_dir_path();


    // capture_page_num(lessonJson);

    // capture_lesson_num(lessonJson);

    // cout << lessonJson << endl;

    return 0;

}
