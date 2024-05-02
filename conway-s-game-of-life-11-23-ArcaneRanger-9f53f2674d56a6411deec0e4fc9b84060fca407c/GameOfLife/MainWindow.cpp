#include "MainWindow.h"
#include "Icons/next.xpm"
#include "Icons/play.xpm"
#include "Icons/pause.xpm"
#include "Icons/trash.xpm"
#include "SettingsUI.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnSizeChange)
	EVT_BUTTON(10001, MainWindow::JokeButtonA)
	EVT_BUTTON(10002, MainWindow::JokeButtonB)
	EVT_MENU(20001, MainWindow::PlayButton)
	EVT_MENU(20002, MainWindow::PauseButton)
	EVT_MENU(20003, MainWindow::NextButton)
	EVT_MENU(20004, MainWindow::ResetButton)
	EVT_MENU(20005, MainWindow::ShowSettingsModal)
	EVT_MENU(20006, MainWindow::RestoreDefault)
	EVT_MENU(20007, MainWindow::RandomScrambleGen)
	EVT_MENU(20008, MainWindow::RandomSeedGen)
	EVT_MENU(20009, MainWindow::FlipUniverseA)
	EVT_MENU(20010, MainWindow::FlipUniverseB)
	EVT_MENU(20011, MainWindow::ViewNeighCount)
	EVT_MENU(20012, MainWindow::ViewTextDisplay)
	EVT_MENU(20013, MainWindow::ViewNormalGrid)
	EVT_MENU(20014, MainWindow::ViewTensGrid)
	EVT_MENU(20015, MainWindow::OnNewClick)
	EVT_MENU(20016, MainWindow::OnOpenClick)
	EVT_MENU(20017, MainWindow::OnSaveClick)
	EVT_MENU(20018, MainWindow::OnSaveAsClick)
	EVT_MENU(20019, MainWindow::CloseWindow)
	EVT_MENU(20020, MainWindow::ImportSameSizeGrid)
	EVT_TIMER(30000, MainWindow::TimedEvent)
wxEND_EVENT_TABLE();

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Game of Life", 
	wxPoint(200, 200), wxSize(600, 600)) {
	oSettings.LoadSettings();

	pStatBar = CreateStatusBar();
	pMenuBar = new wxMenuBar();
	SetMenuBar(pMenuBar);

	pMenuD = new wxMenu();
	pMenuD->Append(20015, "New");
	pMenuD->Append(20016, "Open");
	pMenuD->Append(20017, "Save");
	pMenuD->Append(20018, "Save As");
	pMenuD->Append(20020, "Import");
	pMenuD->Append(20019, "Quit");
	pMenuBar->Append(pMenuD, "File");

	pMenuC = new wxMenu();
	pMenuItemA = new wxMenuItem(pMenuC, 20009, "Finite", "", wxITEM_CHECK);
	pMenuItemB = new wxMenuItem(pMenuC, 20010, "Toroidal", "", wxITEM_CHECK);
	pMenuItemC = new wxMenuItem(pMenuC, 20011, "Neighbors", "", wxITEM_CHECK);
	pMenuItemD = new wxMenuItem(pMenuC, 20012, "Show Info", "", wxITEM_CHECK);
	pMenuItemE = new wxMenuItem(pMenuC, 20013, "Show Grid", "", wxITEM_CHECK);
	pMenuItemF = new wxMenuItem(pMenuC, 20014, "Show 10x10", "", wxITEM_CHECK);
	pMenuC->Append(pMenuItemA);
	pMenuC->Append(pMenuItemB);
	pMenuC->Append(pMenuItemC);
	pMenuC->Append(pMenuItemD);
	pMenuC->Append(pMenuItemE);
	pMenuC->Append(pMenuItemF);
	pMenuBar->Append(pMenuC, "View");

	pMenuB = new wxMenu();
	pMenuB->Append(20007, "Scramble");
	pMenuB->Append(20008, "Seed");
	pMenuBar->Append(pMenuB, "Random");

	pMenuA = new wxMenu();
	pMenuA->Append(20005, "Settings");
	pMenuA->Append(20006, "Default");
	pMenuBar->Append(pMenuA, "Options");

	pMenuItemA->Check(!oSettings.bIsWrap);
	pMenuItemB->Check(oSettings.bIsWrap);
	pMenuItemC->Check(oSettings.bNeighView);
	pMenuItemD->Check(oSettings.bInfoText);
	pMenuItemE->Check(oSettings.bGridView);
	pMenuItemF->Check(oSettings.bTensGrid);

	wxBitmap PlayIcon(play_xpm);
	wxBitmap PauseIcon(pause_xpm);
	wxBitmap NextIcon(next_xpm);
	wxBitmap TrashIcon(trash_xpm);

	pToolBar = CreateToolBar();
	pToolBar->AddTool(20001, "Start", PlayIcon);
	pToolBar->AddTool(20002, "Stop", PauseIcon);
	pToolBar->AddTool(20003, "Next", NextIcon);
	pToolBar->AddTool(20004, "Reset", TrashIcon);
	pToolBar->Realize();

	pTimer = new wxTimer(this, 30000);

	pButtonA = new wxButton(this, 10001, "Get clicked nerd", wxPoint(0, 0),
		wxSize(100, 25));
	pButtonB = new wxButton(this, 10002, "Clickity clickums", wxPoint(100, 0),
		wxSize(100, 25));

	pDrawPanel = new DrawingPanel(this, wxSize(0, 0), vBeforeGameBoard, 
		vNeighborBox);
	pDrawPanel->CreateSettingsPointerA(oSettings);

	pBoxSizerA = new wxBoxSizer(wxVERTICAL);
	pBoxSizerA->Add(pDrawPanel, 1, wxEXPAND | wxALL);

	SetSizer(pBoxSizerA);
	this->Layout();

	GridInitializer();
}

void MainWindow::CloseWindow(wxCommandEvent& click) {
	Close();
}

void MainWindow::FlipUniverseA(wxCommandEvent& click) {
	if (oSettings.bIsWrap) {
		oSettings.bIsWrap = 0;
		pMenuC->Check(20010, false);
	}
	else {
		oSettings.bIsWrap = 1;
		pMenuC->Check(20010, true);
	}
	oSettings.SaveSettings();
	Refresh();
}

void MainWindow::FlipUniverseB(wxCommandEvent& click) {
	if (!oSettings.bIsWrap) {
		oSettings.bIsWrap = 1;
		pMenuC->Check(20009, false);
	}
	else {
		oSettings.bIsWrap = 0;
		pMenuC->Check(20009, true);
	}
	oSettings.SaveSettings();
	Refresh();
}

void MainWindow::GridInitializer() {
	vBeforeGameBoard.clear();
	vAfterGameBoard.clear();
	vNeighborBox.clear();
	std::vector<bool> GarbageLineA;
	std::vector<int> GarbageLineB;
	for (int X = 0; X < oSettings.iGridSize; X++) {
		GarbageLineA.push_back(0);
		GarbageLineB.push_back(0);
	}
	for (int Y = 0; Y < oSettings.iGridSize; Y++) {
		vBeforeGameBoard.push_back(GarbageLineA);
		vAfterGameBoard.push_back(GarbageLineA);
		vNeighborBox.push_back(GarbageLineB);
	}
}

void MainWindow::ImportSameSizeGrid(wxCommandEvent& click) {
	wxFileDialog FileDia(this, "Save GoL Universe", "", "",
		"GoL Files (*.cells) | *.cells", wxFD_OPEN);
	if (FileDia.ShowModal() == wxID_CANCEL) {
		return;
	}
	vBeforeGameBoard.clear();
	vAfterGameBoard.clear();
	vNeighborBox.clear();

	std::string Buffer;
	std::ifstream Stream;
	std::vector<bool> Storage;
	std::vector<int> Garbage;
	Stream.open((std::string)FileDia.GetPath());
	if (Stream.is_open()) {
		while (!Stream.eof()) {
			Storage.clear();
			Garbage.clear();
			std::getline(Stream, Buffer);
			for (int Z = 0; Z < Buffer.size(); Z++) {
				if (Buffer[Z] == '*')
					Storage.push_back(1);
				else
					Storage.push_back(0);
				Garbage.push_back(0);
			}
			for (int W = 0; W < oSettings.iGridSize; W++) {
				vBeforeGameBoard.push_back(Storage);
				vAfterGameBoard.push_back(Storage);
				vNeighborBox.push_back(Garbage);
			}
		}
		Refresh();
		Stream.close();
	}
}

void MainWindow::JokeButtonA(wxCommandEvent& click) {
	pButtonA->SetLabelText("Boop");
}

void MainWindow::JokeButtonB(wxCommandEvent& click) {
	pButtonB->SetLabelText(";3");
}

void MainWindow::LivingCellCount() {
	int LivingCount = 0;
	for (int X = 0; X < oSettings.iGridSize; X++) {
		for (int Y = 0; Y < oSettings.iGridSize; Y++) {
			if (vBeforeGameBoard[X][Y]) {
				LivingCount++;
			}
		}
	}
	oSettings.iLivingCells = LivingCount;
}


int MainWindow::NeighborCount(int xpos, int ypos) {
	int NeighborCount = 0;
	int FindX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int	FindY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

	switch (oSettings.bIsWrap) {
	case 0:
		for (int Z = 0; Z < 8; Z++) {
			int CheckX = xpos + FindX[Z];
			int CheckY = ypos + FindY[Z];
			if (CheckX >= 0 && CheckX < oSettings.iGridSize &&
				CheckY >= 0 && CheckY < oSettings.iGridSize) {
				if (vBeforeGameBoard[CheckX][CheckY] == 1) {
					NeighborCount++;
				}
			}
		}
		break;
	default:
		for (int Z = 0; Z < 8; Z++) {
			int WrapX = (xpos + FindX[Z] + oSettings.iGridSize) % oSettings.iGridSize;
			int WrapY = (ypos + FindY[Z] + oSettings.iGridSize) % oSettings.iGridSize;
			if (vBeforeGameBoard[WrapX][WrapY] == 1) {
				NeighborCount++;
			}
		}
	}
	return NeighborCount;
}


void MainWindow::NextButton(wxCommandEvent& click) {
	NextGeneration();
}

void MainWindow::NextGeneration() {
	for (int X = 0; X < oSettings.iGridSize; X++) {
		for (int Y = 0; Y < oSettings.iGridSize; Y++) {
			NextState(X, Y, NeighborCount(X, Y));
		}
	}
	vBeforeGameBoard.swap(vAfterGameBoard);
	oSettings.iGenerations++;
	LivingCellCount();
	Refresh();
}

void MainWindow::NextState(int xpos, int ypos, int cells) {
	if (cells == 3 || (cells == 2 && vBeforeGameBoard[xpos][ypos] == 1)) {
		vAfterGameBoard[xpos][ypos] = 1;
	}
	else {
		vAfterGameBoard[xpos][ypos] = 0;
	}
}

void MainWindow::OnNewClick(wxCommandEvent& click) {
	GridInitializer();
	Refresh();
}

void MainWindow::OnSaveAsClick(wxCommandEvent& click) {
	wxFileDialog FileDia(this, "Save GoL Universe", "", "",
		"GoL Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (FileDia.ShowModal() == wxID_CANCEL) {
		return;
	}
	std::ofstream Stream;
	sPath = (std::string)FileDia.GetPath();
	Stream.open(sPath);
	if (Stream.is_open()) {
		for (int X = 0; X < oSettings.iGridSize; X++) {
			for (int Y = 0; Y < oSettings.iGridSize; Y++) {
				if (vBeforeGameBoard[X][Y])
					Stream << "*";
				else
					Stream << ".";
			}
			Stream << "\n";
		}
		Stream.close();
	}
}

void MainWindow::OnSaveClick(wxCommandEvent& click) {
	std::ofstream Stream;
	if (!sPath.empty())
		Stream.open(sPath);
	else {
		wxFileDialog FileDia(this, "Save GoL Universe", "", "",
			"GoL Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (FileDia.ShowModal() == wxID_CANCEL) {
			return;
		}
		sPath = (std::string)FileDia.GetPath();
		Stream.open(sPath);
	}
	if (Stream.is_open()) {
		for (int X = 0; X < oSettings.iGridSize; X++) {
			for (int Y = 0; Y < oSettings.iGridSize; Y++) {
				if (vBeforeGameBoard[X][Y])
					Stream << "*";
				else
					Stream << ".";
			}
			Stream << "\n";
		}
		Stream.close();
	}
}

void MainWindow::OnOpenClick(wxCommandEvent& click) {
	wxFileDialog FileDia(this, "Save GoL Universe", "", "",
		"GoL Files (*.cells) | *.cells", wxFD_OPEN);
	if (FileDia.ShowModal() == wxID_CANCEL) {
		return;
	}
	vBeforeGameBoard.clear();
	vAfterGameBoard.clear();
	vNeighborBox.clear();

	std::string Buffer;
	std::ifstream Stream;
	std::vector<bool> Storage;
	std::vector<int> Garbage;
	Stream.open((std::string)FileDia.GetPath());
	if (Stream.is_open()) {
		while (!Stream.eof()) {
			Storage.clear();
			Garbage.clear();
			std::getline(Stream, Buffer);
			for (int Z = 0; Z < Buffer.size(); Z++) {
				if (Buffer[Z] == '*')
					Storage.push_back(1);
				else
					Storage.push_back(0);
				Garbage.push_back(0);
				if (Z == Buffer.size() - 1) {
					vBeforeGameBoard.push_back(Storage);
					vAfterGameBoard.push_back(Storage);
					vNeighborBox.push_back(Garbage);
				}
			}
		}
		oSettings.iGridSize = vBeforeGameBoard.size();
		Refresh();
		Stream.close();
	}
}

void MainWindow::OnSizeChange(wxSizeEvent& change)
{
	if (pDrawPanel) {
		pDrawPanel->SetSize(change.GetSize());
		change.Skip();
		Refresh();
	}
}

void MainWindow::PauseButton(wxCommandEvent& click) {
	pTimer->Stop();
}

void MainWindow::PlayButton(wxCommandEvent& click) {
	pTimer->Start(oSettings.iTickLength);
}

void MainWindow::RandomizeGrid(int seed) {
	GridInitializer();
	srand(seed);
	for (int X = 0; X < oSettings.iGridSize; X++) {
		for (int Y = 0; Y < oSettings.iGridSize; Y++) {
			vAfterGameBoard[X][Y] = static_cast<bool>(rand() % 2);
		}
	}
	vBeforeGameBoard.swap(vAfterGameBoard);
	Refresh();
}

void MainWindow::RandomScrambleGen(wxCommandEvent& click) {
	RandomizeGrid(time(NULL));
}

void MainWindow::RandomSeedGen(wxCommandEvent& click) {
	RandomizeGrid(wxGetNumberFromUser("Enter Seed","","Seed Prompt"));
}

void MainWindow::Refresh(bool eraseBackground, const wxRect* rect) {
	UpdateNeighborBox();
	LivingCellCount();
	wxFrame::Refresh();
	UpdateStatusBar();
}

void MainWindow::ResetButton(wxCommandEvent& click) {
	oSettings.iGenerations = 0;
	GridInitializer();
	Refresh();
}

void MainWindow::RestoreDefault(wxCommandEvent& click) {
	oSettings.SetLineColor(wxColor(*wxBLACK));
	oSettings.SetDeadColor(wxColor(*wxWHITE));
	oSettings.SetLiveColor(wxColor(*wxGREEN));
	oSettings.iGridSize = 15;
	oSettings.iTickLength = 100;
	oSettings.SaveSettings();
	GridInitializer();
	Refresh();
}

void MainWindow::ShowSettingsModal(wxCommandEvent& click) {
	SettingsUI* window = new SettingsUI(this);
	window->CreateSettingsPointerB(oSettings);
	window->ShowModal();
	oSettings.SaveSettings();
	GridInitializer();
	Refresh();
	delete window;
}

void MainWindow::TimedEvent(wxTimerEvent& tick) {
	NextGeneration();
}

void MainWindow::UpdateNeighborBox() {
	for (int X = 0; X < oSettings.iGridSize; X++) {
		for (int Y = 0; Y < oSettings.iGridSize; Y++) {
			vNeighborBox[X][Y] = NeighborCount(X, Y);
		}
	}
}

void MainWindow::UpdateStatusBar() {
	pStatBar->SetStatusText("Generations: " + std::to_string(oSettings.iGenerations) +
		" Living Cells: " + std::to_string(oSettings.iLivingCells));
}

void MainWindow::ViewNormalGrid(wxCommandEvent& click) {
	if (oSettings.bGridView)
		oSettings.bGridView = 0;
	else
		oSettings.bGridView = 1;
	oSettings.SaveSettings();
	Refresh();
}

void MainWindow::ViewNeighCount(wxCommandEvent& click) {
	if (oSettings.bNeighView)
		oSettings.bNeighView = 0;
	else
		oSettings.bNeighView = 1;
	oSettings.SaveSettings();
	Refresh();
}

void MainWindow::ViewTensGrid(wxCommandEvent& click) {
	if (oSettings.bTensGrid)
		oSettings.bTensGrid = 0;
	else
		oSettings.bTensGrid = 1;
	oSettings.SaveSettings();
	Refresh();
}

void MainWindow::ViewTextDisplay(wxCommandEvent& click) {
	if (oSettings.bInfoText) {
		oSettings.bInfoText = 0;
	}
	else {
		oSettings.bInfoText = 1;
	}
	oSettings.SaveSettings();
	Refresh();
}