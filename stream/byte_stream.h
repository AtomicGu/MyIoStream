﻿//==================================================================================================
// 文件：byte_stream.h
// 作者：顾宇浩
// 简介：字节流抽象接口
// 上次修改：2020-5-8 (v1.0.0)
//==================================================================================================

#pragma once
#include <cstdint>
#include "stream.hpp"


//==================================================================================================
// 类名：Byte
// 说明：元素类
//==================================================================================================
using Byte = uint8_t;


//==================================================================================================
// 类名：IByteStream（纯虚）
// 说明：同步输入字节流类
//==================================================================================================
using IByteStream = IStream<Byte>;


//==================================================================================================
// 类名：OByteStream（纯虚）
// 说明：同步输出字节流类
//==================================================================================================
using OByteStream = OStream<Byte>;


//==================================================================================================
// 类名：AsyncIByteStream（纯虚）
// 说明：异步输入字节流类
//==================================================================================================
using AsyncIByteStream = AsyncIStream<Byte>;


//==================================================================================================
// 类名：AsyncOByteStream（纯虚）
// 说明：异步输出字节流类
//==================================================================================================
using AsyncOByteStream = AsyncOStream<Byte>;
