/*
 * builder.hpp
 * YKSD
 * Bryan Beus
 * Declaration file for UFLI_Lesson_Builder
 * Facilitates guided user input for the creation of UFLI Lessons converted to json output
 */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include "single_declarations.hpp"

// Clear terminal
int clear_terminal();

// Capture page_num for lessonJson
void capture_page_num(json &lessonJson);

// Capture lesson_num for lessonJson
void capture_lesson_num(json &lessonJson);

// Generate a string containing current date and time
string generate_curr_date_time();

// Generate new output directory
void generate_init_dir();

// To Do:
// Update log_file.json file with new curr_date_time data
bool update_log_file();

// Generate the input path
fs::path generate_input_dir(const string curr_date_time);

// Capture the log_file.json file
json capture_log_file();

// TO DO:
// Create log_file class object that has input and output commands

#endif
