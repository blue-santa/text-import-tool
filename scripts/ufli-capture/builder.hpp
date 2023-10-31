/*
 * builder.hpp
 * YKSD
 * Bryan Beus
 * Declaration file for UFLI_Lesson_Builder
 * Facilitates guided user input for the creation of UFLI Lessons converted to json output
 */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <string>               // string()
#include <sstream>              // stringstream()
#include <iostream>             // cout, cin, endl
#include <fstream>              // ifstream()
#include <chrono>               // now(), to_time_t()
#include <filesystem>           // path()
#include <fstream>              // fopen(), fclose()
#include <vector>               // vector<>

#include "nlohmann/json.hpp"    // json input/output


using std::string;
using std::istringstream;
using std::stringstream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::to_string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::exception;

using json = nlohmann::json;

namespace fs = std::filesystem;

// Clear terminal
int clear_terminal();

// Capture page_num for lessonJson
void capture_page_num(json &lessonJson);

// Capture lesson_num for lessonJson
void capture_lesson_num(json &lessonJson);

// Generate a string containing current date and time
string generate_date_and_time();

// Generate new output directory
void generate_init_dir();

// To Do:
// Update log_file.json file with new curr_date_time data
bool update_log_file();

// Generate the input path
fs::path generate_input_dir(const fs::path base_path, const string curr_date_time);

// TO DO:
// Create log_file class object that has input and output commands

#endif
