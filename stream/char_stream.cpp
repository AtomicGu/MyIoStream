//==================================================================================================
// 文件：str_stream.cpp
// 作者：顾宇浩
// 简介：字符流
// 上次修改：2020-5-15 (v1.3.1)
//==================================================================================================

#include "char_stream.h"
#include <cstring>
#include "numformat.h"



//==================================================================================================
// ICharStream
//==================================================================================================

ICharStream& ICharStream::operator>>(char& c)
{
	// 略过开头连续的分隔符
	Byte temp;
	do
	{
		if (!get(&temp))  // 出错
			return *this;
	} while (is_whitespace(temp));

	// 再赋值，保证c不被误改
	c = temp;
	return *this;
}

ICharStream& ICharStream::operator>>(uint32_t& n)
{
	char buf_a[11];
	if (!(get_full_section(buf_a, sizeof(buf_a)) && aton(buf_a, &n)))
		_status = false;
	return *this;
}

ICharStream& ICharStream::operator>>(int32_t& n)
{
	char buf_a[12];
	if (!(get_full_section(buf_a, sizeof(buf_a)) && aton(buf_a, &n)))
		_status = false;
	return *this;
}

ICharStream& ICharStream::operator>>(float& n)
{
	char buf_a[20];
	if (!(get_full_section(buf_a, sizeof(buf_a)) && aton(buf_a, &n)))
		_status = false;
	return *this;
}

ICharStream& ICharStream::operator>>(bool& b)
{
	char buf_a[11];
	if (!get_full_section(buf_a, sizeof(buf_a)))
		return *this;
	uint32_t n;
	if (!aton(buf_a, &n))
	{
		if (0 == strcmp("true", buf_a))
			b = true;
		else if (0 == strcmp("false", buf_a))
			b = false;
		else
			_status = false;
	}
	else
	{
		b = (bool)n;
	}
	return *this;
}

size_t ICharStream::get_piece(char* buf_p, size_t bufSize, IsSeperator isSep)
{
	// 空缓冲情况处理
	if (0 == bufSize)
		return 0;

	// 略过开头连续的分隔符
	Byte temp;
	do
	{
		if (!get(&temp))  // 出错
			return SIZE_MAX;
	} while (isSep(temp));

	// 取到段尾
	size_t left = bufSize;
	do
	{
		*buf_p = temp;
		--left;
		if (0 == left)
			return bufSize;  // 未取尽
		++buf_p;
		if (!get(&temp))  // 出错
			return SIZE_MAX;
	} while (!isSep(temp));

	// 取尽则封尾
	*buf_p = '\0';
	return bufSize - left;
}

bool ICharStream::get_full_piece(char* buf_p, size_t bufSize, IsSeperator isSep)
{
	size_t re = get_piece(buf_p, bufSize, isSep);
	if (SIZE_MAX == re)
		return false;  // 出错，错误状态在get_section中被设置。
	if (bufSize == re)
	{
		Byte temp;
		do
		{
			if (!get(&temp))
				return false;
		} while (!isSep(temp));
		_status = false;  // 缓冲区空间不足，置错误状态
		return false;
	}
	return true;
}

std::string ICharStream::get_full_piece(IsSeperator isSep)
{
	// 略过开头连续的分隔符
	Byte temp;
	do
	{
		if (!get(&temp))  // 出错
			return std::string();
	} while (isSep(temp));

	// 取到段尾
	std::string re;
	do
	{
		re.push_back(temp);
		if (!get(&temp))  // 出错
			return std::string();
	} while (!isSep(temp));

	// 取尽则封尾
	return std::move(re);
}

bool ICharStream::skip_this_piece(IsSeperator isSep)
{
	Byte temp;
	do
	{
		if (!get(&temp))  // 出错
			return false;
	} while (!isSep(temp));

	return true;
}

bool ICharStream::skip_next_piece(IsSeperator isSep)
{
	// 略过开头连续的分隔符
	Byte temp;
	do
	{
		if (!get(&temp))  // 出错
			return false;
	} while (isSep(temp));

	// 略过本段
	do
	{
		if (!get(&temp))  // 出错
			return false;
	} while (!isSep(temp));

	return true;
}



//==================================================================================================
// OCharStream
//==================================================================================================

OCharStream& OCharStream::operator<<(const char* ps)
{
	while (*ps != '\0')
	{
		_out.put(*ps);
		++ps;
	}
	return *this;
}

OCharStream& OCharStream::operator<<(uint32_t n)
{
	char buf_a[11];
	char* p = ntoa(n, buf_a, sizeof(buf_a));
	if (!p)
	{
		_status = false;
		return *this;
	}
	return *this << p;
}

OCharStream& OCharStream::operator<<(int32_t n)
{
	char buf_a[12];
	char* p = ntoa(n, buf_a, sizeof(buf_a));
	if (!p)
	{
		_status = false;
		return *this;
	}
	return *this << p;
}

OCharStream& OCharStream::operator<<(float n)
{
	char buf_a[20];
	char* p = ntoa(n, buf_a, sizeof(buf_a));
	if (!p)
	{
		_status = false;
		return *this;
	}
	return *this << p;
}

OCharStream& OCharStream::operator<<(void* p)
{
	char buf_a[sizeof(p) * 2 + 3];
	char* ps = ntoa((uint32_t)p, buf_a, sizeof(buf_a));
	*--ps = 'x';
	*--ps = '0';
	return *this << ps;
}
