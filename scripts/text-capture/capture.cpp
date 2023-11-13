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
// Change these to focus on specific areas
// Lowest possible is 0
int lower_file_range = 0;

// Upper value of file processing range
// Change these to focus on specific areas
// Highest possible is 137
int upper_file_range = 137;

// Start the lowest file number
// Don't change these
int lowest_file = 0;

// Start at the lowest page number
// Don't change these
int lowest_page = 62;

// Start at the lowest lesson number 
// Don't change these
int lowest_lesson = 1;

// The highest lesson number
// Don't change these
int max_lesson = 128;

// Clear terminal
void clearTerminal() {

    // Code to clear screen
    cout << "\033[2J\033[1;1H";

    return;
}

// Capture string input from user
string captureUserString(const string prompt) {

    // Create a string to hold user input
    string user_input;

    // Print the prompt
    cout << prompt << ": " << endl;

    // Capture user input
    std::getline(cin, user_input);

    return user_input;

}

// General function to open json files and return the content as a json object
json openJsonFile(fs::path file_path) {

    // Declare working variables
    json json_file;

    // Read previous file into json variable
    ifstream fin(file_path);

    // Test opening
    if (!fin || fin.eof()) {
        cerr << "File path exists, but failed to open: " + file_path.string() << endl;
        throw exception();
    }

    // Parse the new file into json format
    json_file = json::parse(fin);
    
    // Close the file
    fin.close();

    return json_file;
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
    
    // Load the most recent information from previous session
    loadMostRecent();

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

// Open log_file for processing
// public
// To Do: This should be just a standard openJsonFile() function
// so that it can be reused for LogFile, WorkingFile, and other classes
// See openJsonFile... function
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

// Write current log_file to disk
// Public
// To Do: Make this a general writeJsonFile(path) func
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

// Set the most recent file path and key
// Public
bool LogFile::setMostRecent(const fs::path &most_recent_path) {

    json most_recent;

    most_recent["most_recent_path"] = most_recent_path.string();

    log_file_json = openLogFile();

    log_file_json["most_recent"] = most_recent;

    writeLogFile(log_file_json);

    return true;
}


bool LogFile::loadMostRecent() {

    json tmp_json = openLogFile();

    log_file_json["most_recent"] = tmp_json["most_recent"];

    string tmp_str = log_file_json["most_recent"]["most_recent_path"].template get<std::string>();

    return true;

}

// Retrieve the most recent information
json LogFile::getMostRecentExtended() {

    // Retrieve the most recent file path
    string complete_path = log_file_json["most_recent"]["most_recent_path"].template get<std::string>();

    // Extract the most recent file name

    // Declare working variable
    string most_recent_filename;
    vector<string> dir_list;
    stringstream ss;

    // Indicate dir delimiter
    char dir_char = '/';

    // Iterate through file path and collect all directories in path
    for (long unsigned int i = 0; i < complete_path.length(); i++) {
        if (complete_path.at(i) == dir_char) {
            ss << complete_path.at(i);
            dir_list.push_back(ss.str());
            ss.str("");
        } else {
            ss << complete_path[i];
            dir_list.push_back(ss.str());
        }
    }

    // Retrieve the last filename
    most_recent_filename = dir_list.back();

    // Process filename to get most recent file as an int

    // Declare working variable
    stringstream m;
    string curr_file_as_str_int;
    int most_recent_file_int;

    // Capture all digits as strings
    for (long unsigned int i = 0; i < most_recent_filename.length(); i++) {
        if ( isdigit(most_recent_filename.at(i))) {
            m << most_recent_filename.at(i);
        }
    }

    // Convert string int to int
    most_recent_file_int = atoi(m.str().c_str());

    // Retrieve the most recent key
    string most_recent_key = log_file_json["most_recent"]["most_recent_key"].template get<std::string>();

    // Declare working variable
    json most_recent_extended_json;

    // Assemble output
    most_recent_extended_json["most_recent_file_path"] = complete_path;
    most_recent_extended_json["most_recent_filename"] = most_recent_filename;
    most_recent_extended_json["most_recent_file_int"] = most_recent_file_int;
    most_recent_extended_json["most_recent_key"] = most_recent_key;

    return most_recent_extended_json;

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

        // Get the sublesson as int
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

            string most_recent_key = "lesson_num";

            // Add the final "lesson_num" object
            new_file[most_recent_key] = lesson_num;

            LogFile *logPtr = &log_file;

            json temp;
            logPtr->setMostRecent(curr_file_path);

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

// Set the working_file variable
bool WorkingFile::setWorkingFile() {

    working_file = openJsonFile(curr_file_path);

    return true;
}

// Set the active path
bool WorkingFile::loadNextWorkingFile(LogFile &log_file) {

    // Discover the previous file
    json most_recent_json = log_file.getMostRecentExtended();

    // Declare working variables
    int prev_file_int = most_recent_json["most_recent_file_int"].template get<int>();
    int next_file_int;

    // Indicate the most recently updated file
    clearTerminal();
    cout << "The most recently updated file is: " << prev_file_int << endl;

    // If the end of the upper file range is reached:
    if (prev_file_int >= upper_file_range) {

        // Ask if user would like to restart from the lower_file_range
        string prompt = "This file is at or above the upper limit set by the upper_file_range var.\nWould you like to restart from the lower_file_range variable? (y/n)";
        string user_input = captureUserString(prompt);

        // If the user enters anything but yes, end the program
        if (user_input != "y") {

            cout << "Good-bye" << endl;

            // To Do: Manage this properly with exceptions
            return false;

        // Otherwise, restart the new_file_int at the bottom of the lower file range
        } else {

            next_file_int = lower_file_range;

        }

    // Otherwise, increase the file int by one
    } else {

        // Iterate to the next file number
        next_file_int = prev_file_int + 1;

    }

    // Verify that the next file is within range
    if (next_file_int < lowest_file || next_file_int < lower_file_range || next_file_int > upper_file_range) {

        cerr << "The next file is outside of the lower and upper file ranges" << endl;

        // To Do: Manage the exception properly
        throw exception();
    }

    // Set the WorkingFile class object variables
    curr_file_num = next_file_int;
    setCurrentFileName(curr_file_num);
    setCurrentFilePath();

    // To Do: Need to change the location of the curr_lesson_num and curr_page_num variables

    // Set the working_file json var by loading current file path
    setWorkingFile();

    return true;
}

// Process the next element in the working_file
bool WorkingFile::processHeader() {

    // Figure out which element we are working on
    // Ask the user which element we are working on

    clearTerminal();

    // TEST CONTENT GOES HERE 
    // The below content changes each time I run it
    // It would be cool to create a recursive function that builds a json object with layers of keys
    // // working_file["header_text"]["right"]["regular"]

    // Insert the highest key name here
    string highest_key = "header_text";

    // Test if there is already a value in working_file for this element
    cout << "Testing whether " <<  highest_key << " exists in " << curr_file_name << ": " << endl;

    bool contains = working_file.contains(highest_key);

    if (contains) {

        cout << "Result: " << contains << endl;

        cout << working_file[highest_key].template get<string>();

    } else {

        cout << "Result: " << contains << endl;

    }

    string prompt = "Proceed with this section? (y/n)";
    string user_input = captureUserString(prompt);

    if (user_input != "y") {

        cout << "Skipping" << endl;

        // To Do: Handle exception properly
        return true;
    }

    // Create the deepest layer of json
    json working_element;

    // Craft the value for the bold text
    int bold_val_int = working_file["lesson_num"]["number"].template get<int>();
    string bold_val = "Lesson " + std::to_string(bold_val_int);
    working_element["right"]["bold"] = bold_val;

    // Craft the left value
    working_element["left"] = "UFLI Foundations";

    // Update this as needed
    cout << "Working on the deepest key of the current json object" << endl;
    cout << "The current deepest key is 'regular' " << endl;

          // "header_text": {
          //   "left": "UFLI Foundations",
          //   "right": {
          //     "bold": "Lesson 1",
          //     "regular": "a /ă/"
          //   }
          // },
    
    // Craft the value for the regular text
    prompt = "Please input the value for the regular text in the upper right corner: ";
    string regular_val = captureUserString(prompt);
    working_element["right"]["regular"] = regular_val;

    // Inform user of total result and query if correct
    cout << "The final result is: " << endl;
    cout << working_element.dump(4) << endl;

    prompt = "Is this correct? (y/n)";
    user_input = captureUserString(prompt);

    // If the result is not correct, end the program and debug
    if (user_input != "y") {

        cerr << "The current object is not correctly assembled" << endl;

        throw exception();

    }


    // Put user inputted value into working_file["variable_section"]
    working_file[highest_key] = working_element;

    // RESTRUCTURING STOPS HERE

    // To Do: Write to file

    return true;
}

// Write the current working_file h
bool WorkingFile::writeCurrentWorkingFile(LogFile &log_file) {

    // Open the current file
    ofstream fout(curr_file_path, ofstream::trunc);

    // Handle possible errors
    if (!fout) {
        cerr << "Failed to open current working file path" << endl;
        throw exception();
    }

    // Write to file
    fout << working_file.dump(-1);

    // Update the log file
    log_file.setMostRecent(curr_file_path);

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

