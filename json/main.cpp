#include <iostream>
#include <cstdlib>
#include "json.h"
#include "exception.h"
#include <fstream>
using namespace std;

int main()
{
	/*
	ifstream fin("test.json");
	string str;
	Json json(&fin);
	json.writeToStr(str);
	cout << str << endl;
	cout << endl;
	string str1;
	json.getRoot()->writeToStr(str1);
	cout << str1 << endl;
	//ÒÀ´Î¶ÁÈ¡ÔªËØ
	string str2;
	json.getRoot()->toDict()->getValue("key4")->toList()->getValue(1)->writeToStr(str2);
	cout << str2 << endl;*/
	
	Json json;
	JsonValue value1(123.0);
	JsonDict dict1;
	dict1.setValue("key1", &value1);
	JsonValue value2(456.0);
	dict1.setValue("key2", &value2);
	json.setRoot(&dict1);
	JsonList value3;
	JsonValue val1(12.0);
	JsonValue val2(34.0);
	value3.append(&val1);
	value3.append(&val2);
	dict1.setValue("key3", &value3);
	JsonString str1("hello world");
	dict1.setValue("key4", &str1);
	string str;
	json.writeToStr(str);
	cout << str << endl;
	system("pause");
	return 0;
}