#include "SettingsUI.h"

wxBEGIN_EVENT_TABLE(SettingsUI, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsUI::ModalOkButton)
	EVT_BUTTON(wxID_CANCEL, SettingsUI::ModalCancelButton)
wxEND_EVENT_TABLE()

SettingsUI::SettingsUI(wxWindow* parent) 
	: wxDialog(parent, wxID_ANY, "Values", wxPoint(400, 400),
		wxSize(250, 300)) {
	pSpinnerA = new wxSpinCtrl(this, wxID_ANY);
	pSpinnerB = new wxSpinCtrl(this, wxID_ANY);
	pColorPickA = new wxColourPickerCtrl(this, wxID_ANY);
	pColorPickB = new wxColourPickerCtrl(this, wxID_ANY);
	pColorPickC = new wxColourPickerCtrl(this, wxID_ANY);

	pTextLabelA = new wxStaticText(this, wxID_ANY, "Current Grid Size");
	pTextLabelB = new wxStaticText(this, wxID_ANY, "Living Cell Color");
	pTextLabelC = new wxStaticText(this, wxID_ANY, "Interval Length");
	pTextLabelD = new wxStaticText(this, wxID_ANY, "Dead Cell Color");
	pTextLabelE = new wxStaticText(this, wxID_ANY, "Grid Line Color");

	pBoxSizerB = new wxBoxSizer(wxVERTICAL);
	pBoxSizerC = new wxBoxSizer(wxHORIZONTAL);
	pBoxSizerD = new wxBoxSizer(wxHORIZONTAL);
	pBoxSizerE = new wxBoxSizer(wxHORIZONTAL);
	pBoxSizerF = new wxBoxSizer(wxHORIZONTAL);
	pBoxSizerG = new wxBoxSizer(wxHORIZONTAL);
	pButtonSizer = CreateButtonSizer(wxOK | wxCANCEL);

	SetSizer(pBoxSizerB);
	pTextLabelA->SetMinSize(wxSize(190, 25));
	pTextLabelB->SetMinSize(wxSize(150, 25));
	pTextLabelC->SetMinSize(wxSize(190, 25));
	pTextLabelD->SetMinSize(wxSize(150, 25));
	pTextLabelE->SetMinSize(wxSize(150, 25));

	pSpinnerA->SetValue(oSettingsB.iGridSize);
	pSpinnerB->SetValue(oSettingsB.iTickLength);
	pColorPickA->SetColour(oSettingsB.GetLiveColor());
	pColorPickB->SetColour(oSettingsB.GetDeadColor());
	pColorPickC->SetColour(oSettingsB.GetLineColor());

	pBoxSizerC->Add(pTextLabelA);
	pBoxSizerC->Add(pSpinnerA);

	pBoxSizerE->Add(pTextLabelC);
	pBoxSizerE->Add(pSpinnerB);

	pBoxSizerD->Add(pTextLabelB);
	pBoxSizerD->Add(pColorPickA);

	pBoxSizerF->Add(pTextLabelD);
	pBoxSizerF->Add(pColorPickB);

	pBoxSizerG->Add(pTextLabelE);
	pBoxSizerG->Add(pColorPickC);

	pBoxSizerB->Add(pBoxSizerC);
	pBoxSizerB->Add(pBoxSizerE);
	pBoxSizerB->Add(pBoxSizerD);
	pBoxSizerB->Add(pBoxSizerF);
	pBoxSizerB->Add(pBoxSizerG);
	pBoxSizerB->Add(pButtonSizer);
}

void SettingsUI::CreateSettingsPointerB(Settings& setobj)
{
	pSettingsB = &setobj;
}

void SettingsUI::ModalOkButton(wxCommandEvent& click)
{
	pSettingsB->iGridSize = pSpinnerA->GetValue();
	pSettingsB->iTickLength = pSpinnerB->GetValue();

	pSettingsB->SetLiveColor(pColorPickA->GetColour());
	pSettingsB->SetDeadColor(pColorPickB->GetColour());
	pSettingsB->SetLineColor(pColorPickC->GetColour());
	this->EndModal(wxID_OK);
}

void SettingsUI::ModalCancelButton(wxCommandEvent& click)
{
	this->EndModal(wxID_CANCEL);
}
