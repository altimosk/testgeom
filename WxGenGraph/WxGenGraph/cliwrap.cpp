#include <wx/wx.h>
#include "cliwrap.h"
#include "gengraph.h"

void ClientWrap::Unwrap()
{
if (dll)
	{
		if (ggf)
			ggf(0);
		ggf = 0;

		FreeLibrary(dll);
		dll = 0;
	}
}

bool ClientWrap::ReInit(GenericGraphics* gg)
{
	Unwrap();
	wxString filename = wxFileSelector(wxT("Select a DLL"), wxEmptyString, wxEmptyString, wxEmptyString, wxT("*.dll"));
	if (filename.empty())
		return false;

	HMODULE dll = LoadLibrary(filename);
	if (!dll)
		return false;
	
	ggf = (SetGGFunc) ::GetProcAddress(dll, "SetGenericGraphics");
	if (!ggf)
		return false;
	ggf(gg);

	typedef bool(*init_func_ptr)();
	if (init_func_ptr init_func = (init_func_ptr)::GetProcAddress(dll, "init_unit_test"))
		(*init_func)();

	return true;
}
