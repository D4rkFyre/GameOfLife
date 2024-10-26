#include "SettingsDialog.h"
#include "Settings.h"
#include "DrawingPanel.h"
#include "MainWindow.h"
#include <wx/wx.h>

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
	EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

// Constructor for Settings options menu
SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings) 
	: wxDialog(parent, wxID_ANY, "Game Options", wxPoint(200, 200), wxSize(300, 350)), dSettingsObj(settings) {
	
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);

	wxBoxSizer* gridSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* timeSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* liveSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* deadSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* gridColourSizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxSizer* okCancel = CreateButtonSizer(wxOK | wxCANCEL);

	gridSpinner = new wxSpinCtrl(this, wxID_ANY);
	gridSpinner->SetRange(1, 1000);
	gridSpinner->SetValue(dSettingsObj->GetGridSize());
	wxStaticText* gridSpinnerLabel = new wxStaticText(this, wxID_ANY, "   Grid Size");
	gridSpinnerLabel->SetMinSize(wxSize(150, 50));

	timeSpinner = new wxSpinCtrl(this, wxID_ANY);
	timeSpinner->SetRange(0, 1000);
	timeSpinner->SetValue(dSettingsObj->GetTimerInterval());
	wxStaticText* timeSpinnerLabel = new wxStaticText(this, wxID_ANY, "   Timer Increment(ms)");
	timeSpinnerLabel->SetMinSize(wxSize(150, 50));
	
	liveColourPicker = new wxColourPickerCtrl(this, wxID_ANY);
	liveColourPicker->SetColour(dSettingsObj->GetLivingCellColour());
	wxStaticText* liveColourLabel = new wxStaticText(this, wxID_ANY, "   Live Cell Colour");
	liveColourLabel->SetMinSize(wxSize(150, 50));

	deadColourPicker = new wxColourPickerCtrl(this, wxID_ANY);
	deadColourPicker->SetColour(dSettingsObj->GetDeadCellColour());
	wxStaticText* deadColourLabel = new wxStaticText(this, wxID_ANY, "   Dead Cell Colour");
	deadColourLabel->SetMinSize(wxSize(150, 50));

	gridColourPicker = new wxColourPickerCtrl(this, wxID_ANY);
	gridColourPicker->SetColour(dSettingsObj->GetGridColour());
	wxStaticText* gridColourLabel = new wxStaticText(this, wxID_ANY, "   Grid Cell Outline");
	gridColourLabel->SetMinSize(wxSize(150, 50));

	gridSizer->Add(gridSpinnerLabel);
	gridSizer->Add(gridSpinner);

	timeSizer->Add(timeSpinnerLabel);
	timeSizer->Add(timeSpinner);

	liveSizer->Add(liveColourLabel);
	liveSizer->Add(liveColourPicker);

	deadSizer->Add(deadColourLabel);
	deadSizer->Add(deadColourPicker);

	gridColourSizer->Add(gridColourLabel);
	gridColourSizer->Add(gridColourPicker);

	sizer->Add(gridSizer);
	sizer->Add(timeSizer);
	sizer->Add(liveSizer);
	sizer->Add(deadSizer);
	sizer->Add(gridColourSizer);
	sizer->Add(okCancel);
}

void SettingsDialog::OnOk(wxCommandEvent& event) {
	dSettingsObj->SetGridSize(gridSpinner->GetValue());
	dSettingsObj->SetTimerInterval(timeSpinner->GetValue());
	dSettingsObj->SetLivingCellColour(liveColourPicker->GetColour());
	dSettingsObj->SetDeadCellColour(deadColourPicker->GetColour());
	dSettingsObj->SetGridColour(gridColourPicker->GetColour());
	dSettingsObj->SaveSettings();
	EndModal(wxID_OK);
}

void SettingsDialog::OnCancel(wxCommandEvent& event) {
	EndModal(wxID_CANCEL);
}
