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

// Global base_path string var
const fs::path base_path = fs::canonical("../../UFLI-Lesson-JSON-test/");
// const fs::path base_path = fs::current_path();

// Log file name
const string log_file_name = "log_file.json";

// Name of non-real directory to initialize json file
const string empty_dir_name = "0000-00-00-00-00-00";

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

// Declare working paths
fs::path capture_log_file_path();

// Capture the log_file.json file
json capture_log_file(const fs::path log_file_path);

// Generate the input path
fs::path capture_input_dir(const json log_file);

// Generate the output dir
fs::path generate_output_dir(const json log_file);

// Update the log file
json update_log_file(json log_file, const string curr_date_time, const fs::path log_file_path);

// Import log_file json into vector<string>
vector<string> import_log_file_json(const json log_file);

// TO DO:
// Create log_file class object that has input and output commands

#endif
