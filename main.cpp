#include <iostream>
#include <string>
#include "table.h"

int main(int argc, char** argv) {
    std::string input;
    if (argc > 1) {
        input = argv[argc - 1];
    }

    Table t;
    if (input != "") t.read_file(input);
    else {
        std::cerr << "Error: no input file" << std::endl;
        return 1;
    }

    t.parser();

    t.pagerank();

    // t.test();


    return 0;
}