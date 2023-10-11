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

#include "nlohmann/json.hpp"   // json input/output


using std::string;
using std::istringstream;
using std::stringstream;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::to_string;
using std::ifstream;

using json = nlohmann::json;

// Clear terminal
int clear_terminal();

// Capture page_num for lessonJson
void capture_page_num(json &lessonJson);

// Capture lesson_num for lessonJson
void capture_leson_num(json &lessonJson);

#endif
