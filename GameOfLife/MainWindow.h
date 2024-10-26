#pragma once
#include <wx/wx.h>
#include "wx/wx.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "DrawingPanel.h"
#include <vector>

// Forward-declared DrawingPanel class
class DrawingPanel;                     
struct Settings;
// MainWindow class
class MainWindow :  public wxFrame
{
private:
	DrawingPanel* _panel = nullptr;
	wxBoxSizer* _sizer = nullptr;
	wxStatusBar* _statusBar = nullptr;
	wxToolBar* _toolBar = nullptr;
	wxTimer* _timer =  nullptr;

	// View Menu items
	wxMenuItem* _neighborCount = nullptr;
	wxMenuItem* _finite = nullptr;
	wxMenuItem* _toroidal = nullptr;
	wxMenuItem* _showGrid = nullptr;
	wxMenuItem* _show10x10 = nullptr;
	wxMenuItem* _showHUD = nullptr;

	// Randomize Menu items
	wxMenuItem* _randomTime = nullptr;
	wxMenuItem* _randomSeed = nullptr;

	std::vector<std::vector<bool>> _gameBoard;
	std::vector<std::vector<int>> _neighborGrid;
	
	int _numOfGenerations = 0;
	int _livingCells = 0;

	Settings _settingsObj;

	std::string SaveName = "";
	
public:
	MainWindow();

	void StatusBarUpdate(int generation, int liveCells);

	void OnResize(wxSizeEvent& event);
	void OnPlay(wxCommandEvent& event);
	void OnPause(wxCommandEvent& event);
	void OnNext(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);
	
	void OnSettings(wxCommandEvent& event);
	void OnResetSettings(wxCommandEvent& event);

	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnImport(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	
	void OnNeighborCount(wxCommandEvent& event);
	void OnFiniteClick(wxCommandEvent& event);
	void OnToroidalClick(wxCommandEvent& event);
	void OnShowGridLines(wxCommandEvent& event);
	void OnShow10x10(wxCommandEvent& event);
	void OnShowHUD(wxCommandEvent& event);

	void OnRandomTime(wxCommandEvent& event);
	void OnRandomSeed(wxCommandEvent& event);
	void RandomizeGrid(int rType);

	void RefreshMenuItems();

	void TimerOn(wxTimerEvent& timer);

	int CountLiveNeighbors(std::vector<std::vector<bool>>& grid, int x, int y);

	void InitializeGrid();
	void NextGeneration();

	~MainWindow();
	
	wxDECLARE_EVENT_TABLE();
};

