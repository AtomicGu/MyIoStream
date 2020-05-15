﻿//==================================================================================================
// 文件：CmdExit.h
// 作者：顾宇浩
// 简介：退出命令类
// 上次修改：2020-5-15 (v1.0.1)
//==================================================================================================

#pragma once
#include "debug_server.h"



//==================================================================================================
// 类名：CmdExit
// 说明：一个简单的退出调试服务器的命令，忽略任何附加参数
//==================================================================================================
class CmdExit :public DebugCmd
{
public:
	//==============================================================================================
	// 函数：info
	// 参数：无
	// 返回：存放详情信息的C字符串
	// 说明：这个函数是对DebugCmd接口的实现
	//==============================================================================================
	virtual const char* info();

	//==============================================================================================
	// 函数：operator()
	// 参数：
	//		in - 参数输入流
	//		out - 命令使用的输出流
	//
	// 返回：始终返回false
	// 说明：这个函数是对DebugCmd接口的实现
	//==============================================================================================
	virtual bool operator()(ICharStream& in, OCharStream out) { return false; }
};



//==================================================================================================
// inlines: CmdExit
//==================================================================================================

inline const char* CmdExit::info()
{
	return R"(exit & close the server

Usage:
  <$cmd>
)";
}
