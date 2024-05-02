#pragma once
#include "wx/wx.h"
#include "Settings.h"
class DrawingPanel :
    public wxPanel
{
public:
    std::vector<std::vector<bool>>& rBeforeGameBoard;
    std::vector<std::vector<int>>& rNeighborBox;
    Settings* pSettingsA = nullptr;
    wxFrame* pFrame;
    wxPen* pPen;

    DrawingPanel() = default;

    ~DrawingPanel() {};

    DrawingPanel(wxFrame* parent, wxSize size, 
        std::vector<std::vector<bool>>& board, 
        std::vector<std::vector<int>>& box);

    void OnPaint(wxPaintEvent& event);
    void CreateSettingsPointerA(Settings& setobj);
    void FlipBeforeBoardIndex(int xpos, int ypos);
    bool GetBeforeBoardIndex(int xpos, int ypos);
    void OnMouseClick(wxMouseEvent& click);

    wxDECLARE_EVENT_TABLE();
};
