/*
 * single_declarations.hpp
 * YKSD
 * Bryan Beus
 * Single declaration file for UFLI_Lesson_Builder
 * Declares global variables that only need to be defined once
 */

#ifndef SINGLE_DEC_HPP
#define SINGLE_DEC_HPP

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
#endif
