#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "lib.h"

TEST(Version, Pozitive) {
	ASSERT_GT(version(), 0);
}

TEST(PrintIpTest, CommandLineTest) {
    std::string command = "./print_ip | md5sum";
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    std::string actual_output = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            actual_output += buffer;
        }
    }
    pclose(pipe);
    
    ASSERT_EQ(std::string("61ee39fc6ce945874cd6fa44e6851810  -\n"), actual_output);
}