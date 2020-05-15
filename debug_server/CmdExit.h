//==================================================================================================
// �ļ���CmdExit.h
// ���ߣ������
// ��飺�˳�������
// �ϴ��޸ģ�2020-5-15 (v1.0.0)
//==================================================================================================

#pragma once
#include "debug_server.h"



//==================================================================================================
// ������CmdExit
// ˵����һ���򵥵��˳����Է���������������κθ��Ӳ���
//==================================================================================================
class CmdExit :public DebugCmd
{
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
	// ���أ�ʼ�շ���false
	// ˵������������Ƕ�DebugCmd�ӿڵ�ʵ��
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
  <$name>
)";
}