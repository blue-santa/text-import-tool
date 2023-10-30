/*
 * builder.cpp
 * YKSD
 * Bryan Beus
 * Function file for UFLI_Lesson_Builder
 * Facilitates guided user input for the creation of UFLI Lessons converted to json output
 */

#include "builder.hpp"

// Clear terminal
int clear_terminal() {

    cout << "\033[2J\033[1;1H";

    return 0;
}

// Capture page_num for lessonJson
void capture_page_num(json &lessonJson) {

    int page_num[2];

    clear_terminal();

    cout << "Enter the first page number, hit enter, then enter the second page number" << endl;

    cin >> page_num[0];
    cin >> page_num[1];

    lessonJson["page_num"] = page_num;

    clear_terminal();

    return;

}

// Capture lesson_num for lessonJson
void capture_lesson_num(json &lessonJson) {

    int lesson_num;
    bool has_sub_number = false;
    int sub_number = 0;

    cout << "Enter the lesson number?" << endl;

    cin >> lesson_num;

    clear_terminal();

    cout << "Is this lesson split into parts a and b? Type \"yes\" if true, or type \"no\" if false." << endl;

    string user_input = "";

    cin >> user_input;

    if (user_input == "yes") {
        has_sub_number = true;
    }

    if (has_sub_number) {
        
        cout << "Enter \"0\" if this is part a and \"1\" if this is part b: " << endl;

        cin >> sub_number;

    }

    lessonJson["lesson_num"]["number"] = lesson_num;

    lessonJson["lesson_num"]["sub_number"]["active"] = has_sub_number;

    lessonJson["lesson_num"]["sub_number"]["sub_number"] = sub_number;

    clear_terminal();

    return;
    
}


string generate_date_and_time() {

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H-%M-%S");

    return ss.str();
}

// TO DO:
// Function to manage log file

// Generate the input path
fs::path generate_input_dir(const fs::path base_path, const string curr_date_time) {

    // Declare working strings
    string log_file_name = "log_file.json";
    string empty_dir_name = "0000-00-00-00-00-00";

    // Declare working paths
    fs::path input_path;
    fs::path suffix;
    fs::path log_file_path = base_path / log_file_name;
    cout << "log_file_path: " << log_file_path.string() << endl;

    // Declare working json
    json new_file;

    // Test if log_file_name exists
    bool exists = fs::exists(log_file_path);

    // If the file already exists:
    if (exists) {

        // Read previous file into json variable
        ifstream init_fin(log_file_path);

        // Test opening
        if (!init_fin || init_fin.eof()) {
            cerr << "Log_file: " + log_file_name + " exists, but did not open correctly." << endl;
            exit;
        }

        // Parse the new file into json format
        new_file = json::parse(init_fin);
        
        // Close the file
        init_fin.close();

    } else if (!exists) {

        // Create a temp ofstream to create initial file
        ofstream temp_fout(log_file_path, ofstream::out);

        if (!temp_fout) {
            cerr << "Failed to create ofstream for " + log_file_name << "." << endl;
            exit;
        }

        // Create a temporary new json file with init_dir_name as the first directory
        json temp_new_file;
        temp_new_file["timestamps"].push_back(init_dir_name);

        // Write the new file into log_file_name file and close the file
        temp_fout << temp_new_file.dump(-1);
        temp_fout.close();

    } else {
        
        cout << "Failed to test whether " + log_file_name + " exists." << endl;
        exit;

    }


    // Read in file for processing
    ifstream fin(log_file_path);

    // Open file
    ofstream fout(log_file_path, ofstream::trun);
    temp_fout << temp_new_file;

    // Close the file
    temp_fout.close();

    // Trunc
    new_file["timestamps"].push_back(curr_date_time);


    json new_file;
    new_file["timestamps"] = {curr_date_time};


        // To Do:
        // Return func here
        return 

    ofstream fout(log_file_path, ofstream::trunc);

    fin << "{}\n";
    fin.close();

    fin.open(log_file_path);

    if(!fin || fin.eof()) {
        // TO DO:
        // Create a new log file
        fin << new_file; // .dump(-1);
        cout << new_file.dump(-1) << endl;
    }
    

    // Test

    // TO DO: 
    //
    // Look for the most recent collection (directory) of 
    // json files in the ../../UFLI-Lesson-JSON/ directory
    //
    // Set the name of the last directory as the string suffix

    // Open a file name

    fin.close();

    input_path = prefix / suffix;

    return input_path;

}
