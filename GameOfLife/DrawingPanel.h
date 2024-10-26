#pragma once
#include <wx/wx.h>
#include "wx/wx.h"
#include "Settings.h"
#include <vector>

class MainWindow;
struct Settings;

class DrawingPanel :  public wxPanel
{
private:
	std::vector<std::vector<bool>>& pGameBoard;
	std::vector<std::vector<int>>& pNeighborGrid;
	Settings* pSettingsObj = nullptr;
	
public:
	DrawingPanel(wxFrame* parent, wxSize size, std::vector<std::vector<bool>>& gameBoard, Settings* settings, std::vector<std::vector<int>>& neighborGrid);

	void OnPaint(wxPaintEvent& event);
	void OnMouseUp(wxMouseEvent& click);

	void SetNewPanelSize(wxSize& size);
	void SetGridSize(int newGridSize);

	int CountLivingNeighbors(std::vector<std::vector<bool>>& grid, int x, int y);

	~DrawingPanel();

	wxDECLARE_EVENT_TABLE();
};

