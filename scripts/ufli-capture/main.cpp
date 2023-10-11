// capture main.cpp
// Bryan Beus
// YKSD UFLI Data Creation Tool
// 2023-10-10

#include "builder.hpp"

int main() {

    clear_terminal();

    json lessonJson;

    capture_page_num(lessonJson);

    lessonJson = capture_leson_num(lessonJson);

    cout << lessonJson << endl;


    return 0;

}
