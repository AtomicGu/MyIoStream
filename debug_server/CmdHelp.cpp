//==================================================================================================
// 文件：CmdHelp.cpp
// 作者：顾宇浩
// 简介：帮助命令类
// 上次修改：2020-6-16 (v2.0.1)
//==================================================================================================

#include "CmdHelp.h"
#include "lib/stream/str_stream.h"



bool CmdHelp::operator()(ICharStream& in, OCharStream out)
{
	auto cmd = in.get_full_section();
	if (cmd.empty())  // 为空打印所有可用指令
	{
		out << "All commands:\n\
  (specify command to show detailed info)\n\
\n";
		for (auto& i : _cmdMap)
		{
			out << "    " << i.first << " : ";
			// 只打印DebugCmd::info的第一行
			for (const char* p = i.second->info(); *p != '\n' && *p != '\0'; ++p)
			{
				out << *p;
			}
			out << '\n';
		}
		out << "\n  (Total:" << _cmdMap.size() << ")\n";
	}
	else  // 否则显示对应指令的信息
	{
		auto i = _cmdMap.find(cmd);
		if (i == _cmdMap.end())
		{
			out << "No such command: \"" << cmd << "\"\n";
			return true;
		}
		out << "Command \"" << cmd << "\" : {\n\n";
		out << i->second->info();
		out << "\n}\n";
	}
	return true;
}
