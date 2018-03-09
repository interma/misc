/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 03/08/2018 14:54:07 CST
*/


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"
extern "C"{
	#include "../tuplebatch.h"
}

using namespace testing;

class TestBatch: public ::testing::Test {
    virtual void SetUp() {
    	str = std::string("nba");
	}
    virtual void TearDown() {
    }

protected:
	std::string str;
};

//#define palloc0(sz) malloc(sz)

TEST_F(TestBatch, Generate) {
	TupleBatch tb = tbGenerate(10,10);
    EXPECT_EQ(tb->ncols, 10);
}

