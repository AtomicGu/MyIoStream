﻿//==================================================================================================
// 文件：numformat.h
// 作者：顾宇浩
// 简介：安全的数值-字符串转换函数库
// 上次修改：2020-5-15 (v1.0.1)
//==================================================================================================

#pragma once
#include <cstdint>
#include <cstddef>


//==================================================================================================
// 函数：ntoa（重载）
// 参数：n - 转换数
//		buf_p - 缓冲区指针
//		bufSize - 缓冲区大小
//		radix - 基数，取值[2,16]
// 返回：字符串结果的头指针，为nullptr则表示转换失败。
// 说明：
//		1. 从buf_p+bufSize往前生成字符串，返回生成的字符串头指针。
//		2. 缓冲区过小、radix取值无效都会导致转换失败。
//		3. 这个函数是安全的。
//==================================================================================================
char* ntoa(uint32_t n, char* buf_p, size_t bufSize, uint8_t radix = 10);


//==================================================================================================
// 函数：ntoa（重载）
// 参数：n - 转换数
//		buf_p - 缓冲区指针
//		bufSize - 缓冲区大小
//		radix - 基数，取值[2,16]
// 返回：字符串结果的头指针，为nullptr则表示转换失败。
// 说明：
//		1. 从buf_p+bufSize往前生成字符串，返回生成的字符串头指针。
//		2. 缓冲区过小、radix取值无效都会导致转换失败。
//		3. 这个函数是安全的。
//==================================================================================================
char* ntoa(int32_t n, char* buf_p, size_t bufSize, uint8_t radix = 10);


//==================================================================================================
// 函数：ntoa（重载）
// 参数：n - 转换数
//		buf_p - 缓冲区指针
//		bufSize - 缓冲区大小
//		radix - 基数，取值[2,16]
// 返回：字符串结果的头指针，为nullptr则表示转换失败。
// 说明：
//		1. 从buf_p+bufSize往前生成字符串，返回生成的字符串头指针。
//		2. 缓冲区过小、radix取值无效都会导致转换失败。
//		3. 这个函数是安全的。
//		4. 输出为科学计数法形式。
//==================================================================================================
char* ntoa(float n, char* buf_p, size_t bufSize, uint8_t radix = 10);


//==================================================================================================
// 函数：aton（重载）
// 参数：a_ps - 字符串指针
//		n_p - 保存结果的指针
//		radix - 基数，取值不限
// 返回：接受返回true，拒绝返回false
// 说明：
//		1. 接受的正则式：
//			^[0-f]*$
//		2. 转换溢出、出现无效字符等都会造成转换失败。
//		3. 若转换失败，n_p指向内存将不会被修改。
//		4. 这个函数是安全的。
//==================================================================================================
bool aton(const char* a_ps, uint32_t* n_p, uint8_t radix = 10);


//==================================================================================================
// 函数：aton（重载）
// 参数：a_ps - 字符串指针
//		n_p - 保存结果的指针
//		radix - 基数，取值[0,36]
// 返回：接受返回true，拒绝返回false
// 说明：
//		1. 接受的正则式：
//			^(+|-)?[0-f]*$
//		2. 转换溢出、出现无效字符等都会造成转换失败。
//		3. 若转换失败，n_p指向内存将不会被修改。
//		4. 这个函数是安全的。
//==================================================================================================
bool aton(const char* a_ps, int32_t* n_p, uint8_t radix = 10);


//==================================================================================================
// 函数：aton（重载）
// 参数：a_ps - 字符串指针
//		n_p - 保存结果的指针
//		radix - 基数，取值[0,36]
// 返回：接受返回true，拒绝返回false
// 说明：
//		1. 接受的正则式：
//			^(+|-)?[0-f]*(.[0-f]*)?(^(+|-)[0-f]*)?$
//			此外，不接受非规格化浮点数
//		2. 转换溢出、精度丢失、出现无效字符等都会造成转换失败。
//		3. 若转换失败，n_p指向内存将不会被修改。
//		4. 这个函数是安全的。
//==================================================================================================
bool aton(const char* a_ps, float* n_p, uint8_t radix = 10);
