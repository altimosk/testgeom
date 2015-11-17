#ifndef GENTREE_H
#define GENTREE_H
#include <vector>
#include <iostream>
#include <string>

class TreeNode
{
public: 
	TreeNode(char* nm) : name(nm), parent(0){
	}

	TreeNode* AddChild(char* nm)
	{
		TreeNode* c = new TreeNode(nm);
		c->parent = this;
		children.push_back(c);
		return c;
	}

	char const * Name() const {
		return name; }

	TreeNode* Parent() const {
		return parent;
	}

	void Path(std::string&) const;

	std::vector<TreeNode*> const & Children() const {
	return children; }

	~TreeNode()
	{
		for (TreeNode* c : children)
			delete c;
		delete name;
	}

private:
	char * name;
protected:
	std::vector<TreeNode*> children;
	TreeNode* parent;
public:
	static char* strdup(char const * c);
	static char* strdup(std::string const &s) {
		return strdup(s.c_str()); }
};

TreeNode* MakeTree(std::istream& in);

#endif


