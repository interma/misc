/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 11/24/16 14:05:12 CST
*/

//g++ test_libhdfs3.cpp -I/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/include/ -L/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/lib -lhdfs3
//export LIBHDFS3_CONF=/Users/hma/workspace/interma-hawq/depends/libhdfs3/test/data/function-test-mac.xml
//export DYLD_LIBRARY_PATH=/Users/hma/workspace/interma-hawq/depends/libhdfs3/build/install/Users/hma/hawq/install/lib

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
void try_open_twice2(hdfsFS fs, const char *file_path) {
	int retval = 0;
	int rc = 0;
	char buf1[] = "12345";
	char buf2[] = "abcde";
	hdfsFile fout1 = hdfsOpenFile(fs, file_path, O_WRONLY, 0/*not used*/, 0, 0);
	hdfsFile fout2 = hdfsOpenFile(fs, file_path, O_WRONLY, 0/*not used*/, 0, 0);
	if (fout2 == NULL) {
		printf("open file failed: %s", hdfsGetLastError());
	}
	rc = hdfsWrite(fs, fout1, buf1, sizeof(buf1)-1);
	rc = hdfsWrite(fs, fout2, buf2, sizeof(buf2)-1);
	retval = hdfsCloseFile(fs, fout1);
	retval = hdfsCloseFile(fs, fout2);
	//open is ok, but two write/close cause failed:
	//No lease on /test2/1.txt (inode 0): File is not open for writing. Holder libhdfs3_client_random_297805678_count_1_pid_78821_tid_0x7fff9f2853c0 does not have any open files.
}

void test_multi_write(hdfsFS &fs) {
	test_cat_file(fs, "/test2/1.txt");	
	//try_open_twice(fs, "/test2/1.txt");
	try_open_twice2(fs, "/test2/1.txt");
	test_cat_file(fs, "/test2/1.txt");	
}

void test_read_write(hdfsFS &fs) {
	int retval = 0;
	int rc = 0;
	char buf1[] = "123456";
	const char *file_path = "/test2/read_write";

	//test seek
	hdfsFile fout = hdfsOpenFile(fs, file_path, O_WRONLY, 0/*not used*/, 0, 0);
	rc = hdfsWrite(fs, fout, buf1, sizeof(buf1)-1);

	hdfsFile fin = hdfsOpenFile(fs, file_path, O_RDONLY, 0/*not used*/, 0, 0);
	assert(fin != NULL);
	retval = hdfsSeek(fs, fin, 5);
	std::cout<<"seek1:"<<retval<<std::endl;
	
	rc = hdfsWrite(fs, fout, buf1, sizeof(buf1)-1);
	retval = hdfsSeek(fs, fin, 11);
	std::cout<<"seek2:"<<retval<<std::endl;
	
	retval = hdfsCloseFile(fs, fout);
	retval = hdfsSeek(fs, fin, 11);
	std::cout<<"seek3:"<<retval<<std::endl;

	retval = hdfsCloseFile(fs, fin);

	hdfsFile fin2 = hdfsOpenFile(fs, file_path, O_RDONLY, 0/*not used*/, 0, 0);
	assert(fin2 != NULL);
	retval = hdfsSeek(fs, fin2, 11);
	std::cout<<"seek4:"<<retval<<std::endl;
	retval = hdfsCloseFile(fs, fin2);

	//output
	//seek1:-1
	//seek2:-1
	//seek3:-1
	// so if seek failed when a write is working on the same file
	//seek4:0
	//TODO there are other more complex cases to be tested.
	
	

	//test read
	const char *buf2 = "abcdef";
	hdfsFile fin10 = hdfsOpenFile(fs, file_path, O_RDONLY, 0/*not used*/, 0, 0);
	assert(fin10 != NULL);
	
	hdfsFile fout10 = hdfsOpenFile(fs, file_path, O_WRONLY, 0/*not used*/, 0, 0);
	char read_buf[128];
	rc = hdfsRead(fs, fin10, read_buf, 5);
	std::cout<<read_buf<<std::endl;

	rc = hdfsWrite(fs, fout10, buf2, sizeof(buf2)-1);
	rc = hdfsRead(fs, fin10, read_buf, 7);
	std::cout<<read_buf<<std::endl;

	retval = hdfsCloseFile(fs, fin10);
	retval = hdfsCloseFile(fs, fout10);

	//output
	//12345
	//6123456
	//final content is: abcdef
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

	//test_list_dir(fs, "/test2/");	

	test_read_write(fs);

	//free hdfs builder
	hdfsFreeBuilder(builder);

	return 0;
}
