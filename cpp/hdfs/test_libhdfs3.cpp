/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 11/24/16 14:05:12 CST
*/

//g++ test_libhdfs3.cpp -I/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/include/ -L/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/lib -lhdfs3
//export LIBHDFS3_CONF=/Users/hma/workspace/interma-hawq/depends/libhdfs3/test/data/function-test-mac.xml
//export DYLD_LIBRARY_PATH=/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/lib

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "hdfs/hdfs.h"

using namespace std;
//using namespace Hdfs::Internal;
//using Hdfs::CheckBuffer;


static bool CreateFile(hdfsFS fs, const char * path, int64_t blockSize,
                       int64_t fileSize) {
    hdfsFile out;
    size_t offset = 0;
    int64_t todo = fileSize, batch;
    std::vector<char> buffer(32 * 1024);
    int rc = -1;

    do {
        if (NULL == (out = hdfsOpenFile(fs, path, O_WRONLY, 0, 0, blockSize))) {
            break;
        }

        while (todo > 0) {
            batch = todo < static_cast<int32_t>(buffer.size()) ?
                    todo : buffer.size();
            //Hdfs::FillBuffer(&buffer[0], batch, offset);

            if (0 > (rc = hdfsWrite(fs, out, &buffer[0], batch))) {
                break;
            }

            todo -= rc;
            offset += rc;
        }

        rc = hdfsCloseFile(fs, out);
    } while (0);

    return rc >= 0;
}

void test_list_dir(hdfsFS fs, const char * dir_path) {
	int dir_num = 0;
	cout<<"LIST DIR:"<<endl;
	hdfsFileInfo *dirinfo = hdfsListDirectory(fs, dir_path, &dir_num);
	for (int i = 0; i < dir_num; i++)
		cout<<(dirinfo+i)->mName<<endl;
	hdfsFreeFileInfo(dirinfo, dir_num);
}

void test_cat_file(hdfsFS fs, const char *file_path) {
	cout<<"READ FILE:"<<endl;
	char buffer[64];
	int bytesToRead = sizeof(buffer);

	hdfsFile fin = hdfsOpenFile(fs, file_path, O_RDONLY, 0/*not used*/, 0, 0);
	int done = hdfsRead(fs, fin, buffer, bytesToRead);
	buffer[done] = '\0';

	cout<<buffer<<endl;

	int retval = hdfsCloseFile(fs, fin);
}

void try_open_twice(hdfsFS fs, const char *file_path) {
	int retval = 0;
	int rc = 0;
	char buf1[] = "12345";
	char buf2[] = "abcde";
	hdfsFile fout1 = hdfsOpenFile(fs, file_path, O_WRONLY | O_APPEND, 0/*not used*/, 0, 0);
	hdfsFile fout2 = hdfsOpenFile(fs, file_path, O_WRONLY | O_APPEND, 0/*not used*/, 0, 0);
	assert(fout2 == NULL);
	if (fout2 == NULL) {
		printf("open file failed: %s", hdfsGetLastError());
	}
	rc = hdfsWrite(fs, fout1, buf1, sizeof(buf1)-1);
	rc = hdfsWrite(fs, fout2, buf2, sizeof(buf2)-1);
	retval = hdfsCloseFile(fs, fout1);
	retval = hdfsCloseFile(fs, fout2);
}

int main()
{
	//create a connect builder.
	struct hdfsBuilder *builder = hdfsNewBuilder();

	//set namenode address.
	hdfsBuilderSetNameNode(builder, "localhost");

	//set namenode port.
	hdfsBuilderSetNameNodePort(builder, 9000);

	//connect to hdfs
	hdfsFS fs = hdfsBuilderConnect(builder);

	test_list_dir(fs, "/test2/");	
	test_cat_file(fs, "/test2/1.txt");	
	try_open_twice(fs, "/test2/1.txt");
	test_cat_file(fs, "/test2/1.txt");	

	//free hdfs builder
	hdfsFreeBuilder(builder);

	return 0;
}
