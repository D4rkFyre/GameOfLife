#pragma once
#include <wx/wx.h>
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

class SettingsDialog : public wxDialog 
{
private:
	wxSpinCtrl* gridSpinner = nullptr;
	wxSpinCtrl* timeSpinner = nullptr;
	wxColourPickerCtrl* liveColourPicker = nullptr;
	wxColourPickerCtrl* deadColourPicker = nullptr;
	wxColourPickerCtrl* gridColourPicker = nullptr;

	Settings* dSettingsObj = nullptr;

public:
	SettingsDialog(wxWindow* parent, Settings* settings);

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

