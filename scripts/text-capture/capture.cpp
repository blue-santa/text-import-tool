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

// File at which to begin processing
int lower_file_range = 0;

// Upper value of file processing range
int upper_file_range = 128;

// Start the lowest file number
int lowest_file = 0;

// Start at the lowest page number
int lowest_page = 62;

// Start at the lowest lesson number 
int lowest_lesson = 1;

// The highest lesson number
int max_lesson = 128;

// Clear terminal
void clear_terminal() {

    // Code to clear screen
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

    // To Do:
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

    try {

        curr_file_path = base_path / curr_file_name;

    } catch (...) {

        cerr << "Failed to create curr_file_path." << endl;
        throw exception();

    }

}

bool WorkingFile::autoInitializeFiles() {

    // Cycle through the range of files
    for (int i = lower_file_range; i <= upper_file_range; i++) {

        // To Do:
        // Check that there is a maximum number of files that can exist in the directory

        // To Do: Reset the curr_file_path
        setCurrentFileName(i);
        setCurrentFilePath();

        bool exists = fs::exists(curr_file_path);

        if (!exists) {

            json new_file;

            // Set the page numbers
            new_file["page_num"].push_back(curr_page_num);
            new_file["page_num"].push_back(curr_page_num + 1);

            // Check if this is one of the lessons that is a sublesson

            // Set the lesson number information
            // To Do: Handle the subnumbers
            // Manage the sub json object lesson_num
            json lesson_num;
            lesson_num["number"] = curr_lesson_num;
            json sub_number;
            // Manage the sub json object sub_number
            sub_number["active"] = false;
            sub_number["sub_number"] = 0;
            lesson_num["sub_number"] = sub_number;

            // Add the final "lesson_num" object
            new_file["lesson_num"] = lesson_num;

            ofstream fout(curr_file_path, ofstream::out);

            if (!fout) {

                cerr << "Failed to open curr_file_path" << endl;
                throw exception();

            }

            fout << new_file.dump(-1);

        }

        // Update all current variables to prepare for next file
        curr_page_num += 2;
        curr_lesson_num++;
    }

    return true;

}

SubLessonList::SubLessonList() {

    path_to_sublesson_file = base_path / sublesson_file_name;

}

void SubLessonList::setList() {

    ifstream fin(path_to_sublesson_file);

    if (!fin || fin.eof()) {

        cerr << "Unable to open the sublesson_file" << endl;
        throw exception();

    }

    sublesson_list = json::parse(fin);

    return;

}

// Check if input lesson number is in lesson list
json SubLessonList::checkLessonNum(const int &curr_number) {

    // Check that curr_number is valid
    if (curr_number > max_lesson || curr_number < lowest_lesson) {
        
        cerr << "An attempt to check a lesson number outside the appropriate range was made." << endl;
        throw exception();

    }

    // Declare working variables
    json results;
    json sublessons_json;
    vector<string> sublessons_vec;

    // Assume that the curr_number lesson is not in the list until proven otherwise
    results["is_in_list"] = false;
    results["sublessons"] = sublessons_json;

    // Retrieve the number of lessons listed in the sublesson_list file
    int num_elements = sublesson_list["lessons"].size();

    // Iterate over the lessons in sublesson_list to look for curr_number
    for (int i = 0; i < num_elements; i++) {

        json temp_lesson = sublesson_list["lessons"].at(i);

        // If there is a match, set the "is_in_list" value to true and break
        if (temp_lesson["lesson"] == curr_number) {

            // Optionally print message to console
            cout << "Detected a sublesson in lesson #" << curr_number << endl;

            results["is_in_list"] = true;

            break;
        }

    }

    // If curr_number lesson is in the list, extract the sublessons
    if (results["is_in_list"] == true) {

        // Extract the sublesson list to vector<string> for processing
        json temp_json = sublesson_list["lessons"].at(curr_number);
        vector<string> temp_sublessons_array = temp_json["sublessons"];

        // Implicitly cast the json value to int to avoid 'unsigned long int' confusion
        int temp_json_size = temp_json["sublessons"].size();

        // Add each sublesson string to the sublessons vector
        for (int i = 0; i < temp_json_size; i++) {

            sublessons_vec.push_back(temp_sublessons_array.at(i));

        }

    } else {

        return results;

    }


    // If the sublessons_vec is greater than 0
    if (sublessons_vec.size() > 0 && results["is_in_list"] == true) {

        int sublessons_vec_size = sublessons_vec.size();
        for (int i = 0; i < sublessons_vec_size; i++) {

            sublessons_json.push_back(sublessons_vec.at(i));

        }

        results["sublessons"] = sublessons_json;

    } else if (sublessons_vec.size() == 0 && results["is_in_list"] == true) {

        cerr << "An error occurred between detecting a lesson with sublessons and extracting the sublessons" << endl;

        throw exception();

    }

    return results;

}

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
