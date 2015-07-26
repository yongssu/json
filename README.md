# json
json是一个C++语言的Json解析器，提供json解析及构造json数据功能。
(目前只支持英文的解析，支持中文的构造）

#json解析器解析json样例
	ifstream fin("test.json");
	string str;
	Json json(&fin);      //开始解析
	json.writeToStr(str); //如果解析成功，打印出解析出来的json字符，失败则报出异常
 	cout << str << endl;  
	cout << endl;
	string str1;
	json.getRoot()->writeToStr(str1);  //读取json元素
	cout << str1 << endl;	
	string str2;   //依次读取元素
	json.getRoot()->toDict()->getValue("key4")->toList()->getValue(1)->writeToStr(str2);
	cout << str2 << endl;`
#json解析器构造json文本样例
	Json json;
	JsonDict dict1;
	//构造元素
	JsonValue value1(123.0);
	JsonValue value2(456.0);	
	JsonList value3;
	JsonValue val1(12.0);
	JsonValue val2(34.0);
	value3.append(&val1);
	value3.append(&val2);
	//把元素加入对象
	dict1.setValue("key1", &value1);
	dict1.setValue("key2", &value2);
	dict1.setValue("key3", &value3);
	//把dict1设为根节点
	json.setRoot(&dict1);
	//打印构造的json字符串
	string str;
	json.writeToStr(str);
	cout << str << endl;
