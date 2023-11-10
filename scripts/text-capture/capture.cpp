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
int upper_file_range = 137;

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

// Set log_file path
// Private
void LogFile::setLogFilePath() {

    log_file_path = base_path / log_file_name;

}

// Create the log_file.json file if it does not already exist
// Private
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

    json log_file_json = openLogFile();

    // Add the new timestamp
    log_file_json["timestamps"].push_back(curr_date_time);

    writeLogFile(log_file_json);

    return;

}


// Create a LogFile object
// Public
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
// Public
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
// Public
string LogFile::getCurrDateTime() {

    return curr_date_time;
}

// Write current log_file to disk
// Public
bool LogFile::writeLogFile(const json &log_file_json) {

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

    return true;
}

// Open log_file for processing
// public
json LogFile::openLogFile() {

    // Declare new log_file_json for processing
    json log_file_json;

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

    return log_file_json;
}

// Set the most recent file path and key
// Public
bool LogFile::setMostRecent(const fs::path &most_recent_path, const string &most_recent_str) {


    json output;

    output["most_recent_path"] = most_recent_path.string();

    output["most_recent_key"] = most_recent_str;

    json log_file = openLogFile();

    log_file["most_recent"] = output;

    writeLogFile(log_file);

    return true;
}

// Initialization for WorkingFile class object
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

// Set the current file name
void WorkingFile::setCurrentFileName(const int curr_file_num) {

    // Convert curr_file_num to three digit string
    stringstream file_name_ss;
    file_name_ss << std::setw(3) << std::setfill('0') << curr_file_num;
    file_name_ss << "_ufli.json";

    curr_file_name = file_name_ss.str();

    // Optional print current filename
    // cout << curr_file_name << endl;

}

// Set the current file path
void WorkingFile::setCurrentFilePath() {

    // Try to set the current file path
    // In case of failure, throw an exception
    try {

        curr_file_path = base_path / curr_file_name;

    } catch (...) {

        cerr << "Failed to create curr_file_path." << endl;
        throw exception();

    }

}

// Auto initialize each json file, if it does not already exist
bool WorkingFile::autoInitializeFiles(LogFile &log_file) {

    // Import the sublesson_list
    SublessonList *sublesson_list = new SublessonList;
    sublesson_list->setList();

    // Cycle through the range of files
    for (int i = lower_file_range; i <= upper_file_range; i++) {

        // Set current file name and path
        setCurrentFileName(i);
        setCurrentFilePath();

        // Create the lesson_num object, which will later be added to new_file
        // Set the "number" property to curr_lesson_num
        json lesson_num;
        lesson_num["number"] = curr_lesson_num;

        // Create an empty sub_number json, which will later be added to new_file
        // Set default to false and 0
        json sub_number;
        sub_number["active"] = false;
        sub_number["sub_number"] = 0;

        // Check if this is one of the lessons that has sublessons
        json sublesson_results = sublesson_list->checkLessonNum(curr_lesson_num);

        // If this lesson is in the list, set the sub_number property to true
        if (sublesson_results["active"]) {

            sub_number["active"] = true;
            sub_number["sub_number"] = curr_position_sublesson_array;

        }

        // To Do:
        // use the get() template thing
        // https://json.nlohmann.me/api/basic_json/get/#return-value

        int sublessons_int = sublesson_results["sublessons"].template get<int>();

        // Indicate whether this is the last position in the list of sublessons
        if (curr_position_sublesson_array == sublessons_int) {

            curr_file_last_sublesson = true;

        } else {

            curr_file_last_sublesson = false;

        }

        // If this is the last position, reset the relevant variables
        if (curr_file_last_sublesson == true) {

            curr_position_sublesson_array = 0;

        }

        // Add the final sub_number json object to lesson_num
        lesson_num["sub_number"] = sub_number;

        // Test to see if the current file path already exists
        bool exists = fs::exists(curr_file_path);

        // If it does not exist, execute the following
        if (!exists) {

            // Create an empty json file for output
            json new_file;

            // Set the page numbers in the output file
            for (int i = 0; i < 2; i++) {
                new_file["page_num"].push_back(curr_page_num);
                curr_page_num++;
            }

            string temp_str = "lesson_num";

            // Add the final "lesson_num" object
            new_file[temp_str] = lesson_num;

            LogFile *logPtr = &log_file;

            json temp;
            logPtr->setMostRecent(curr_file_path, temp_str);

            // writeWorkingFile(curr_file_path, new_file);

            // Write the file
            ofstream fout(curr_file_path, ofstream::out);

            if (!fout) {

                cerr << "Failed to open curr_file_path" << endl;
                throw exception();

            }

            fout << new_file.dump(-1);

        }

        // Increase either the lesson number or the sublesson number, but not both
        if (curr_file_last_sublesson) {

            curr_lesson_num++;

        } else {

            curr_position_sublesson_array++;

        }

    }

    // Free up the memory from the sublesson_list object
    sublesson_list->~SublessonList();

    // Return true to indicate all finished successfully
    return true;

}

// Initialization for SublessonList object
SublessonList::SublessonList() {

    path_to_sublesson_file = base_path / sublesson_file_name;

}

// Destruction of SublessonList object
SublessonList::~SublessonList() {

}

// Import the sublesson_list file
void SublessonList::setList() {

    // Open the sublesson file
    ifstream fin(path_to_sublesson_file);

    // Test if successful
    if (!fin || fin.eof()) {

        cerr << "Unable to open the sublesson_file" << endl;
        throw exception();

    }

    // Parse the file
    sublesson_list = json::parse(fin);

    return;

}

// Check if input lesson number is in lesson list
json SublessonList::checkLessonNum(const int &curr_number) {

    // cout << "Checking lesson number: " << curr_number << endl;

    // Check that curr_number is valid
    if (curr_number > max_lesson || curr_number < lowest_lesson) {
        
        cerr << "An attempt to check a lesson number outside the appropriate range was made." << endl;
        throw exception();

    }

    // Declare working variables
    json results;
    json sublessons_json;
    int num_sublessons = 0;

    // Assume that the curr_number lesson is not in the list until proven otherwise
    results["active"] = false;
    results["sublessons"] = num_sublessons;

    // Retrieve the number of lessons listed in the sublesson_list file
    int num_elements = sublesson_list["lessons"].size();

    // Create a var to hold any possible matches
    int match = 0;

    // Iterate over the lessons in sublesson_list to look for curr_number
    for (int i = 0; i < num_elements; i++) {

        // Create a temporary json container to improve code readability
        json temp_lesson = sublesson_list["lessons"].at(i);

        // Do the following upon a match
        if (temp_lesson["lesson"] == curr_number) {

            // Set active to true
            results["active"] = true;

            // Save i in the match variable for later usage
            match = i;

            break;
        }

    }

    // If curr_number lesson is in the list, extract the sublessons
    if (results["active"] == true) {

        // Extract the sublesson list to vector<string> for processing
        json temp_json = sublesson_list["lessons"].at(match);
        num_sublessons = temp_json["sublessons"];

    } else {

        return results;

    }


    // If the sublessons_vec is greater than 0
    if (num_sublessons > 0 && results["active"] == true) {

        results["sublessons"] = num_sublessons;

    // On the other hand, if somehow the below statement is true, throw an exception
    } else if (num_sublessons == 0 && results["active"] == true) {

        cerr << "An error occurred between detecting a lesson with sublessons and extracting the number of sublessons" << endl;

        throw exception();

    }

    return results;

}

// Capture string input from user
string captureUserString(const string prompt) {

    // Create a string to hold user input
    string user_input;

    // Clear the terminal
    clear_terminal();

    // Print the prompt
    cout << prompt << ": " << endl;

    // Capture user input
    std::getline(cin, user_input);

    return user_input;

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
