﻿//==================================================================================================
// 文件：DebugServer.cpp
// 作者：顾宇浩
// 简介：基于DebugIoStream类的调试服务器类
// 上次修改：2020-6-17 (v1.0.1)
//==================================================================================================

#include "DebugServer.h"
#include "lib/stream/str_stream.h"


// HACK: 提供不限长指令的版本
#define MAX_CMD_LENTH	128		// 最大指令长度


// 判断是否是分号
static bool is_semicolon(char c)
{
	return c == ';';
}

DebugServer::ReturnCode DebugServer::main()
{
	// 输出启动信息
	_ios << "\n[ DEBUG SERVER STARTED ]\n\n";

	// 主循环
	char cmdBuf[MAX_CMD_LENTH];
	while (true)
	{
		// 每条指令以“"”开始
		do
		{
			if (!_ios.get((Byte*)cmdBuf))
				return ReturnCode::kIostreamBroken;
		} while (*cmdBuf != ':');

		// 获取指令，指令以“;”结束
		if (!_ios.get_section(cmdBuf, sizeof(cmdBuf), is_semicolon))
			return ReturnCode::kIostreamBroken;

		// 提取指令关键字
		IStrStream iss(cmdBuf);
		std::string cmdKey = iss.get_full_section();

		// 查找指令
		auto cmdIter = _cmdMap.find(cmdKey);

		// 无效指令处理
		if (_cmdMap.end() == cmdIter)
		{
			invalid_cmd(cmdKey);
			continue;
		}

		// 正常执行
		_ios << "[ EXECUTING: \"" << cmdBuf << "\"]\n\n";
		if ((*cmdIter->second)(iss, _ios))
		{
			_ios << "\n[ CONTINUE ]\n\n";
		}
		else
		{
			_ios << "\n[ TERMINATE ]\n\n";
			break;
		}
	}

	// 输出停止信息
	_ios << "\n[ DEBUG SERVER CLOSED ]\n\n";
	return ReturnCode::kOK;
}
