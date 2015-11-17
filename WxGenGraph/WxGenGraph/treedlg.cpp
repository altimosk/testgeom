#include <wx/wx.h>
#include <wx/treectrl.h>
#include "gentree.h"
#include "treedlg.h"

struct TreeNodeBackPtr : public wxTreeItemData
{
	TreeNodeBackPtr(TreeNode* n) : node(n) {}
	TreeNode* node;
};

static void AddChildren(TreeNode* n, wxTreeCtrl& t, wxTreeItemId& id)
{
	for (TreeNode* c : n->Children())
	{
		wxTreeItemId cid = t.AppendItem(id, c->Name(), -1, -1, new TreeNodeBackPtr(c));
		AddChildren(c, t, cid);
	}
}

class TreeSelector : public wxDialog
{
public:
	TreeSelector(char const * title, TreeNode* root) : 
		wxDialog(0, -1, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
		tree(this)
	{
		selected.Unset();
		wxTreeItemId id = tree.AddRoot(root->Name(), -1, -1, new TreeNodeBackPtr(root));
		AddChildren(root, tree, id);
		Bind(wxEVT_TREE_ITEM_ACTIVATED, &TreeSelector::ActivateNode, this);
	}

	TreeNode* SelectedNode() const
	{
		if (selected.IsOk())
		{
			TreeNodeBackPtr* ptr = static_cast<TreeNodeBackPtr*>(tree.GetItemData(selected));
			return ptr->node;
		}
		return 0;
	}

	TreeNode* SelectNode()
	{
		ShowModal();
		return SelectedNode();
	}

private:
	wxTreeItemId selected;
	wxTreeCtrl tree;
	void ActivateNode(wxTreeEvent& evt)
	{
		if (selected.IsOk())
			tree.SetItemBold(selected, false);

		wxTreeItemId newSelected = evt.GetItem();
		if (newSelected != selected)
		{
			selected = newSelected;
			tree.SetItemBold(selected, true);
		}
		else
			selected.Unset();
	}
};

TreeNode* SelectNode(TreeNode* root, char const * title)
{
	TreeSelector ts(title ? title : "Select Node", root);
	return ts.SelectNode();
}
