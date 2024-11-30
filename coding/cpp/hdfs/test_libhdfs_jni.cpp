/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 07/26/2017 14:53:37 CST
*/
//g++ test_libhdfs_jni.cpp -I/Users/hma/hawq/hadoop-2.7.3/include -L/Users/hma/hawq/hadoop-2.7.3/lib/native -lhdfs
//export CLASSPATH=`hadoop classpath --glob`
/*
 * Ok, this is officially a Java 1.8 bug:
 *
 * https://bugs.openjdk.java.net/browse/JDK-7131356
 *
 * Ironically, you have to install 1.6 from Apple
 *
 * http://support.apple.com/kb/DL1572
 *
 * then Oracle Java 1.8 will start working.
*/
#include <stdio.h>
#include <string.h>
#include "hdfs.h"

int main(int argc, char **argv) {

	hdfsFS fs = hdfsConnect("default", 0);
	const char* writePath = "/test2/2.txt";
	hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY |O_CREAT, 0, 0, 0);
	if(!writeFile) {
		fprintf(stderr, "Failed to open %s for writing!\n", writePath);
		return -1;
	}
	/*
	//try open again, ERR msg: No lease on /test2/2.txt
	hdfsFile writeFile2 = hdfsOpenFile(fs, writePath, O_WRONLY |O_CREAT, 0, 0, 0);
	if(!writeFile2) {
		fprintf(stderr, "Failed to open %s for writing!\n", writePath);
		return -1;
	}
	*/
	const char* buffer = "Hello, World!";
	tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
	if (hdfsFlush(fs, writeFile)) {
		fprintf(stderr, "Failed to 'flush' %s\n", writePath);
		return -1;
	}
	hdfsCloseFile(fs, writeFile);
	return 0;
}
