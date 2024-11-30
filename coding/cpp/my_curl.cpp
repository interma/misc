/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/03/2020 14:55:24 CST
*/

using namespace std;

#include <stdio.h>
#include <curl.h>

#define YOUR_URL "http://krb-sdw1.c.data-gpdb-gptext.internal:18983/solr/admin/collections?action=LIST"
#define ANYUSER ""
#define ANYPWD ""

int main(int argc, char* argv[])
{
	CURLcode result;
	int x;
	CURL* curl = curl_easy_init();                  //initialize a easy curl handle
	if(curl){
		curl_easy_setopt(curl,CURLOPT_USERNAME, ANYUSER);                       //set second option to enable anyuser, a trick necessary for program to work
		curl_easy_setopt(curl,CURLOPT_USERNAME, ANYPWD);
		curl_easy_setopt(curl,CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_URL,YOUR_URL);
		curl_easy_setopt(curl,CURLOPT_HTTPAUTH, CURLAUTH_GSSNEGOTIATE);         //set first option to enable gssnegotiate authentication
		curl_easy_perform(curl);
		//curl_easy_cleanup(curl);
		scanf("%d", &x);                            //last statement used to get delay in demo situations
	}
	return 0;
}
