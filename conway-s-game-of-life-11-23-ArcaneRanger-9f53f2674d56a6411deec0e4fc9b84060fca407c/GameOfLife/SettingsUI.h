#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"
class SettingsUI :
    public wxDialog
{
private:
	wxColourPickerCtrl* pColorPickA = nullptr;
	wxColourPickerCtrl* pColorPickB = nullptr;
	wxColourPickerCtrl* pColorPickC = nullptr;
	wxStaticText* pTextLabelA = nullptr;
	wxStaticText* pTextLabelB = nullptr;
	wxStaticText* pTextLabelC = nullptr;
	wxStaticText* pTextLabelD = nullptr;
	wxStaticText* pTextLabelE = nullptr;
	wxBoxSizer* pBoxSizerB = nullptr;
	wxBoxSizer* pBoxSizerC = nullptr;
	wxBoxSizer* pBoxSizerD = nullptr;
	wxBoxSizer* pBoxSizerE = nullptr;
	wxBoxSizer* pBoxSizerF = nullptr;
	wxBoxSizer* pBoxSizerG = nullptr;
	wxSpinCtrl* pSpinnerA = nullptr;
	wxSpinCtrl* pSpinnerB = nullptr;
	wxSizer* pButtonSizer = nullptr;
	Settings* pSettingsB = nullptr;
	Settings oSettingsB;

public:
	SettingsUI(wxWindow* parent);
	void CreateSettingsPointerB(Settings& setobj);
	void ModalOkButton(wxCommandEvent& click);
	void ModalCancelButton(wxCommandEvent& click);

	wxDECLARE_EVENT_TABLE();
};

