/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 04/02/2018 15:29:17 CST
*/

//compile in mac:
//gcc jsonc_example.c -I /usr/local/Cellar/json-c/0.12/include/json-c -L /usr/local/Cellar/json-c/0.12/lib/ -ljson-c

#include <json.h>
#include <stdio.h>

int main() {
	struct json_object *jobj, *arr, *item;

	char *str= "{'ret':0, 'data': [{'id':0,'msg':'msg1'}, {'id':1,'msg':'msg2'}]}";
	jobj = json_tokener_parse(str);

	json_object_object_get_ex(jobj, "ret", &item);
	int ret = json_object_get_int(item);
	printf("ret:%d\n", ret);

	json_object_object_get_ex(jobj, "data", &arr);
	int arraylen = json_object_array_length(arr);
	printf("%d items:\n", arraylen);
	for (int i = 0; i < arraylen; i++) {
		item = json_object_array_get_idx(arr, i);
		printf("data json string:%s\n", json_object_to_json_string(item));
		
		struct json_object *jtmp;
		json_object_object_get_ex(item, "id", &jtmp);
		int id = json_object_get_int(jtmp);
		json_object_object_get_ex(item, "msg", &jtmp);
		const char* msg = json_object_get_string(jtmp);

		printf("\t%d\t%s\n", id, msg);
	}


	return 0;
}

