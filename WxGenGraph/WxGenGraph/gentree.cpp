#define _CRT_SECURE_NO_WARNINGS
#include "gentree.h"
#include <iostream>
#include <string>
#include "tokens.h"
#include <cstring>

char* TreeNode::strdup(char const * c)
{
	if (!c)
		return 0;

	size_t len = std::strlen(c) + 1;
	char* res = new char[len];
	std::strcpy(res, c);
	return res;
}

static void AddChildren(Tokens& tok, TreeNode* parent)
{
	TreeNode* cur = 0;
	while (tok.Good())
	{
		std::string str;
		tok >> str;
		if (!tok.Good() || str.at(0) == '}')
			return;
		if (str.at(0) == '{')
		{
			if (!cur)
				cur = parent->AddChild(TreeNode::strdup(""));
			AddChildren(tok, cur);
			continue;
		}

		cur = parent->AddChild(TreeNode::strdup(str));
	}
}

TreeNode* MakeTree(std::istream& in)
{
	Tokens tok(in);
	tok.AddDelims("{}");
	TreeNode* root = new TreeNode(TreeNode::strdup(""));
	AddChildren(tok, root);
	return root;
}
void TreeNode::Path(std::string& path) const
{
	path.erase();

	for (TreeNode const * n = this; n; n = n->Parent())
	{
		if (!path.empty())
			path = "\\" + path;
		path = n->Name() + path;
	}

}
