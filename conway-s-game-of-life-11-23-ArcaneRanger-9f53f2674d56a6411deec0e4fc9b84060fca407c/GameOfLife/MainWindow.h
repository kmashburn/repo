#pragma once
#include "wx/wx.h"
#include <vector>
#include "DrawingPanel.h"
#include "SettingsUI.h"

class MainWindow : public wxFrame {
private:	

	std::vector<std::vector<bool>> vBeforeGameBoard;
	std::vector<std::vector<bool>> vAfterGameBoard;
	std::vector<std::vector<int>> vNeighborBox;
	DrawingPanel* pDrawPanel = nullptr;
	wxMenuItem* pMenuItemA = nullptr;
	wxMenuItem* pMenuItemB = nullptr;
	wxMenuItem* pMenuItemC = nullptr;
	wxMenuItem* pMenuItemD = nullptr;
	wxMenuItem* pMenuItemE = nullptr;
	wxMenuItem* pMenuItemF = nullptr;
	wxBoxSizer* pBoxSizerA = nullptr;
	wxStatusBar* pStatBar = nullptr;
	wxToolBar* pToolBar = nullptr;
	wxMenuBar* pMenuBar = nullptr;
	wxButton* pButtonA = nullptr;
	wxButton* pButtonB = nullptr;
	wxTimer* pTimer = nullptr;
	wxMenu* pMenuA = nullptr;
	wxMenu* pMenuB = nullptr;
	wxMenu* pMenuC = nullptr;
	wxMenu* pMenuD = nullptr;
	Settings oSettings;
	std::string sPath;

public:
	MainWindow();

	~MainWindow() { delete pTimer; };

	void Refresh(bool eraseBackground = true, const wxRect* rect = NULL);
	void ImportSameSizeGrid(wxCommandEvent& click);
	void NextState(int xpos, int ypos, int cells);
	void ShowSettingsModal(wxCommandEvent& click);
	void RandomScrambleGen(wxCommandEvent& click);
	void ViewTextDisplay(wxCommandEvent& click);
	void RestoreDefault(wxCommandEvent& click);
	void ViewNeighCount(wxCommandEvent& click);
	void ViewNormalGrid(wxCommandEvent& click);
	void FlipUniverseA(wxCommandEvent& click);
	void FlipUniverseB(wxCommandEvent& click);
	void OnSaveAsClick(wxCommandEvent& click);
	void RandomSeedGen(wxCommandEvent& click);
	void ViewTensGrid(wxCommandEvent& click);
	void CloseWindow(wxCommandEvent& click);
	void JokeButtonA(wxCommandEvent& click);
	void JokeButtonB(wxCommandEvent& click);
	void OnOpenClick(wxCommandEvent& click);
	void OnSaveClick(wxCommandEvent& click);
	void PauseButton(wxCommandEvent& click);
	void ResetButton(wxCommandEvent& click);
	void NextButton(wxCommandEvent& click);
	void OnNewClick(wxCommandEvent& click);
	void OnSizeChange(wxSizeEvent& change);
	void PlayButton(wxCommandEvent& click);
	int NeighborCount(int xpos, int ypos);
	void TimedEvent(wxTimerEvent& tick);
	void RandomizeGrid(int seed);
	void UpdateNeighborBox();
	void GridInitializer();
	void LivingCellCount();
	void UpdateStatusBar();
	void NextGeneration();

	wxDECLARE_EVENT_TABLE();
};