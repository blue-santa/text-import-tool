/*
 * single_declarations.hpp
 * YKSD
 * Bryan Beus
 * Single declaration file for UFLI_Lesson_Builder
 * Declares global variables that only need to be defined once
 */

#ifndef SINGLE_DEC_HPP
#define SINGLE_DEC_HPP

// Global base_path string var
const fs::path base_path = fs::canonical("../../UFLI-Lesson-JSON-test/");
// const fs::path base_path = fs::current_path();

#endif
