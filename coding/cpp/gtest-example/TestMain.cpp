/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 03/08/2018 14:54:07 CST
*/

#include "gtest/gtest.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace testing;

class TestInterMa: public ::testing::Test {
    virtual void SetUp() {
    	str = std::string("nba");
	}
    virtual void TearDown() {
    }

protected:
	std::string str;
};

TEST_F(TestInterMa, Example) {
    EXPECT_EQ(str, std::string("nba"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
