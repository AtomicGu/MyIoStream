//==================================================================================================
// 文件：numformat.cpp
// 作者：顾宇浩
// 简介：
//		安全的数值-字符串转换函数库
//		提供可选的优化预编译选项
// 上次修改：2020-5-8 (v1.0.0)
//==================================================================================================

#include "numformat.h"
#include <cmath>



//==================================================================================================
// ntoas
//==================================================================================================

static const char kNAMap_a[] = {
	'0','1','2','3',
	'4','5','6','7',
	'8','9','a','b',
	'c','d','e','f',
};

char* ntoa(uint32_t n, char* buf_p, size_t bufSize, uint8_t radix)
{
	// 检查radix取值
	if (radix < 2 || radix > 16)
		return nullptr;

	// 字符串末尾置'\0'
	char* p = buf_p + bufSize;
	*--p = '\0';

	// 从后向前生成字符串
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[n % radix];
		n /= radix;
	} while (0 != n);
	return p;
}

char* ntoa(int32_t n, char* buf_p, size_t bufSize, uint8_t radix)
{
	// 检查radix取值
	if (radix < 2 || radix > 16)
		return nullptr;

	// 字符串末尾置'\0'
	char* p = buf_p + bufSize;
	*--p = '\0';

	// 正数化
	bool sign = n < 0;
	if (sign)
		n = -n;

	// 生成无符号数
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[n % radix];
		n /= radix;
	} while (0 != n);

	// 处理符号
	if (sign)
	{
		if (p == buf_p)
			return nullptr;
		*--p = '-';
	}

	return p;
}

#if 0
// 特化的以10为基数的浮点数格式化函数
// 因为浮点数输出时需要计算log2f，故此优化用以提高效率
char* ntoa(float n, char* buf_p, size_t bufSize, uint8_t radix)
{
	// 局部变量前置声明
	bool sign;
	bool rankSign;
	int16_t rank;
	char* p;
	uint32_t value;

	// 检查radix取值
	if (10 != radix)
		return nullptr;

	// 字符串末尾置'\0'
	p = buf_p + bufSize;
	*--p = '\0';

	// 判别符号
	sign = n < 0;

	// nan处理
	if (std::isnan(n))
	{
		if (bufSize < 4)
			return nullptr;
		*--p = 'n';
		*--p = 'a';
		*--p = 'n';
		goto SIGN;
	}

	// inf处理
	if (std::isinf(n))
	{
		if (bufSize < 4)
			return nullptr;
		*--p = 'f';
		*--p = 'n';
		*--p = 'i';
		goto SIGN;
	}

	// 单独处理0
	if (0 == n)
	{
		if (bufSize < 4)
			return nullptr;
		*--p = '0';
		*--p = '.';
		*--p = '0';
		goto SIGN;
	}

	// 正数化
	if (sign)
		n = -n;

	// 规格化到[1/10,10)
	// 同时获得阶数
	rank = 0;
	if (n >= 10)
	{
		do
		{
			n /= 10;
			++rank;
		} while (n >= 10);
	}
	else
	{
		float lb = 0.1;
		while (n < lb)
		{
			n *= 10;
			--rank;
		}
	}

	// 生成阶数
	rankSign = rank < 0;
	if (rankSign)
		rank = -rank;
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[rank % 10];
		rank /= 10;
	} while (0 != rank);
	if (p == buf_p)
		return nullptr;
	if (rankSign)
	{
		*--p = '-';
		if (p == buf_p)
			return nullptr;
	}
	*--p = '^';

	// 生成数值
	value = (uint32_t)(n * 10000000);
	do
	{
		if (0 != value % 10)
			break;
		value /= 10;
	} while (true);  // 略去末尾的'0'
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[value % 10];
		value /= 10;
	} while (0 != value);
	if (*(p + 1) == '^')  // 如果没有小数域
	{
		if (p == buf_p)
			return nullptr;
		*(p - 1) = *p;
		*p = '0';
		--p;
	}
	if (p == buf_p)
		return nullptr;
	*(p - 1) = *p;
	*p = '.';
	--p;

SIGN:
	// 处理符号
	if (sign)
	{
		if (p == buf_p)
			return nullptr;
		*--p = '-';
	}

	return p;
}

#else
char* ntoa(float n, char* buf_p, size_t bufSize, uint8_t radix)
{
	// 局部变量前置声明
	bool sign;
	bool rankSign;
	int16_t rank;
	char* p;
	uint32_t value;

	// 检查radix取值
	if (radix < 2 || radix > 16)
		return nullptr;

	// 字符串末尾置'\0'
	p = buf_p + bufSize;
	*--p = '\0';

	// 判别符号
	sign = n < 0;

	// nan处理
	if (std::isnan(n))
	{
		if (bufSize < 4)
			return nullptr;
		*--p = 'n';
		*--p = 'a';
		*--p = 'n';
		goto SIGN;
	}

	// inf处理
	if (std::isinf(n))
	{
		if (bufSize < 4)
			return nullptr;
		*--p = 'f';
		*--p = 'n';
		*--p = 'i';
		goto SIGN;
	}

	// 单独处理0
	if (0 == n)
	{
		if (bufSize < 4)
			return nullptr;
		*--p = '0';
		*--p = '.';
		*--p = '0';
		goto SIGN;
	}

	// 正数化
	if (sign)
		n = -n;

	// 规格化到[1/radix,radix)
	// 同时获得阶数
	rank = 0;
	if (n >= radix)
	{
		do
		{
			n /= radix;
			++rank;
		} while (n >= radix);
	}
	else
	{
		float lb = (float)1 / radix;
		while (n < lb)
		{
			n *= radix;
			--rank;
		}
	}

	// 生成阶数
	rankSign = rank < 0;
	if (rankSign)
		rank = -rank;
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[rank % radix];
		rank /= radix;
	} while (0 != rank);
	if (p == buf_p)
		return nullptr;
	if (rankSign)
	{
		*--p = '-';
		if (p == buf_p)
			return nullptr;
	}
	*--p = '^';

	// 生成数值
	value = (uint32_t)(n * pow(radix, (int32_t)(24 / log2f(radix))));
	do
	{
		if (0 != value % radix)
			break;
		value /= radix;
	} while (true);  // 略去末尾的'0'
	do
	{
		if (--p < buf_p)
			return nullptr;
		*p = kNAMap_a[value % radix];
		value /= radix;
	} while (0 != value);
	if (*(p + 1) == '^')  // 如果没有小数域
	{
		if (p == buf_p)
			return nullptr;
		*(p - 1) = *p;
		*p = '0';
		--p;
	}
	if (p == buf_p)
		return nullptr;
	*(p - 1) = *p;
	*p = '.';
	--p;

SIGN:
	// 处理符号
	if (sign)
	{
		if (p == buf_p)
			return nullptr;
		*--p = '-';
	}

	return p;
}

#endif



//==================================================================================================
// atons
//==================================================================================================

// 字符转数位
static uint8_t ctod(char c, uint8_t radix)
{
	uint8_t temp;
	if (c < '0')
		temp = UINT8_MAX;
	else if (c <= '9')
		temp = c - '0';
	else if (c < 'A')
		temp = UINT8_MAX;
	else if (c <= 'Z')
		temp = 10 + c - 'A';
	else if (c < 'a')
		temp = UINT8_MAX;
	else if (c <= 'z')
		temp = 10 + c - 'a';
	else
		temp = UINT8_MAX;
	if (temp >= radix)
		return UINT8_MAX;
	else
		return temp;
}

#if 0
// 基数法（不安全的、有BUG）
bool aton(const char* a_ps, uint32_t* n_p, uint8_t radix)
{
	// 找到字符串末尾
	const char* p = a_ps;
	while ('\0' != *p)
		++p;

	// 求补数
	uint32_t base = 1;
	uint32_t comp = UINT32_MAX;
	while (--p >= a_ps)
	{
		uint32_t temp = *p - '0';
		if (temp < 0 || temp >= radix)
			return false;  // 无效字符
		temp *= base;
		if (temp > comp)
			return false;  // 溢出（这个判断是不完全的）
		comp -= temp;
		base *= radix;
	}

	// 再求补数的补
	*n_p = UINT32_MAX - comp;
	return true;
}

#else
// 累乘法
bool aton(const char* a_ps, uint32_t* n_p, uint8_t radix)
{
	// 检查radix取值
	if (radix > 36)
		return false;

	// 略去无效零
	while (*a_ps == '0')
		++a_ps;

	// 累乘法计算数值
	uint64_t ans = 0;
	while (true)
	{

		uint8_t temp = ctod(*a_ps, radix);
		if (temp == UINT8_MAX)
		{
			if (*a_ps == '\0')
				goto END;
			return false;  // 无效字符
		}
		ans *= radix;
		ans += temp;
		if (ans > UINT32_MAX)  // 判溢出
			return false;
		++a_ps;
	}

END:
	*n_p = ans;
	return true;
}

#endif

#if 0
// 基数法（有BUG、不安全）
bool aton(const char* a_ps, int32_t* n_p, uint8_t radix)
{
	// 找到字符串末尾
	const char* p = a_ps;
	while ('\0' != *p)
		++p;

	// 求无符号数的补
	uint32_t base = 1;
	uint32_t comp = UINT32_MAX;
	while (--p >= a_ps)
	{
		uint32_t temp = *p - '0';
		if (temp < 0 || temp >= radix)
		{
			if (p != a_ps)  // 若合法，则这个字符一定在开头
				return false;
			if (*p == '+')  // 且不是'+'
				goto POSITIVE;
			if (*p == '-')  // 就是'-'
				goto NEGTIVE;
			return false;  // 无效字符
		}
		temp *= base;
		if (temp > comp)
			return false;  // 溢出
		comp -= temp;
		base *= radix;
	}

	// 正数处理
POSITIVE:
	comp = UINT32_MAX - comp;
	if (comp > INT32_MAX)
		return false;
	*n_p = comp;
	return true;

NEGTIVE:
	comp = UINT32_MAX - comp;
	if (comp > -INT32_MIN)
		return false;
	*n_p = -(int32_t&)comp;
	return true;
}

#else
// 累乘法
bool aton(const char* a_ps, int32_t* n_p, uint8_t radix)
{
	// 检查radix取值
	if (radix > 36)
		return false;

	// 处理可能的符号
	bool sign = (*a_ps == '-');
	if (sign || '+' == *a_ps)
	{
		++a_ps;
	}

	// 略去无效零
	while (*a_ps == '0')
		++a_ps;

	// 累乘法计算数值
	uint64_t ans = 0;
	if (sign)  // 负数
	{
		while (true)
		{
			uint8_t temp = ctod(*a_ps, radix);
			if (temp == UINT8_MAX)
			{
				if (*a_ps == '\0')
					goto END;
				return false;  // 无效字符
			}
			ans *= radix;
			ans -= temp;
			if (ans < (uint64_t)INT32_MIN)  // 判溢出
				return false;
			++a_ps;
		}
	}
	else  // 正数
	{
		while (true)
		{
			uint8_t temp = ctod(*a_ps, radix);
			if (temp == UINT8_MAX)
			{
				if (*a_ps == '\0')
					goto END;
				return false;  // 无效字符
			}
			ans *= radix;
			ans += temp;
			if (ans > INT32_MAX)  // 判溢出
				return false;
			++a_ps;
		}
	}

END:
	// 结果
	*n_p = (int32_t)ans;
	return true;
}

#endif

bool aton(const char* a_ps, float* n_p, uint8_t radix)
{
	// 检查radix取值
	if (radix > 36)
		return false;

	// 处理可能的符号
	bool sign = (*a_ps == '-');
	if (sign || '+' == *a_ps)
	{
		++a_ps;
	}

	// 略去无效零
	while (*a_ps == '0')
		++a_ps;

	// 累乘法计算数值
	uint32_t value = 0;  // 数值
	int32_t rank = 0;  // 阶数
	while (true)
	{
		uint8_t temp = ctod(*a_ps, radix);
		if (temp == UINT8_MAX)
		{
			if (*a_ps == '\0')
				goto END;
			if (*a_ps == '.')
				goto POINT;
			if (*a_ps == '^')
				goto RANK;
			return false;  // 无效字符
		}
		value *= radix;
		value += temp;
		if (value > 0x11111111)  // 判溢出（浮点数有效数位24位）
			return false;
		++a_ps;
	}

POINT:
	// 略过'.'接着计算数值
	while (true)
	{
		++a_ps;
		uint8_t temp = *a_ps - '0';
		if (temp < 0 || temp >= radix)
		{
			if (*a_ps == '\0')
				goto END;
			if (*a_ps == '^')
				goto RANK;
			return false;  // 无效字符
		}
		value *= radix;
		value += temp;
		if (value > 0x11111111)  // 判溢出（浮点数有效数位24位）
			return false;
		--rank;  // 每识别一个字符，阶数减1
	}

RANK:
	// 修正阶数
	int32_t rank2;
	if (!aton(++a_ps, &rank2, radix))
		return false;
	rank += rank2;

END:
	float ans = (float)value * std::pow((float)radix, (float)rank);
	if (sign)
		ans = -ans;
	if (0 != ans && !std::isnormal(ans))
		return false;
	*n_p = ans;
	return true;
}
