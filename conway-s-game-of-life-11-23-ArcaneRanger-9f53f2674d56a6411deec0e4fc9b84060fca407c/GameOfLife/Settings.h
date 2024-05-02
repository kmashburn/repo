#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings {
	int iGridSize = 15;
	int iTickLength = 100;
	bool bIsWrap = 1;
	bool bNeighView = 0;
	bool bInfoText = 1;
	bool bGridView = 1;
	bool bTensGrid = 0;
	int iGenerations;
	int iLivingCells;

	unsigned int iLiveRed = 0;
	unsigned int iLiveGreen = 255;
	unsigned int iLiveBlue = 0;
	unsigned int iLiveAlpha = 255;

	unsigned int iDeadRed = 255;
	unsigned int iDeadGreen = 255;
	unsigned int iDeadBlue = 255;
	unsigned int iDeadAlpha = 255;

	unsigned int iLineRed = 0;
	unsigned int iLineGreen = 0;
	unsigned int iLineBlue = 0;
	unsigned int iLineAlpha = 255;

	wxColor GetLiveColor() {
		wxColor hue(iLiveRed, iLiveGreen, iLiveBlue, iLiveAlpha);
		return hue;
	}

	wxColor GetDeadColor() {
		wxColor hue(iDeadRed, iDeadGreen, iDeadBlue, iDeadAlpha);
		return hue;
	}

	wxColor GetLineColor() {
		wxColor hue(iLineRed, iLineGreen, iLineBlue, iLineAlpha);
		return hue;
	}

	void SetLiveColor(wxColor hue) {
		iLiveRed = hue.GetRed();
		iLiveGreen = hue.GetGreen();
		iLiveBlue = hue.GetBlue();
		iLiveAlpha = hue.Alpha();
	}

	void SetDeadColor(wxColor hue) {
		iDeadRed = hue.GetRed();
		iDeadGreen = hue.GetGreen();
		iDeadBlue = hue.GetBlue();
		iDeadAlpha = hue.Alpha();
	}

	void SetLineColor(wxColor hue) {
		iLineRed = hue.GetRed();
		iLineGreen = hue.GetGreen();
		iLineBlue = hue.GetBlue();
		iLineAlpha = hue.Alpha();
	}

	void LoadSettings() {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}

	void SaveSettings() {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}
};
