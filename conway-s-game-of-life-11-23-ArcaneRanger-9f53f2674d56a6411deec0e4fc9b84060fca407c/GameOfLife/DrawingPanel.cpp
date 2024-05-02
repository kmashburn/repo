#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_LEFT_UP(DrawingPanel::OnMouseClick)
	EVT_PAINT(DrawingPanel::OnPaint)
wxEND_EVENT_TABLE();

DrawingPanel::DrawingPanel(wxFrame* parent, wxSize size, 
	std::vector<std::vector<bool>>& board,
	std::vector<std::vector<int>>& box) : wxPanel(parent,
		wxID_ANY, wxPoint(0, 0), size), rBeforeGameBoard(board), 
		rNeighborBox(box) {
		this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	pFrame = parent;
}

void DrawingPanel::OnPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext* Image = wxGraphicsContext::Create(dc);

	wxSize PanelCalc = GetSize();
	float XPosition = PanelCalc.GetX() / (float)pSettingsA->iGridSize;
	float YPosition = PanelCalc.GetY() / (float)pSettingsA->iGridSize;
	
	if (!Image) { return; }
	if (pSettingsA->bGridView) {
		Image->SetPen(pSettingsA->GetLineColor());
	}
	Image->SetFont(wxFontInfo(16), *wxBLUE);

	for (int Y = 0; Y < pSettingsA->iGridSize; Y++) {
		for (int X = 0; X < pSettingsA->iGridSize; X++) {
			bool Check = rBeforeGameBoard[X][Y];
			if (Check) {
				Image->SetBrush(pSettingsA->GetLiveColor());
			}
			else {
				Image->SetBrush(pSettingsA->GetDeadColor());
			}
			Image->DrawRectangle((X * XPosition), (Y * YPosition),
				XPosition, YPosition);
			if(pSettingsA->bNeighView && rNeighborBox[X][Y] != 0)
			Image->DrawText(std::to_string(rNeighborBox[X][Y]),
				X * XPosition + (XPosition / 3),
				Y * YPosition + (YPosition / 3));
		}
	}
	pPen = new wxPen(pSettingsA->GetLineColor(), 6);
	dc.SetPen(*pPen);

	if (pSettingsA->bTensGrid) {
		for (int Z = 0; Z < (pSettingsA->iGridSize / 10) + 1; Z++) {
			wxPoint VerStart(Z * 10 * XPosition, 0);
			wxPoint VerEnd(Z * 10 * XPosition, this->GetSize().y);
			dc.DrawLine(VerStart, VerEnd);

			wxPoint HorStart(0, Z * 10 * YPosition);
			wxPoint HorEnd(this->GetSize().x, Z * 10 * YPosition);
			dc.DrawLine(HorStart, HorEnd);
		}
	}
	
	if (pSettingsA->bInfoText) {
		Image->SetFont(wxFontInfo(16), *wxRED);
		if (pSettingsA->bIsWrap)
			Image->DrawText("Universe: Toroidal", 1, 1);
		else
			Image->DrawText("Universe: Finite", 1, 1);
		Image->DrawText("Grid Size:" + std::to_string(pSettingsA->iGridSize) +
			"x" + std::to_string(pSettingsA->iGridSize), 1, 18);
		Image->DrawText("Live Count: " + std::to_string(pSettingsA->iLivingCells),
			1, 35);
		Image->DrawText("Generations: " + std::to_string(pSettingsA->iGenerations),
			1, 52);
	}
}

void DrawingPanel::CreateSettingsPointerA(Settings& setobj) {
	pSettingsA = &setobj;
}

void DrawingPanel::FlipBeforeBoardIndex(int xpos, int ypos) {
	if (rBeforeGameBoard[xpos][ypos])
		rBeforeGameBoard[xpos][ypos] = 0;
	else
		rBeforeGameBoard[xpos][ypos] = 1;
}

void DrawingPanel::OnMouseClick(wxMouseEvent& click) {
	wxSize panelCalc = GetSize();
	float xPosition = panelCalc.GetX() / (float)pSettingsA->iGridSize;
	float yPosition = panelCalc.GetY() / (float)pSettingsA->iGridSize;

	int clickPosX = click.GetX() / xPosition;
	int clickPosY = click.GetY() / yPosition;
	
	FlipBeforeBoardIndex(clickPosX, clickPosY);
	pFrame->Refresh();
}