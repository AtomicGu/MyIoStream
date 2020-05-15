//==================================================================================================
// �ļ���CmdHelp.h
// ���ߣ������
// ��飺����������
// �ϴ��޸ģ�2020-5-15 (v1.0.0)
//==================================================================================================

#pragma once
#include "debug_server.h"



//==================================================================================================
// ������CmdHelp
// ˵����
//		һ���򵥵İ���ָ���ѡ�ؽ���һ������ָ������Ϊ����
//		��ָ��û�в��������ӡָ��������п���ָ�
//		����������Ч��ָ���������ӡ��Ӧ�������ϸ��Ϣ��
//==================================================================================================
class CmdHelp :public DebugCmd
{
public:
	//==============================================================================================
	// ���������죨���أ�
	// ������ds - ���Է���������
	// ˵������
	//==============================================================================================
	CmdHelp(DebugServer& ds) :_cmdMap(ds.cmd_map()) {}

	//==============================================================================================
	// ���������죨���أ�
	// ������cmdMap - ����ָ�������
	// ˵������
	//==============================================================================================
	CmdHelp(DebugCmdMap& cmdMap) :_cmdMap(cmdMap) {}

public:
	//==============================================================================================
	// ������info
	// ��������
	// ���أ����������Ϣ��C�ַ���
	// ˵������������Ƕ�DebugCmd�ӿڵ�ʵ��
	//==============================================================================================
	virtual const char* info();

	//==============================================================================================
	// ������operator()
	// ������
	//		in - ����������
	//		out - ����ʹ�õ������
	//
	// ���أ�ʼ�շ���true
	// ˵������������Ƕ�DebugCmd�ӿڵ�ʵ��
	//==============================================================================================
	virtual bool operator()(ICharStream& in, OCharStream out);

private:
	DebugCmdMap& _cmdMap;
};



//==================================================================================================
// inlines: CmdHelp
//==================================================================================================

inline const char* CmdHelp::info()
{
	return R"(show help info

Usage:
  <$name> [cmdName]

If no command specified, show all avaliable commands.
)";
}