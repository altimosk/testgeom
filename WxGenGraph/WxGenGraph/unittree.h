#ifndef UNITTREE_H
#define UNITTREE_H
#include "gentree.h"

class UnitTreeNode : public TreeNode
{
public:
	unsigned long id;

	UnitTreeNode(char* nm) : TreeNode(nm), id(0) {
		}

	UnitTreeNode* AddChild(char* nm)
	{
		UnitTreeNode* c = new UnitTreeNode(nm);
		c->parent = this;
		children.push_back(c);
		return c;
	}

	UnitTreeNode* Parent() const {
		return static_cast<UnitTreeNode*> (parent);
	}

	std::vector<UnitTreeNode*> const & Children() const {
		return (std::vector<UnitTreeNode*>&)children;
	}
};

UnitTreeNode* MakeCurrentUnitTree();
int RunUnitTests(unsigned long id);

#endif