//==================================================================================================
// �ļ���CmdGroup.h
// ���ߣ������
// ��飺��������
// �ϴ��޸ģ�2020-5-22 (v1.0.0)
//==================================================================================================

#include "CmdGroup.h"



bool CmdGroup::operator()(ICharStream& in, OCharStream out)
{
	auto subCmd = in.get_full_section();

	// ����ָ��
	auto cmdIter = _cmdMap.find(subCmd);

	// ��Чָ���
	if (_cmdMap.end() == cmdIter)
	{
		out << "invalid sub command: " << subCmd << '\n';
		return true;
	}

	// ִ����ָ�����
	return (*cmdIter->second)(in, out);
}
