//==================================================================================================
// 文件：CmdGroup.h
// 作者：顾宇浩
// 简介：组命令类
// 上次修改：2020-5-22 (v1.0.0)
//==================================================================================================

#include "CmdGroup.h"



bool CmdGroup::operator()(ICharStream& in, OCharStream out)
{
	auto subCmd = in.get_full_section();

	// 查找指令
	auto cmdIter = _cmdMap.find(subCmd);

	// 无效指令处理
	if (_cmdMap.end() == cmdIter)
	{
		out << "invalid sub command: " << subCmd << '\n';
		return true;
	}

	// 执行子指令并返回
	return (*cmdIter->second)(in, out);
}
