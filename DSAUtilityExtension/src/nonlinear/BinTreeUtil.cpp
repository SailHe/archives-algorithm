#include "CompleteBinTree.h"

namespace BinTreeUtil{
	void calcPostOrder(Tree::string const &sPre, Tree::string const &sMed, Tree::string &sPostBuffer) {
		//�����еĸ�����λ��
		Tree::string::size_type medRootSub = sMed.find(sPre[0]);
		//�������ĳ���
		Tree::string::size_type leftSubLen = medRootSub;
		/*
		A B CD	ǰ��(Root Left Right)
		B A DC	����(Left Root Right)

		BDC A	����(Left Right Root)
		*/

		//�����������������������ݹ�
		if (leftSubLen > 0) {
			//�����һ��ֵ(root_)�ұ�ȡ�������ĳ��ȼ���������
			Tree::string sPreLeftSub = sPre.substr(1, leftSubLen);
			//root_�����
			Tree::string sMedLeftSub = sMed.substr(0, leftSubLen);
			calcPostOrder(sPreLeftSub, sMedLeftSub, sPostBuffer);
		}
		else {
			//do nothing
		}

		//�����������������������ݹ�
		if (medRootSub + 1 < sMed.size()) {
			//�����һ��ֵ(root_)���ұ���������������ȡ���м���������
			Tree::string sPreRightSub = sPre.substr(1 + leftSubLen);
			//root_���ұ�
			Tree::string sMedRightSub = sMed.substr(medRootSub + 1);
			calcPostOrder(sPreRightSub, sMedRightSub, sPostBuffer);
		}
		else {
			//do nothing
		}

		if (medRootSub != Tree::string::npos) {
			//����root_
			sPostBuffer += sMed.at(medRootSub);
		}
		else {
			//do nothing
		}
	}

}


