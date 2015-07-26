#pragma once
#include <string>
//////////////////////////////////////////////////////
/// @brief 异常处理基类
/// @note  任何异常从该类派生
///////////////////////////////////////////////////////
class Exception
{
protected:
	int m_Time;            ///< @brief 记录异常事件戳
	std::string m_ExcpSrc; ///< @brief 记录异常来源
	std::string m_ExcpDesc;///< @brief 记录异常信息
public:
	int getTime() const;           ///< @brief 获得异常时间
	std::string getSrc() const;    ///< @brief 获得异常来源
	std::string getDesc() const;   ///< @brief 获得异常信息
public:
	/// @brief 空构造函数
	Exception();
	/// @brief      构造函数
	/// @warning    描述不得超过512个字符
	/// @param[in] Src       异常源
	/// @param[in] DescFmt   异常格式
	Exception(std::string Src, const char* DescFmt, ...);
	Exception(const Exception& Org);
	~Exception();
};


//////////////////////////////////////////////////////////
/// @brief   json解析异常
/// @note    派生自Exception,用来描述词法错误
//////////////////////////////////////////////////////////
class ReaderException :public Exception
{
private:
	int m_Line;    ///< @brief 储存异常产生的行
	int m_Row;     ///< @brief 存储异常产生的列
public:
	int getLine();   ///< @brief 获得异常产生的行
	int getRow();    ///< @brief 获得异常产生的列
public:
	/// @brief             构造函数
	/// @note              构造函数会自动把行号和列号添加到Desc字段
	/// @param[in] Src     异常源
	/// @param[in] Desc    异常描述
	/// @param[in] Line    异常产生的行号
	/// @param[in] Row     异常产生的列
	ReaderException(std::string Src, const char* Desc, int Line, int Row);
	~ReaderException();
};
