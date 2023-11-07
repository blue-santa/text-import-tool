/*
 * builder.cpp
 * YKSD
 * Bryan Beus
 * Function file for UFLI_Lesson_Builder
 * Facilitates guided user input for the creation of UFLI Lessons converted to json output
 */

#include "capture.hpp"

// Global base_path string var
const fs::path base_path = fs::canonical("../../ufli-working-json/");

// Start the lowest file number
int lowest_file = 0;

// Start at the lowest page number
int lowest_page = 62;

// Start at the lowest lesson number 
int lowest_lesson = 1;

// The highest lesson number
int max_lesson = 128;

// List of lesson numbers that are split into sub-lessons
vector<int> sublesson_list = {};

// Clear terminal
void clear_terminal() {

    cout << "\033[2J\033[1;1H";

    return;
}

// Create a LogFile object
LogFile::LogFile() {
    // Set the current date/time
    setCurrDateTime();

    // Set the log_file_path variable
    setLogFilePath();

    // Initialize the log file
    initLogFile();

    return;
}

// Generate new string variable containing date/time
void LogFile::setCurrDateTime() {

    // Capture the current date/time using chrono
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert to string and store privately in class
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H-%M-%S");
    curr_date_time = ss.str();

    return;
}

// Return the curr_date_time variable
string LogFile::getCurrDateTime() {

    return curr_date_time;
}

// Set log_file path
void LogFile::setLogFilePath() {

    log_file_path = base_path / log_file_name;

    cout << "log_file_path: " << log_file_path << endl;

}

// Capture the log_file.json file
void LogFile::initLogFile() {

    // Test if log_file_name exists
    bool exists = fs::exists(log_file_path);

    // To Do:
    // New to set user and groups to protect host system

    // Check if log_file already exists
    // If it does not, create a dummy log_file
    if (!exists) {

        // Create a temp ofstream to create initial file
        ofstream temp_fout(log_file_path, ofstream::out);

        if (!temp_fout) {
            cerr << "Failed to create ofstream for " + log_file_name << "." << endl;
            throw exception();
        }

        // Create a temporary new json file with empty_dir_name as the first directory
        json temp_new_file;
        temp_new_file["timestamps"].push_back(empty_dir_name);

        // Write the new file into log_file_name file and close the file
        temp_fout << temp_new_file.dump(-1);
        temp_fout.close();

    } 

    // Read previous file into json variable
    ifstream fin(log_file_path);

    // Test opening
    if (!fin || fin.eof()) {
        cerr << "Log_file: " + log_file_name + " exists, but did not open correctly." << endl;
        throw exception();
    }

    // Parse the new file into json format
    log_file_json = json::parse(fin);
    
    // Close the file
    fin.close();

    // Add the new timestamp
    log_file_json["timestamps"].push_back(curr_date_time);

    // To Do:
    // Write out the new timestamps and file
    ofstream fout(log_file_path, ofstream::trunc);

    // Handle possible errors
    if (!fout) {
        cerr << "Failed to write log_file to file" << endl;
        throw exception();
    }

    // Write to file
    fout << log_file_json.dump(-1);

    return;

}

WorkingFile::WorkingFile() {

    // Initialize all lesson parameters
    curr_file_num = lowest_file;
    curr_page_num = lowest_page;
    curr_lesson_num = lowest_lesson;

    // Set the current file name
    setCurrentFileName(curr_file_num);

    // Set current file path
    setCurrentFilePath();

    // Test for each file and autogenerate those that do not yet exist
}

void WorkingFile::setCurrentFileName(const int curr_file_num) {

    // Convert curr_file_num to three digit string
    stringstream file_name_ss;
    file_name_ss << std::setw(3) << std::setfill('0') << curr_file_num;
    file_name_ss << "_ufli.json";

    curr_file_name = file_name_ss.str();

    cout << curr_file_name << endl;

}

void WorkingFile::setCurrentFilePath() {

    fs::path curr_file_path = base_path / curr_file_name;
}

// // Capture page_num for lessonJson
// void capture_page_num(json &lessonJson) {
// 
//     int page_num[2];
// 
//     clear_terminal();
// 
//     cout << "Enter the first page number, hit enter, then enter the second page number" << endl;
// 
//     cin >> page_num[0];
//     cin >> page_num[1];
// 
//     lessonJson["page_num"] = page_num;
// 
//     clear_terminal();
// 
//     return;
// 
// }
// 
// // Capture lesson_num for lessonJson
// void capture_lesson_num(json &lessonJson) {
// 
//     int lesson_num;
//     bool has_sub_number = false;
//     int sub_number = 0;
// 
//     cout << "Enter the lesson number?" << endl;
// 
//     cin >> lesson_num;
// 
//     clear_terminal();
// 
//     cout << "Is this lesson split into parts a and b? Type \"yes\" if true, or type \"no\" if false." << endl;
// 
//     string user_input = "";
// 
//     cin >> user_input;
// 
//     if (user_input == "yes") {
//         has_sub_number = true;
//     }
// 
//     if (has_sub_number) {
//         
//         cout << "Enter \"0\" if this is part a and \"1\" if this is part b: " << endl;
// 
//         cin >> sub_number;
// 
//     }
// 
//     lessonJson["lesson_num"]["number"] = lesson_num;
// 
//     lessonJson["lesson_num"]["sub_number"]["active"] = has_sub_number;
// 
//     lessonJson["lesson_num"]["sub_number"]["sub_number"] = sub_number;
// 
//     clear_terminal();
// 
//     return;
//     
// }

// // TO DO:
// // Function to manage log file
// 
// 
// // Generate new directory
// void generate_init_dir() {
// 
//     // To Do:
//     // Lift all initial directory creation here
// 
// }
// 
// // Generate the input path
// fs::path capture_input_dir(const json log_file) {
// 
//     // Declare working paths
//     fs::path input_dir;
// 
//     // Declare working strings
//     string input_dir_str;
// 
//     // Declare working vectors
//     vector<string> dir_names_vec = import_log_file_timestamps_json(log_file);
// 
//     input_dir_str = dir_names_vec.end()[-2];
//     input_dir = base_path / input_dir_str;
// 
//     return input_dir;
// }
// 
// // Generate the output dir
// fs::path generate_output_dir(const json log_file) {
// 
//     vector<string> dir_names_vec = import_log_file_timestamps_json(log_file);
// 
//     string output_dir_str = dir_names_vec.end()[-1];
// 
//     fs::path output_dir = base_path / output_dir_str;
// 
//     // Create the output directory
//     fs::create_directory(output_dir);
// 
//     return output_dir;
// 
// }
// 
// 
// // Update the log file
// json update_log_file_timestamp(const string curr_date_time) {
// 
//     fs::path log_file_path = log_file["log_file_path"];
// 
//     // Add new curr_date_time to log_file
//     log_file["timestamps"].push_back(curr_date_time);
// 
//     // Open file
//     ofstream fout(log_file_path, ofstream::trunc);
// 
//     if (!fout) {
//         cerr << "Failed to open fout: " + log_file_path.string() << endl;
//         throw exception();
//     }
// 
//     // Write the new_file json data to the log_file_path file
//     fout << log_file;
// 
//     // Close the file
//     fout.close();
// 
//     // To Do:
//     // Receive bool from function to update log file
//     // If bool is false, end program
//     // If bool is true:
//     // Create new directory
//     // Return old directory and new directory (as vector?)
// 
//     return log_file;
// 
// }
// 
// vector<string> import_log_file_timestamps_json(const json log_file) {
// 
//     vector<string> dir_names_vec;
// 
//     for (auto it = log_file["timestamps"].begin(); it != log_file["timestamps"].end(); it++) {
//         string expectsString{*it};
//         dir_names_vec.push_back(expectsString);
//     }
//     
//     return dir_names_vec;
// }
