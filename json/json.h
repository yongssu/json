#pragma once
#pragma once
////////////////////////////////////////////////////////////////////////////////
/// @file  Json.h
/// @brief Json解析器
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "reader.h"


////////////////////////////////////////////////////////////////////////////////
/// @brief JSON元素类型
////////////////////////////////////////////////////////////////////////////////
enum JSONVALUETYPE
{
	JSONVALUETYPE_NULL,   ///< @brief 空类型
	JSONVALUETYPE_BOOL,   ///< @brief 逻辑类型
	JSONVALUETYPE_NUMBER, ///< @brief 数字类型
	JSONVALUETYPE_STRING, ///< @brief 字符串类型
	JSONVALUETYPE_LIST,   ///< @brief 表类型
	JSONVALUETYPE_DICT,   ///< @brief 字典类型
};

class JsonString;
class JsonList;
class JsonDict;

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON值
////////////////////////////////////////////////////////////////////////////////
class JsonValue
{
	friend class JsonString;
	friend class JsonList;
	friend class JsonDict;
	friend class Json;
protected:
	JSONVALUETYPE m_Type;  ///< @brief JSON对象类型
	/// @brief 数据域
	union
	{
		bool m_ValueBool;     ///< @brief 逻辑型
		double m_ValueNumber; ///< @brief 数字型
	};
public:
	/// @brief      写到字符串
	/// @param[out] OutStr 输出的目的字符串
	virtual void writeToStr(std::string& OutStr);
public: //用于接口转换
	virtual JsonString* toString();   ///< @brief 转换到字符串
	virtual JsonList* toList();       ///< @brief 转换到数组
	virtual JsonDict* toDict();       ///< @brief 转换到字典
public: //类型转换
	JSONVALUETYPE getType();       ///< @brief 返回类型

	bool toNull();                 ///< @brief 转换到NULL
	///< @note  true表示NULL，false表示非NULL
	bool toBool();                 ///< @brief 转换到Bool
	///< @note  true表示true，false表示非false
	double toNumber();             ///< @brief 到数字
	///< @brief 非Number型用0表示
public: // 值类型操作，非值类型返回false
	bool setValue();                 ///< @brief 设置为NULL
	///< @return 如果对象不是值类型，则返回假
	bool setValue(bool Value);		 ///< @brief 设置为bool
	///< @return 如果对象不是值类型，则返回假
	bool setValue(double Value);     ///< @brief 设置为数字
	///< @return 如果对象不是值类型，则返回假
public: // 用于手动创建
	JsonValue();                       ///< @brief 创建为null值类型
	JsonValue(bool Value);             ///< @brief 创建为bool值类型
	JsonValue(double Value);           ///< @brief 创建为数值类型
	JsonValue(JSONVALUETYPE Type);     ///< @brief 创建为String/List/Dict
	virtual ~JsonValue();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字符串
////////////////////////////////////////////////////////////////////////////////
class JsonString : public JsonValue
{
protected:
	std::string m_Str;   ///< @brief 字符串字面值
public:
	void writeToStr(std::string& OutStr);
public: //用于接口转换
	JsonString* toString();   ///< @brief 转换到字符串
public:
	std::string getStr();           ///< @brief 获得字符串
	void setStr(std::string Value); ///< @brief 设置字符串
public:
	/// @brief     构造函数
	/// @param[in] Value 字符串值
	JsonString(std::string Value);
	~JsonString();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON数组
/// @note  JsonList将会负责销毁子对象
////////////////////////////////////////////////////////////////////////////////
class JsonList : public JsonValue
{
protected:
	std::vector<JsonValue*> m_ObjList;   ///< @brief 内部数组
public:
	void writeToStr(std::string& OutStr);
public: //用于接口转换
	JsonList* toList();       ///< @brief 转换到数组

public:
	/// @brief     获得对象
	/// @param[in] Index 索引
	/// @return    如果索引不存在返回NULL，否则返回对象指针
	JsonValue* getValue(int Index);

	/// @brief     设置对象
	/// @param[in] Index 索引
	/// @param[in] pNew  对象指针
	/// @return    返回操作是否成功
	bool setValue(int Index, JsonValue* pNew);

	/// @brief     往数组末端追加对象
	/// @param[in] pNew 对象指针
	void append(JsonValue* pNew);

	/// @brief 清空
	void clear();

	/// @brief 返回对象数量
	int getCount();
public:
	/// @brief 构造函数
	JsonList();
	~JsonList();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON字典
////////////////////////////////////////////////////////////////////////////////
class JsonDict : public JsonValue
{
protected:
	std::vector<std::string> m_ObjList;                      ///< @brief 对象数组
	std::unordered_map<std::string, JsonValue*> m_Cache;  ///< @brief 对象字典缓存
public:
	void writeToStr(std::string& OutStr);
public: //用于接口转换
	JsonDict* toDict();       ///< @brief 转换到字典
public:
	/// @brief      返回对象
	/// @param[in]  Index 对象索引
	/// @param[out] pKeyOut 键
	/// @return     返回对象指针，若索引越界返回NULL
	JsonValue* getValue(int Index, std::string* pKeyOut = NULL);

	/// @brief     返回对象
	/// @param[in] Name 对象键名
	/// @return    返回对象指针，若不存在对象返回NULL
	JsonValue* getValue(std::string Name);

	/// @brief     设置对象
	/// @note      如果对象已经存在则会释放原有对象
	/// @param[in] Name 对象键名
	/// @param[in] pNew 对象指针
	void setValue(std::string Name, JsonValue* pNew);

	/// @brief     是否包含对象
	/// @param[in] Name 对象的键名
	/// @return    true=包含对象，false=没包含对象
	bool contain(std::string Name);

	/// @brief     移除对象
	/// @param[in] Index 对象键名
	/// @return    true=成功，false=失败
	bool remove(std::string Index);

	/// @brief 清空
	void clear();

	/// @brief 返回元素个数
	int getCount();
public:
	/// @brief 构造函数
	JsonDict();
	~JsonDict();
};

////////////////////////////////////////////////////////////////////////////////
/// @brief JSON解析器
/// @note  注意该解析器只会解析第一个元素。
////////////////////////////////////////////////////////////////////////////////
class Json
{
private:
	JsonValue* m_Root;  ///< @brief 根节点
private: // 解析函数
	JsonValue* parseValue(Reader& Context);    ///< @brief 解析一个值
	JsonValue* parseNumber(Reader& Context);   ///< @brief 解析一个数值
	JsonString* parseString(Reader& Context);  ///< @brief 解析一个字符串
	JsonList* parseList(Reader& Context);      ///< @brief 解析一个表
	JsonDict* parseDict(Reader& Context);      ///< @brief 解析一个字典
public:
	JsonValue* getRoot();              ///< @brief 解析一个根节点
	void setRoot(JsonValue* pValue);   ///< @brief 设置根节点

	void writeToStr(std::string& pOut); ///< @brief 写到字符串
	void writeToStream(std::ofstream* pOut); ///< @brief 写到流
public:
	Json();                           ///< @brief 构造函数
	Json(const std::string& Str);    ///< @brief 从文本构造JSON
	Json(std::ifstream* pStream);         ///< @brief 从流构造JSON
	~Json();
};
/// @}
