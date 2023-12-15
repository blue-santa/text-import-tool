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

// Write a generic json file in pretty print
bool writeJsonFilePrettyPrint(const json &json_file) {

    // Declare working variables
    fs::path json_file_path = base_path / "pretty-print.json";

    // Open the pretty print file
    ofstream fout(json_file_path, ofstream::trunc);

    // Handle possible errors
    if (!fout) {
        cerr << "Failed to open current working file path" << endl;
        throw exception();
    }

    // Write to file
    fout << json_file.dump(4);

    return true;
}


// Set log_file path
void LogFile::setLogFilePath() {

    // Declare working variable
    log_file_path = base_path / log_file_name;

}

// Create the log_file.json file if it does not already exist
// Private
void LogFile::initLogFile() {

    // Test if log_file_name exists
    bool exists = fs::exists(log_file_path);

    // To Do:
    // Set user and groups to protect host system

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

    // Open the log_file as json
    json log_file_json = openLogFile();

    // Add the new timestamp
    log_file_json["timestamps"].push_back(curr_date_time);
    
    // Load the most recent information from previous session
    loadMostRecent();

    // Write the log file
    writeLogFile(log_file_json);

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
bool LogFile::setMostRecent(const fs::path &most_recent_path) {

    // Declare working variable
    json most_recent;

    // Instantiate the most_recent path as a string
    most_recent["most_recent_path"] = most_recent_path.string();

    // Open the log_file as json
    log_file_json = openLogFile();

    // Instantiate the most_recent json variable
    // (It is empty at this point in the program)
    log_file_json["most_recent"] = most_recent;

    // Write the log_file
    writeLogFile(log_file_json);

    return true;
}

// Load the most_recent json object
bool LogFile::loadMostRecent() {

    // Open the log_file as a temporary variable
    json tmp_json = openLogFile();

    // Set the previous most_recent values as the current most_recent values
    log_file_json["most_recent"] = tmp_json["most_recent"];

    // To Do:
    // This can be deleted?
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
}

// Set the current file name
void WorkingFile::setCurrentFileName(const int curr_file_num) {

    // Convert curr_file_num to three digit string
    stringstream file_name_ss;
    file_name_ss << std::setw(3) << std::setfill('0') << curr_file_num;

    // Add the suffix to the file name
    file_name_ss << "_ufli.json";

    // Set hte current file name as the resulting file_name_ss variable
    curr_file_name = file_name_ss.str();

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

            // Declare a new pointer to the current log file
            LogFile *logPtr = &log_file;

            // To Do:
            // Can this be deleted?
            json temp;

            // Set the most_recent variable in log_file using the pointer
            logPtr->setMostRecent(curr_file_path);

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

    // Open the previous working_file data and set to current WorkingFile class obj
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


// Test whether the provided highest_key exists in working_file
bool WorkingFile::testHighestKey(const string &highest_key) {

    // Test if there is already a value in working_file for this element

    // Print status to console
    cout << "Testing whether " <<  highest_key << " exists in " << curr_file_name << ": " << endl;

    // Test if the highest_key variable exsts
    bool contains = working_file.contains(highest_key);

    // Print the results
    if (contains) {

        cout << "Result: " << contains << endl;

        cout << working_file[highest_key].template get<string>();

    } else {

        cout << "Result: " << contains << endl;

    }

    // Query the user's choice of direction
    string prompt = "Proceed with this section? (y/n)";
    string user_input = captureUserString(prompt);

    if (user_input != "y") {

        cout << "Skipping" << endl;

        // To Do: Handle exception properly
        return false;
    }

    return true;
}

// Query the user's approval for the provided working_element obj
bool WorkingFile::queryUserApprovalWorkingElement(const string &highest_key, const json &working_element) {

    // Inform user of total result and query if correct
    cout << "The final result for " << highest_key << " is: " << endl;
    cout << working_element.dump(4) << endl;

    // Prompt user's choice of direction
    string prompt = "Is this correct? (y/n)";
    string user_input = captureUserString(prompt);

    // If the result is not correct, end the program and debug
    if (user_input != "y") {

        cerr << "The current object is not correctly assembled" << endl;

        return false;

    }

    return true;
}

// Process the next element in the working_file
bool WorkingFile::processHeader(LogFile & log_file) {

    // Figure out which element we are working on
    // Ask the user which element we are working on

    clearTerminal();

    // TEST CONTENT GOES HERE 
    // // working_file["header_text"]["right"]["regular"]

    // Insert the highest key name here
    string highest_key = "header_text";

    // Test whether the highest key already exists
    // After discovering the result, as the user whether they want to continue
    bool user_response = testHighestKey(highest_key);

    // If the user does not wish to proceed with this section
    // Exit this func, returning true so as to move on to next section
    if (!user_response) {

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
    string prompt = "Please input the value for the regular text in the upper right corner: ";
    string regular_val = captureUserString(prompt);
    working_element["right"]["regular"] = regular_val;

    // Query user's approval and, if not approved, exit current function
    if (!queryUserApprovalWorkingElement(highest_key, working_element)) {

        return false;

    }

    // Put user inputted value into working_file["variable_section"]
    working_file[highest_key] = working_element;

    // RESTRUCTURING STOPS HERE

    // 2023-12-14 Pausing here

    // Pause for user to verify that all proceeded as planned
    clearTerminal();

    // Test a pretty-print version
    cout << "Testing pretty-print version of working_file: " << endl;

    writeJsonFilePrettyPrint(working_file);

    // Query user's choice of direction
    prompt = "Please verify that the pretty print file is correct. (y/n)";
    string user_input = captureUserString(prompt);

    if (user_input == "y") {

        writeCurrentWorkingFile(log_file);

    } else {

        cout << "User indicates that the pretty-print.json file is not correct" << endl;
        cout << "Ending current function iteration" << endl;
        return false;

    }

    return true;
}

// Process the next element in the working_file
bool WorkingFile::processLessonTitle(LogFile & log_file) {

    // Figure out which element we are working on
    // Ask the user which element we are working on

    clearTerminal();

    // TEST CONTENT GOES HERE 
    // // working_file["lesson_title"]["regular"]

    // Insert the highest key name here
    string highest_key = "lesson_title";

    // Test whether the highest key exists in working_file already
    // After test is conducted, have user decide whether or not to continue
    bool user_response = testHighestKey(highest_key);

    // If the user does not wish to continue, exit this function without further error
    if (!user_response) {

        return true;

    }

    // Create the deepest layer of json
    json working_element;

    // Update this as needed
    cout << "Working on the deepest key of the current json object" << endl;
    cout << "The current deepest key is " << highest_key << endl;

    // Model values:
      // "lesson_title": {
      //   "active": true,
      //   "bold": "a /ă/",
      //   "regular": "The grapheme A spells /ă/"
      // },
    
    // Query whether there is a lesson title for this lesson
    string prompt_active = "Is there a lesson title for this lesson? Enter 'y' for yes:"; 
    string active = captureUserString(prompt_active);

    // If there is, craft the "active" value
    if (active == "y") {

        // Craft the active value
        working_element["active"] = true;

    // If there is not, set "active" to false and enter default hidden values
    } else {

        working_element["active"] = false;
        working_element["bold"] = working_file["header_text"]["right"]["regular"].template get<string>();
        working_element["regular"] = "Lorum ipsum...";

        return true;
    }

    // Continue crafting an active title

    // Craft the bold portion of the lesson title

    // Query the user whether the upper right header and bold title are the same
    string prompt_title_and_header = "Is the upper right header text the same as the bold title text? Enter 'y' for yes: ";
    string is_the_same = captureUserString(prompt_title_and_header);

    // If it is the same, set the working_file accordingly
    if (is_the_same == "y") {

        working_element["bold"] = working_file["header_text"]["right"]["regular"].template get<string>();

    // If it is not, obtain the bold value from the user
    } else {

        string prompt_obtain_bold_title_val = "Please enter the bold portion of the title (not including the colon): ";
        string bold_portion = captureUserString(prompt_obtain_bold_title_val);

        working_element["bold"] = bold_portion;
    }

    // Obtain the regular font value of the title
    string prompt_regular_title = "Please enter the regular font value of the title: ";
    string regular_title_val = captureUserString(prompt_regular_title);

    working_element["regular"] = regular_title_val;

    // Query user approval on final working element
    if (!queryUserApprovalWorkingElement(highest_key, working_element)) {

        return false;

    }

    // Put user inputted value into working_file[highest_key]
    working_file[highest_key] = working_element;

    // RESTRUCTURING STOPS HERE

    // Pause for user to verify that all proceeded as planned using pretty print
    clearTerminal();
    cout << "Testing pretty-print version of working_file: " << endl;
    writeJsonFilePrettyPrint(working_file);

    // Wait for user to approve pretty-print file
    string prompt_pretty_print = "Please verify that the pretty print file is correct. (y/n)";
    string user_input = captureUserString(prompt_pretty_print);

    // If the user approves of the pretty print, write the final file
    if (user_input == "y") {

        writeCurrentWorkingFile(log_file);

    // If the user does not approve, exit with false indicator to repeat
    } else {

        return false;

    }

    return true;
}

// Process instructional notes element in working_file
bool WorkingFile::processInstructionalNotes(LogFile &log_file) {


    // Figure out which element we are working on
    // Ask the user which element we are working on

    clearTerminal();

    // TEST CONTENT GOES HERE 
    // // working_file["lesson_title"]["regular"]

    // Insert the highest key name here
    string highest_key = "instructional_notes";

    // Test whether the highest key exists in working_file already
    // After test is conducted, have user decide whether or not to continue
    bool user_response = testHighestKey(highest_key);

    // If the user does not wish to continue, exit this function without further error
    if (!user_response) {

        return true;

    }

    // Create the deepest layer of json
    json working_element;

    // Create working values
    string inst_title = "Instructional Notes";

    // Update this as needed
    cout << "Working on the deepest key of the current json object" << endl;
    cout << "The current deepest key is " << highest_key << endl;

    // Model values:
      // "title": "Instructional Notes",
      // "active": true,
      // "full_text": "At this point, A is being introduced only as short /ă/.  A can also represent other sounds, such as long /ā/ in A_E words (Lesson 54)."
    
    // Query whether there are any instructional notes for this lesson
    string prompt_inst_active = "Are there any instructional notes? Enter 'y' for yes: ";
    string inst_active = captureUserString(prompt_inst_active);

    // If yes, set active accordingly
    if (inst_active == "y") {

        working_element["active"] = true;

    // If not, set default hidden values and end the function without further error
    } else {

        working_element["active"] = false;
        working_element["title"] = inst_title;
        working_element["instructional_notes"] = "Lorum ipsum...";

        return true;
    }

    // Query whether there is a lesson title for this lesson
    string prompt_title_same = "Is the title of the section'" + inst_title + "'? Enter 'y' for yes:"; 
    string same = captureUserString(prompt_title_same);

    // If it is, then put set up the working_element accordingly
    if (same == "y") {

        // Craft the active value
        working_element["title"] = inst_title;

    // If it is not, query the user for input
    } else {

        string prompt_inst_title = "Please provide the title: ";
        string title = captureUserString(prompt_inst_title);

        working_element["title"] = title;
    }

    string prompt_inst_notes = "Please provide the instructional notes: ";
    string inst_notes = captureUserString(prompt_inst_notes);

    working_element["full_text"] = inst_notes;

    // Query user approval on final working element
    if (!queryUserApprovalWorkingElement(highest_key, working_element)) {

        return false;

    }

    // Put user inputted value into working_file[highest_key]
    working_file[highest_key] = working_element;

    // RESTRUCTURING STOPS HERE

    // Pause for user to verify that all proceeded as planned using pretty print
    clearTerminal();
    cout << "Testing pretty-print version of working_file: " << endl;
    writeJsonFilePrettyPrint(working_file);

    // Wait for user to approve pretty-print file
    string prompt_pretty_print = "Please verify that the pretty print file is correct. (y/n)";
    string user_input = captureUserString(prompt_pretty_print);

    // If the user approves of the pretty print, write the final file
    if (user_input == "y") {

        writeCurrentWorkingFile(log_file);

    // If the user does not approve, exit with false indicator to repeat
    } else {

        return false;

    }

    return true;

}

// Process phonemic awareness element in working_file
bool WorkingFile::processPhonemicAwareness(LogFile &log_file) {

    // Figure out which element we are working on
    // Ask the user which element we are working on

    clearTerminal();

    // TEST CONTENT GOES HERE 
    // // working_file["lesson_title"]["regular"]

    // Insert the highest key name here
    string highest_key = "phonemic_awareness";

    // Test whether the highest key exists in working_file already
    // After test is conducted, have user decide whether or not to continue
    bool user_response = testHighestKey(highest_key);

    // If the user does not wish to continue, exit this function without further error
    if (!user_response) {

        return true;

    }

    // Create the deepest layer of json
    json working_element;

    // Create working values
    string phon_title = "Phonemic Awareness";
    json word;

    // Update this as needed
    cout << "Working on the deepest key of the current json object" << endl;
    cout << "The current deepest key is " << highest_key << endl;

    // Model values:
        // "title": "Phonemic Awareness",
        // "active": true,
        // "blend": {
        //     "title": "Blend",
        //     "active": true,
        //     "content": [
        //         {
        //             "ufli_text": "at"
        //         }
        //     ]
        //     
        // },
        // "segment": {
        //     "title": "Segment",
        //     "active": true,
        //     "content": [
        //         {
        //             "ufli_text": "at"
        //         }
        //     ]
        // }

    // Craft working_element standard structure
    working_element["title"] = phon_title;
    working_element["blend"]["title"] = "Blend";
    working_element["blend"]["active"] = true;
    
    // Query whether there are any instructional notes for this lesson
    string prompt_sec_active = "Is the Phonemic Awareness section active? Enter 'y' for yes: ";
    string sec_active = captureUserString(prompt_sec_active);

    // If yes, set active accordingly
    if (sec_active == "y") {

        working_element["active"] = true;

    // STOPPING HERE
    // If not, set default hidden values and end the function without further error
    } else {

        working_element["active"] = false;
        word["ufli_text"] = "at";
        working_element["blend"]["content"].push_back(word);

        return true;
    }

    // Query whether there is a lesson title for this lesson
    string prompt_title_same = "Is the title of the section'" + phon_title + "'? Enter 'y' for yes:"; 
    string same = captureUserString(prompt_title_same);

    // If it is, then put set up the working_element accordingly
    if (same == "y") {

        // Craft the active value
        working_element["title"] = inst_title;

    // If it is not, query the user for input
    } else {

        string prompt_inst_title = "Please provide the title: ";
        string title = captureUserString(prompt_inst_title);

        working_element["title"] = title;
    }

    string prompt_inst_notes = "Please provide the instructional notes: ";
    string inst_notes = captureUserString(prompt_inst_notes);

    working_element["full_text"] = inst_notes;

    // Query user approval on final working element
    if (!queryUserApprovalWorkingElement(highest_key, working_element)) {

        return false;

    }

    // Put user inputted value into working_file[highest_key]
    working_file[highest_key] = working_element;

    // RESTRUCTURING STOPS HERE

    // Pause for user to verify that all proceeded as planned using pretty print
    clearTerminal();
    cout << "Testing pretty-print version of working_file: " << endl;
    writeJsonFilePrettyPrint(working_file);

    // Wait for user to approve pretty-print file
    string prompt_pretty_print = "Please verify that the pretty print file is correct. (y/n)";
    string user_input = captureUserString(prompt_pretty_print);

    // If the user approves of the pretty print, write the final file
    if (user_input == "y") {

        writeCurrentWorkingFile(log_file);

    // If the user does not approve, exit with false indicator to repeat
    } else {

        return false;

    }

    return true;


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

    // Write to file without indentation
    fout << working_file.dump(-1);

    // Update the log file
    log_file.setMostRecent(curr_file_path);

    return true;
}

// Return the current lesson number
int WorkingFile::returnCurrFileNum() {
    return curr_file_num;
}

// Initialization for SublessonList object
SublessonList::SublessonList() {

    // Create var for path to sublesson file
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

