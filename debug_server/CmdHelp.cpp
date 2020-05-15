//==================================================================================================
// �ļ���CmdHelp.cpp
// ���ߣ������
// ��飺����������
// �ϴ��޸ģ�2020-5-15 (v1.0.0)
//==================================================================================================

#include "CmdHelp.h"
#include "str_stream.h"



bool CmdHelp::operator()(ICharStream& in, OCharStream out)
{
	auto cmd = in.get_full_section();
	if (cmd.empty())  // Ϊ�մ�ӡ���п���ָ��
	{
		out << "All commands:\n\
  (specify command to show detailed info)\n\
\n";
		for (auto& i : _cmdMap)
		{
			out << "    " << i.first << " : ";
			// ֻ��ӡDebugCmd::info�ĵ�һ��
			for (const char* p = i.second->info(); *p != '\n' && *p != '\0'; ++p)
			{
				out << *p;
			}
			out << '\n';
		}
		out << "\n  (Total:" << _cmdMap.size() << ")\n";
	}
	else  // ������ʾ��Ӧָ�����Ϣ
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
