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

    // string capture_prev_dir_path();
    // Capture the input directory
    fs::path input_path = generate_input_path(base_path);

   // Set input and output paths
   string output_dir_str = base_path.string() + curr_date_time + "/";
   fs::path output_dir = output_dir_str;

   cout << output_dir.string() << endl;

    // capture_page_num(lessonJson);

    // capture_lesson_num(lessonJson);

    // cout << lessonJson << endl;

    return 0;

}
