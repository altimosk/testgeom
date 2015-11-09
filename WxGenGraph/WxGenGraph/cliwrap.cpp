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
		daf = 0;

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

	daf = (DrawAllFunc) ::GetProcAddress(dll, "DrawAll"); //optional, no failure

	ggf(gg);
	return true;
}
