#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include "lib.h"

TEST(Version, Pozitive) {
	ASSERT_GT(version(), 0);
}

TEST(IpFilterTest, CommandLineTest) {
    std::string command = "cat ip_filter.tsv | ./ip_filter | md5sum";
    FILE* pipe = popen(command.c_str(), "r");
    char buffer[128];
    std::string actual_output = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            actual_output += buffer;
        }
    }
    pclose(pipe);
    
    ASSERT_EQ(std::string("24e7a7b2270daee89c64d3ca5fb3da1a  -\n"), actual_output);
}