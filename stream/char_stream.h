//==================================================================================================
// 文件：char_stream.h
// 作者：顾宇浩
// 简介：字符流
// 上次修改：2020-5-10 (v1.3.0)
//==================================================================================================

#pragma once
#include <cstddef>
#include <string>
#include "byte_stream.h"
#include "numformat.h"


#define ENABLE_SIZE_T_OVERLOAD	false  // 启用size_t的重载



//==================================================================================================
// 类名：ICharStream
// 说明：输入字符流类
//==================================================================================================
class ICharStream :virtual public IByteStream
{
public:
	//==============================================================================================
	// 函数：构造
	// 参数：in - 输入字节流源
	// 说明：无
	//==============================================================================================
	ICharStream(IByteStream& in) :_in(in), _status(true) {}

public:
	operator bool() { return _status; }

public:
	/***********************************************************************************************
	 * 输入运算符重载
	 * 注：operator>>出错时，变量不会被初始化，你可以充分利用这条性质方便地自定义变量的默认值。
	 **********************************************************************************************/

	 /* 基本输入 */
	ICharStream& operator>>(char& c);
	ICharStream& operator>>(char* buf_p);  // 警告：不安全的函数
	ICharStream& operator>>(uint32_t& n);
	ICharStream& operator>>(int32_t& n);
	ICharStream& operator>>(float& n);

	/* 增强输入 */
	ICharStream& operator>>(bool& b);  // 接受数字、"true"和"false"
	ICharStream& operator>>(uint8_t& n);
	ICharStream& operator>>(uint16_t& n);
	ICharStream& operator>>(int8_t& n);
	ICharStream& operator>>(int16_t& n);
	ICharStream& operator>>(std::string& str);
#if ENABLE_SIZE_T_OVERLOAD
	ICharStream& operator>>(size_t& n);
#endif

public:
	//==============================================================================================
	// 函数：get
	// 参数：p - 存放取出字符的指针
	// 返回：成功返回true；失败返回false
	// 说明：从流中取出一个字符，失败会自动置流状态位。
	//==============================================================================================
	virtual bool get(Byte* p) { return _in.get(p) ? true : _status = false; }

public:
	/* 按段读取 */

	//==============================================================================================
	// 函数：get_section
	// 参数：
	//		buf_p - 缓冲区指针
	//		bufSize - 缓冲区大小
	//
	// 返回：
	//		返回值在[0,bufSize-1]内时，表明若缓冲区空间足够，完整容下段，此时缓冲区以'\0'结尾，返回值含
	//		义为不含末尾'\0'的段长度；
	//		返回值为bufSize时，表明缓冲区空间不足，段未接收完全，同时，缓冲区末尾没有'\0'结束标记；
	//		返回值为SIZE_MAX时，表明接受过程出错，同时置错误状态。
	//
	// 说明：
	//		从流中取出一个“段”。
	//		若该次没有完全接收段数据，则下次调用该函数时接着接收这一段，即这个函数不会丢失段的数据。
	//==============================================================================================
	size_t get_section(char* buf_p, size_t bufSize) { return get_piece(buf_p, bufSize, ICharStream::is_whitespace); }

	//==============================================================================================
	// 函数：get_full_section（重载）
	// 参数：
	//		buf_p - 缓冲区指针
	//		bufSize - 缓冲区大小
	//
	// 返回：成功返回true，在传输出错、缓冲区不足以及其它任何没有完整取出的情况下，都返回false，同时置错
	//		误状态。
	// 说明：
	//		C字符串版本，从流中完整取出一个“段”。
	//		若缓冲区空间不足，则溢出的段数据将丢失。
	//==============================================================================================
	bool get_full_section(char* buf_p, size_t bufSize) { return get_full_piece(buf_p, bufSize, ICharStream::is_whitespace); }

	//==============================================================================================
	// 函数：get_full_section（重载）
	// 参数：无
	// 返回：接收成功返回非空串，失败返回空串
	// 说明：
	//		C++字符串版本，从流中完整取出一个“段”。
	//		由于C++字符串有自增长特性，这个函数不会丢失数据。
	//==============================================================================================
	std::string get_full_section() { return get_full_piece(ICharStream::is_whitespace); }

	//==============================================================================================
	// 函数：skip_this_section
	// 参数：无
	// 返回：成功返回true，出错时返回false，同时置错误状态。
	// 说明：跳过当前的段，若当前字符是分隔符，则只会跳过一个字符。
	//==============================================================================================
	bool skip_this_section() { return skip_this_piece(ICharStream::is_whitespace); }

	//==============================================================================================
	// 函数：skip_next_section
	// 参数：无
	// 返回：成功返回true，出错时返回false，同时置错误状态。
	// 说明：跳过下一个段，与skip_this_section的区别在于，当流中接下来是分隔符时，skip_this_section只会
	//		跳过一个字符，而skip_next_section会略去分隔符后再跳过下一个段。在下一个字符不是分隔符时，两
	//		者的行为是一致的。
	//==============================================================================================
	bool skip_next_section() { return skip_next_piece(ICharStream::is_whitespace); }

public:
	/* 按行读取 */

	//==============================================================================================
	// 函数：get_line
	// 参数：
	//		buf_p - 缓冲区指针
	//		bufSize - 缓冲区大小
	//
	// 返回：
	//		返回值在[0,bufSize-1]内时，表明若缓冲区空间足够，完整容下行，此时缓冲区以'\0'结尾，返回值含
	//		义为不含末尾'\0'的行长度；
	//		返回值为bufSize时，表明缓冲区空间不足，行未接收完全，同时，缓冲区末尾没有'\0'结束标记；
	//		返回值为SIZE_MAX时，表明接受过程出错，同时置错误状态。
	//
	// 说明：
	//		从流中取出一行。
	//		若该次没有完全接收行数据，则下次调用该函数时接着接收这一行，即这个函数不会丢失行的数据。
	//==============================================================================================
	size_t get_line(char* buf_p, size_t bufSize) { return get_piece(buf_p, bufSize, ICharStream::is_lineEnd); }

	//==============================================================================================
	// 函数：get_full_line
	// 参数：
	//		buf_p - 缓冲区指针
	//		bufSize - 缓冲区大小
	//
	// 返回：成功返回true，在传输出错、缓冲区不足以及其它任何没有完整取出的情况下，都返回false，同时置错
	//		误状态。
	// 说明：
	//		从流中完整取出一行。
	//		若缓冲区空间不足，则溢出的行数据将丢失。
	//==============================================================================================
	bool get_full_line(char* buf_p, size_t bufSize) { return get_full_piece(buf_p, bufSize, ICharStream::is_lineEnd); }

	//==============================================================================================
	// 函数：get_full_line（重载）
	// 参数：无
	// 返回：接收成功返回非空串，失败返回空串
	// 说明：
	//		C++字符串版本，从流中完整取出一行。
	//		由于C++字符串有自增长特性，这个函数不会丢失数据。
	//==============================================================================================
	std::string get_full_line() { return get_full_piece(ICharStream::is_lineEnd); }

	//==============================================================================================
	// 函数：skip_this_line
	// 参数：无
	// 返回：成功返回true，出错时返回false，同时置错误状态。
	// 说明：跳过当前的行，若当前字符是分隔符，则只会跳过一个字符。
	//==============================================================================================
	bool skip_this_line() { return skip_this_piece(ICharStream::is_lineEnd); }

	//==============================================================================================
	// 函数：skip_next_section
	// 参数：无
	// 返回：成功返回true，出错时返回false，同时置错误状态。
	// 说明：跳过下一个行，与skip_this_section的区别在于，当流中接下来是分隔符时，skip_this_section只会
	//		跳过一个字符，而skip_next_section会略去分隔符后再跳过下一个行。在下一个字符不是分隔符时，两
	//		者的行为是一致的。
	//==============================================================================================
	bool skip_next_line() { return skip_next_piece(ICharStream::is_lineEnd); }

public:
	/* 状态访问 */

	//==============================================================================================
	// 函数：status
	// 参数：无
	// 返回：正常返回true，出错返回false。
	// 说明：流状态受传输错误、缓冲区溢出、转换失败等多种因素影响，请查看其他函数的说明。
	//==============================================================================================
	bool status() { return _status; }

	//==============================================================================================
	// 函数：clear
	// 参数：无
	// 返回：无
	// 说明：复位流的状态
	//==============================================================================================
	void clear() { _status = true; }

private:
	using IsSeperator = bool (*)(char);

private:
	static bool is_whitespace(char c) { return ('\t' <= c && c <= '\r') || c == ' ' || c == '\0'; };
	static bool is_lineEnd(char c) { return c == '\n' || c == '\0'; };

private:
	IByteStream& _in;
	bool _status;

private:
	size_t get_piece(char* buf_p, size_t bufSize, IsSeperator isSep);
	bool get_full_piece(char* buf_p, size_t bufSize, IsSeperator isSep);
	std::string get_full_piece(IsSeperator isSep);
	bool skip_this_piece(IsSeperator isSep);
	bool skip_next_piece(IsSeperator isSep);
};



//==================================================================================================
// 类名：OCharStream
// 说明：输出字符流类
//==================================================================================================
class OCharStream :virtual public OByteStream
{
public:
	//==============================================================================================
	// 函数：构造
	// 参数：out - 输出字节流源
	// 说明：无
	//==============================================================================================
	OCharStream(OByteStream& out) :_out(out), _status(true) {}

public:
	operator bool() { return _status; }

public:
	/***********************************************************************************************
	 * 输出运算符重载
	 **********************************************************************************************/

	 // 基础输出
	OCharStream& operator<<(char c);
	OCharStream& operator<<(const char* ps);
	OCharStream& operator<<(uint32_t n);
	OCharStream& operator<<(int32_t n);
	OCharStream& operator<<(float n);

	// 增强输出
	OCharStream& operator<<(bool b) { return *this << (b ? "true" : "false"); }
	OCharStream& operator<<(void* p);
	OCharStream& operator<<(uint8_t n) { return *this << (uint32_t)n; }
	OCharStream& operator<<(uint16_t n) { return *this << (uint32_t)n; }
	OCharStream& operator<<(int8_t n) { return *this << (int32_t)n; }
	OCharStream& operator<<(int16_t n) { return *this << (int32_t)n; }
	OCharStream& operator<<(const std::string& str) { return *this << str.c_str(); }
#if ENABLE_SIZE_T_OVERLOAD
	OCharStream& operator<<(size_t n);
#endif

public:
	//==============================================================================================
	// 函数：put
	// 参数：c - 放入字符
	// 返回：成功返回true；失败返回false，同时置错误状态。
	// 说明：往流中放入一个字符
	//==============================================================================================
	virtual bool put(Byte c) { return _out.put(c) ? true : _status = false; }

	//==============================================================================================
	// 函数：status
	// 参数：无
	// 返回：正常返回true，出错返回false。
	// 说明：流状态受传输错误、缓冲区溢出、转换失败等多种因素影响，请查看其他函数的说明。
	//==============================================================================================
	bool status() { return _status; }

	//==============================================================================================
	// 函数：clear
	// 参数：无
	// 返回：无
	// 说明：清除流的异常状态。
	//==============================================================================================
	void clear() { _status = true; }

private:
	OByteStream& _out;
	bool _status;
};



//==================================================================================================
// inlines: ICharStream
//==================================================================================================

inline ICharStream& ICharStream::operator>>(char& c)
{
	get((Byte*)&c);
	return *this;
}

inline ICharStream& ICharStream::operator>>(char* buf_p)
{
	get_section(buf_p, SIZE_MAX);
	return *this;
}

inline ICharStream& ICharStream::operator>>(uint8_t& n)
{
	uint32_t temp;
	*this >> temp;
	if (temp <= UINT8_MAX)
		n = temp;
	else
		_status = false;
	return *this;
}

inline ICharStream& ICharStream::operator>>(uint16_t& n)
{
	uint32_t temp;
	*this >> temp;
	if (temp <= UINT16_MAX)
		n = temp;
	else
		_status = false;
	return *this;
}

inline ICharStream& ICharStream::operator>>(int8_t& n)
{
	int32_t temp;
	*this >> temp;
	if (INT8_MIN <= temp && temp <= INT8_MAX)
		n = temp;
	else
		_status = false;
	return *this;
}

inline ICharStream& ICharStream::operator>>(int16_t& n)
{
	int32_t temp;
	*this >> temp;
	if (INT16_MIN <= temp && temp <= INT16_MAX)
		n = temp;
	else
		_status = false;
	return *this;
}

inline ICharStream& ICharStream::operator>>(std::string& str)
{
	str = std::move(get_full_section());
	return *this;
}

#if ENABLE_SIZE_T_OVERLOAD

inline ICharStream& ICharStream::operator>>(size_t& n)
{
	uint32_t temp;
	*this >> temp;
	if (temp <= SIZE_MAX)
		n = temp;
	else
		_status = false;
	return *this;
}

#endif



//==================================================================================================
// inlines: OCharStream
//==================================================================================================

inline OCharStream& OCharStream::operator<<(char c)
{
	put(c);
	return *this;
}

#if ENABLE_SIZE_T_OVERLOAD

inline OCharStream& OCharStream::operator<<(size_t n)
{
	if (n > UINT32_MAX)
		_status = false;
	else
		*this << (uint32_t)n;
	return *this;
}

#endif



#undef ENABLE_SIZE_T_OVERLOAD
