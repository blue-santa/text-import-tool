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
#include <ctype.h>              // isdigit()
#include <stdlib.h>             // atoi()

// Libraries from local include dir
#include "nlohmann/json.hpp"    // json input/output

using std::string;
using std::istringstream;
using std::stringstream;
using std::cin;
using std::cout;
using std::cerr;
using std::boolalpha;
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
void clearTerminal();

// Capture string input from user
string captureUserString(const string prompt);

// General function to open json files and return the content as a json object
json openJsonFile(fs::path file_path);

// Write a generic json file in pretty print
bool writeJsonFilePrettyPrint(const json &json_file);

// Manage content related to keeping log records, including those stored to disk
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

        // Public member variables
        // The log_file
        json log_file_json;

        // Capture the log_file.json file
        void initLogFile();

        // Set the log_file_path var
        void setLogFilePath();

    public:

        // Initialization process
        LogFile();

        // Capture date/time of initialization
        void setCurrDateTime();

        // Set the most recent file path and key
        bool setMostRecent(const fs::path &most_recent_path);

        // Load the most recent information
        bool loadMostRecent();

        // Retrieve the most recent information
        json getMostRecentExtended();

        // Print curr_date_time
        string getCurrDateTime();

        // Open the log_file for processing
        json openLogFile();

        // Write current log_file to disk
        bool writeLogFile(const json &log_file_json);

};

// Manage a ufli .json file
class WorkingFile {

    private:

        // Current file number
        int curr_file_num;

        // Current file name
        string curr_file_name;

        // Current file relative path
        fs::path curr_file_path;

        // Current page number
        // To Do: Something is wrong about having this here
        // -- Should just be auto loaded or auto saved, not stored in the class
        // -- because it needs to be updated, but shouldn't be managed
        // -- beyond simply loading it from the existing file
        int curr_page_num;

        // Current lesson number;
        // To Do: Something is wrong about having this here
        // -- Should just be auto loaded or auto saved, not stored in the class
        // -- because it needs to be updated, but shouldn't be managed
        // -- beyond simply loading it from the existing file
        int curr_lesson_num;

        // Json model of the current file
        json working_file;

        // Whether the current file has reached the last sublesson
        // To Do: Something is wrong about having this here
        // -- Should be somewhere else, maybe stored in the autoInitializeFiles... func
        bool curr_file_last_sublesson = true;

        // Current position in sublesson printout
        // To Do: Something is wrong about having this here
        // -- Should be somewhere else, maybe stored in the autoInitializeFiles... func
        int curr_position_sublesson_array = 0;

        // Number of sublessons in current file
        // To Do: Something is wrong about having this here
        // -- Should be somewhere else, maybe stored in the autoInitializeFiles... func
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

        // Set the active file and associated variables
        bool loadNextWorkingFile(LogFile &log_file);

        // Set the working_file variable
        bool setWorkingFile();

        // Test whether the provided highest_key exists in working_file
        bool testHighestKey(const string &highest_key);

        // Question the user whether the result is acceptable
        bool queryUserApprovalWorkingElement(const string &highest_key, const json &working_element);

        // Process the header element in working_file
        bool processHeader(LogFile &log_file);

        // Process the title element in working_file
        bool processLessonTitle(LogFile &log_file);

        // Process instructional notes element in working_file
        bool processInstructionalNotes(LogFile &log_file);

        // Process phonemic awareness element in working_file
        bool processPhonemicAwareness(LogFile &log_file);

        // Process phonological awareness element in working_file
        bool processPhonologicalAwareness(LogFile &log_file);

        // Write the current working_file h
        bool writeCurrentWorkingFile(LogFile &log_file);

        // Return the current lesson number
        int returnCurrFileNum();
};

// Manages the list of sublessons
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

#endif
