/*
 * builder.hpp
 * YKSD
 * Bryan Beus
 * Declaration file for UFLI_Lesson_Builder
 * Facilitates guided user input for the creation of UFLI Lessons converted to json output
 */

#ifndef BUILDER_HPP
#define BUILDER_HPP

// Standard libraries
#include <string>               // string()
#include <sstream>              // stringstream()
#include <iostream>             // cout, cin, endl
#include <fstream>              // ifstream()
#include <filesystem>           // path()
#include <fstream>              // fopen(), fclose()
#include <vector>               // vector<>

// Downstream libraries
#include <iomanip>              // setw(), setfill()
#include <typeinfo>             // typeid
#include <chrono>               // now(), to_time_t()

// Libraries from local include dir
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
void clear_terminal();

// Manages content related to keeping log records, including those stored to disk
class LogFile {

    private:

        // Log file name
        const string log_file_name = "log_file.json";

        // Name of non-real directory to initialize json file
        const string empty_dir_name = "0000-00-00-00-00-00";

        // Current date/time
        string curr_date_time;

        // Declare working paths
        fs::path log_file_path;

        // Most recent file and key information
        json most_recent;

        // Capture the log_file.json file
        void initLogFile();

        // Set the log_file_path var
        void setLogFilePath();

    public:

        // Public member variables
        // The log_file
        json log_file_json;

        // Initialization process
        LogFile();

        // Capture date/time of initialization
        void setCurrDateTime();

        // Print curr_date_time
        string getCurrDateTime();

        // Open the log_file for processing
        json openLogFile();

        // Write current log_file to disk
        bool writeLogFile(const json &log_file_json);

        // Set the most recent file path and key
        bool setMostRecent(const fs::path &most_recent_path, const string &most_recent_str);

        // To Do:
        // Build a json file that contains all directory names

};

// Manages the ufli .json file that is in process 
class WorkingFile {

    private:

        // Current file number
        int curr_file_num;

        // Current page number
        int curr_page_num;

        // Current lesson number;
        int curr_lesson_num;

        // Current file name
        string curr_file_name;

        // Current file relative path
        fs::path curr_file_path;

        // Json model of the current file
        json working_file;

        // Whether the current file has reached the last sublesson
        bool curr_file_last_sublesson = true;

        // Current position in sublesson printout
        int curr_position_sublesson_array = 0;

        // Number of sublessons in current file
        int num_sublessons = 1;

    public:

        // Create a new WorkingFile class object
        WorkingFile();

        // Set the current file name
        void setCurrentFileName(const int curr_file_num);

        // Set the current file path
        void setCurrentFilePath();

        // Auto-generate any and all uncreated files
        bool autoInitializeFiles(LogFile &log_file);

};

// 
class SublessonList {

    private:

        // Working variables
        json sublesson_list;
        string sublesson_file_name = "sublesson_list.json";
        fs::path path_to_sublesson_file;

    public:

        // Create a new SublessonList class object
        SublessonList();

        // Destructor
        ~SublessonList();

        // Retrieve the sublesson .json file
        void setList();

        // Check if input lesson number is in lesson list
        json checkLessonNum(const int &curr_number);

};

// Capture string input from user
string captureUserString(const string prompt);


// // Capture lesson_num for lessonJson
// void capture_lesson_num(json &lessonJson);
// 
// // Generate new output directory
// void generate_init_dir();
// 
// // Generate the input path
// fs::path capture_input_dir(const json log_file);
// 
// // Generate the output dir
// fs::path generate_output_dir(const json log_file);

// TO DO:
// Create log_file class object that has input and output commands

#endif
