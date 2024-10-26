#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "PLAY.xpm"
#include "PAUSE.xpm"
#include "NEXT.xpm"
#include "CLEAR.xpm"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include <fstream>
#include <algorithm>
#include <wx/wx.h>

// Event table - define handlers for various events (Menu buttons, Timer, resize, etc.)
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_SIZE(MainWindow::OnResize)
	// Toolbar events
	EVT_MENU(10001, MainWindow::OnPlay)
	EVT_MENU(10002, MainWindow::OnPause)
	EVT_MENU(10003, MainWindow::OnNext)
	EVT_MENU(10004, MainWindow::OnClear)
	// SettingsDialog menu
	EVT_MENU(10005, MainWindow::OnSettings)
	EVT_MENU(10012, MainWindow::OnResetSettings)
	// File menu items
	EVT_MENU(wxID_NEW, MainWindow::OnNew)
	EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
	EVT_MENU(wxID_SAVE, MainWindow::OnSave)
	EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAs)
	EVT_MENU(10011, MainWindow::OnImport)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	// View menu items (clickable)
	EVT_MENU(10006, MainWindow::OnNeighborCount)
	EVT_MENU(10007, MainWindow::OnFiniteClick)
	EVT_MENU(10008, MainWindow::OnToroidalClick)
	EVT_MENU(10013, MainWindow::OnShowGridLines)
	EVT_MENU(10014, MainWindow::OnShow10x10)
	EVT_MENU(10015, MainWindow::OnShowHUD)
	// Randomize Menu Items
	EVT_MENU(10009, MainWindow::OnRandomTime)
	EVT_MENU(10010, MainWindow::OnRandomSeed)
	// Timer
	EVT_TIMER(10050, MainWindow::TimerOn)
wxEND_EVENT_TABLE()	

// MainWindow constructor
MainWindow::MainWindow() 
	: wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 100), wxSize(800, 800)) {
	// Load settings object
	_settingsObj.Settings::LoadSettings();
	
	/*_numOfGenerations = 0;
	_livingCells = 0;*/
	Refresh();
	// Create/instantiate a new wxMenuBar for main menu bar, set it to be the main menu bar
	wxMenuBar* mainMenuBar = new wxMenuBar();
	SetMenuBar(mainMenuBar);
	
	// Create/instantiate new menus
	wxMenu* fileMenu = new wxMenu();
	wxMenu* viewMenu = new wxMenu();
	wxMenu* randomizeMenu = new wxMenu();
	wxMenu* optionsMenu = new wxMenu();
	
	// Options under File menu
	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);
	fileMenu->Append(10011, "Import");
	fileMenu->Append(wxID_EXIT);

	// Options under the View menu (checkable)
	_neighborCount = new wxMenuItem(viewMenu, 10006, "Show Neighbor Count", "", wxITEM_CHECK);
	_neighborCount->SetCheckable(true);
	viewMenu->Append(_neighborCount);
	
	_finite = new wxMenuItem(viewMenu, 10007, "Finite", "", wxITEM_CHECK);
	_finite->SetCheckable(true);
	viewMenu->Append(_finite);
	
	_toroidal = new wxMenuItem(viewMenu, 10008, "Toroidal", "", wxITEM_CHECK);
	_toroidal->SetCheckable(true);
	viewMenu->Append(_toroidal);

	_showGrid = new wxMenuItem(viewMenu, 10013, "Show Grid Lines", "", wxITEM_CHECK);
	_showGrid->SetCheckable(true);
	viewMenu->Append(_showGrid);
	

	_show10x10 = new wxMenuItem(viewMenu, 10014, "Show 10x10 Grid Lines", "", wxITEM_CHECK);
	_show10x10->SetCheckable(true);
	viewMenu->Append(_show10x10);
	

	_showHUD = new wxMenuItem(viewMenu, 10015, "Show HUD", "", wxITEM_CHECK);
	_showHUD->SetCheckable(true);
	viewMenu->Append(_showHUD);
	
	// Options under Randomize menu 
	randomizeMenu->Append(10009, "Randomize (time)");
	randomizeMenu->Append(10010, "Randomize (seed)");
	
	// Options under Options menu
	optionsMenu->Append(10005, "Settings");
	optionsMenu->Append(10012, "Reset Settings");
	
	// Add sub menus to the main menu bar
	mainMenuBar->Append(fileMenu, "File");
	mainMenuBar->Append(viewMenu, "View");
	mainMenuBar->Append(randomizeMenu, "Randomize");
	mainMenuBar->Append(optionsMenu, "Options");

	// DrawingPanel instance - MainWindow parent, size, gameboard argument, Settings object
	_panel = new DrawingPanel(this, wxSize(800, 800), _gameBoard, &_settingsObj, _neighborGrid);
	// Call method to initialize grid based on panel values
	InitializeGrid();
	// Status bar with 4 sections
	_statusBar = CreateStatusBar(10);
	StatusBarUpdate(_numOfGenerations, _livingCells);
	
	// Timer for the window (when play button is selected)
	_timer = new wxTimer(this, 10050);
	// Tool Bar with 4 buttons - play, pause, next, trash.
	_toolBar = CreateToolBar();
	_toolBar->AddTool(10001, "Play", PLAYbutton, "Play");
	_toolBar->AddTool(10002, "Pause", PAUSEbutton, "Pause");
	_toolBar->AddTool(10003, "Next", NEXTbutton, "Next");
	_toolBar->AddTool(10004, "Clear", CLEARbutton, "Clear");
	_toolBar->SetToolBitmapSize(wxSize(32, 32));
	_toolBar->Realize();
	// Sizer containing the drawing panel
	_sizer = new wxBoxSizer(wxVERTICAL);
	_sizer->Add(_panel, 1, wxEXPAND | wxALL);
	// Set the layout of the window using _sizer
	SetSizer(_sizer);
	// Update layout
	this->Layout();
	// Refresh menu options
	RefreshMenuItems();
}

// Update the status bar with current generation and number of living cells at present
void MainWindow::StatusBarUpdate(int generation, int liveCells) {
	_statusBar->SetStatusText("Generation:", 0);
	_statusBar->SetStatusText(std::to_string(generation), 1);
	_statusBar->SetStatusText("Living Cells:", 2);
	_statusBar->SetStatusText(std::to_string(liveCells), 3);
}



// Event handler for the window resize event
void MainWindow::OnResize(wxSizeEvent& event) {
	// If panel not nullptr, sets the size of _panel to match the new window size
	if (_panel != nullptr) {
		_panel->SetSize(event.GetSize());
		_panel->Refresh();
	}
	// Allows event to propagate
	event.Skip();
}

// Event handler for when the "play" button is clicked
void MainWindow::OnPlay(wxCommandEvent& event) {
	_timer->Start(_settingsObj.timerInterval);
}

// Event handler for when the "pause" button is clicked
void MainWindow::OnPause(wxCommandEvent& event) {
	_timer->Stop();
}

// Event handler for when the "next" button is clicked
void MainWindow::OnNext(wxCommandEvent& event) {
	NextGeneration();
}

// Event handler for when the "clear" button is clicked
void MainWindow::OnClear(wxCommandEvent& event) {
	// Resets the gameboard/neighbor count grid
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			_gameBoard[i][j] = false;
			_neighborGrid[i][j] = 0;
		}
	}
	// Reset the generation count and living cells count, then refreshes the status bar, then updates the display
	_numOfGenerations = 0;
	_livingCells = 0;
	_settingsObj.SetGenerationCt(_numOfGenerations);
	_settingsObj.SetLiveCells(_livingCells);
	StatusBarUpdate(_numOfGenerations, _livingCells);
	
	Refresh();
}

// Event handlers for Options menu
void MainWindow::OnSettings(wxCommandEvent& event) {
	SettingsDialog* settings = new SettingsDialog(this, &_settingsObj);
	int result = settings->ShowModal();
	if (result == wxID_OK) {
		InitializeGrid();
		_panel->Refresh();
	}
}

void MainWindow::OnResetSettings(wxCommandEvent& event) {
	
	_settingsObj.SetGridSize(30);
	_settingsObj.SetTimerInterval(100);
	_settingsObj.SetLivingCellColour(*wxLIGHT_GREY);
	_settingsObj.SetDeadCellColour(*wxWHITE);
	_settingsObj.SetGridColour(*wxBLACK);
	
	InitializeGrid();
	RefreshMenuItems();
	
	_settingsObj.SaveSettings();
	Refresh();
}

// Event Handlers for File menu
void MainWindow::OnNew(wxCommandEvent& event) {
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			_gameBoard[i][j] = false;
			_neighborGrid[i][j] = 0;
		}
	}
	SaveName = "";
	_numOfGenerations = 0;
	_livingCells = 0;
	_settingsObj.SetGenerationCt(_numOfGenerations);
	_settingsObj.SetLiveCells(_livingCells);
	StatusBarUpdate(_numOfGenerations, _livingCells);
	
	Refresh();
}

void MainWindow::OnOpen(wxCommandEvent& event) {
	wxFileDialog saveFile(this, "Open Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (saveFile.ShowModal() == wxID_CANCEL) {
		return;
	}
	for (int i = 0; 1 < _gameBoard.size(); i++) {
		_gameBoard[i].clear();
		_neighborGrid[i].clear();
		_gameBoard.resize(0);
		_neighborGrid.resize(0);
	}
	_gameBoard.clear();
	_gameBoard.resize(0);
	_neighborGrid.clear();
	_neighborGrid.resize(0);

	SaveName = (std::string)saveFile.GetPath();
	std::string buffer;
	std::ifstream fileStream;
	int LivingCells = 0;
	int index = 0;
	fileStream.open((std::string)saveFile.GetPath());
	if (fileStream.is_open()) {
		while (!fileStream.eof()) {
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			if (_gameBoard.size() == 0) {
				_settingsObj.SetGridSize(buffer.size());
				_gameBoard.resize(buffer.size());
				_neighborGrid.resize(buffer.size());
			}
			_gameBoard[index].resize(buffer.size());
			_neighborGrid[index].resize(buffer.size());
			for (int i = 0; i < buffer.size(); i++) {
				if (buffer[i] == '*') {
					_gameBoard[index][i] = true;
					LivingCells++;
				}
				else {
					_gameBoard[index][i] = false;
				}
			}
			index++;
		}
		fileStream.close();
	}
	StatusBarUpdate(_settingsObj.generationCt, LivingCells);
	_settingsObj.SetLiveCells(LivingCells);
	Refresh();
}

void MainWindow::OnSave(wxCommandEvent& event) {
	if (SaveName == "") {
		wxFileDialog saveFile(this, "Save Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFile.ShowModal() == wxID_CANCEL) {
			return;
		}
		SaveName = (std::string)saveFile.GetPath();
	}
	
	std::ofstream fileStream;
	fileStream.open(SaveName);
	
	if (fileStream.is_open()) {
		for (int i = 0; i < _gameBoard.size(); i++) {
			for (int j = 0; j < _gameBoard.size(); j++) {
				if (_gameBoard[i][j] == true) {
					fileStream << '*';
				}
				else {
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}
}

void MainWindow::OnSaveAs(wxCommandEvent& event) {
	wxFileDialog saveFile(this, "Save Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFile.ShowModal() == wxID_CANCEL) {
		return;
	}
	std::ofstream fileStream;
	fileStream.open((std::string)saveFile.GetPath());
	if (fileStream.is_open()) {
		for (int i = 0; i < _gameBoard.size(); i++) {
			for (int j = 0; j < _gameBoard.size(); j++) {
				if (_gameBoard[i][j] == true) {
					fileStream << '*';
				}
				else {
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}
}

void MainWindow::OnImport(wxCommandEvent& event) {
	wxFileDialog saveFile(this, "Open Game of Life File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (saveFile.ShowModal() == wxID_CANCEL) {
		return;
	}
	
	std::string buffer;
	std::ifstream fileStream;
	int LivingCells = 0;
	int index = 0;
	fileStream.open((std::string)saveFile.GetPath());
	if (fileStream.is_open()) {
		
		int centerPos = _gameBoard[0].size() / 2;

		while (!fileStream.eof()) {
			std::getline(fileStream, buffer);
			if (buffer.size() == 0) { break; }
			
			int startPos = centerPos - buffer.size() / 2;

			if (index < _gameBoard.size()) {
				for (int i = 0; i < buffer.size(); i++) {
					int row = startPos + index;
					int col = startPos + i;
					if (row >= 0 && row < _gameBoard.size() && col >= 0 && col < _gameBoard[row].size()) {
						if (buffer[i] == '*') {
							_gameBoard[row][col] = true;
							LivingCells++;
						}
						else {
							_gameBoard[row][col] = false;
						}
					}
				}
			}
			index++;
		}
		fileStream.close();
	}
	StatusBarUpdate(_settingsObj.generationCt, LivingCells);
	_settingsObj.SetLiveCells(LivingCells);
	Refresh();
}

void MainWindow::OnExit(wxCommandEvent& event) {
	this->Close();
}

// Event handler for show neighbor count checkable option
void MainWindow::OnNeighborCount(wxCommandEvent& event) {
	_settingsObj._isNeighborShow = !_settingsObj._isNeighborShow;
	Refresh();
	RefreshMenuItems();
}

// Event Handler for finite checkable option
void MainWindow::OnFiniteClick(wxCommandEvent& event) {
	_settingsObj._isFinite = true;
	Refresh();
	RefreshMenuItems();
}

// Event handler for toroidal checkable option
void MainWindow::OnToroidalClick(wxCommandEvent& event) {
	_settingsObj._isFinite = false;
	Refresh();
	RefreshMenuItems();
}

void MainWindow::OnShowGridLines(wxCommandEvent& event) {
	_settingsObj._isShowGrid = !_settingsObj._isShowGrid;
	Refresh();
	RefreshMenuItems();
}

void MainWindow::OnShow10x10(wxCommandEvent& event) {
	_settingsObj._isShow10x10 = !_settingsObj._isShow10x10;
	Refresh();
	RefreshMenuItems();
}

void MainWindow::OnShowHUD(wxCommandEvent& event) {
	_settingsObj._isShowHUD = !_settingsObj._isShowHUD;
	Refresh();
	RefreshMenuItems();
}

void MainWindow::OnRandomTime(wxCommandEvent& event) {
	RandomizeGrid(time(NULL));
	Refresh();
}

void MainWindow::OnRandomSeed(wxCommandEvent& event) {
	long seed = wxGetNumberFromUser("What seed # would you like to use?", "Seed:", "Seed Selection", LONG_MAX, 0, LONG_MAX);
	
	if (seed == -1) {
		return;
	}
	RandomizeGrid(seed);
	Refresh();
}

void MainWindow::RandomizeGrid(int rType) {
	srand(rType);
	int LivingCells = 0;
	for (int i = 0; i < _gameBoard.size(); i++) {
		for (int j = 0; j < _gameBoard[i].size(); j++) {
			int num = rand();
			if (num % 3 == 0) {
				_gameBoard[i][j] = true;
				LivingCells++;
			}
			else {
				_gameBoard[i][j] = false;
			}
		}
	}
	
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			int neighborCount = CountLiveNeighbors(_gameBoard, i, j);
			_neighborGrid[i][j] = neighborCount;
		}
	}
	StatusBarUpdate(_settingsObj.generationCt, LivingCells);
	_settingsObj.SetLiveCells(LivingCells);
	Refresh();
}

// Method to refresh menu choices and save the current status of checkable items
void MainWindow::RefreshMenuItems() {
	_neighborCount->Check(_settingsObj._isNeighborShow);
	_finite->Check(_settingsObj._isFinite);
	_toroidal->Check(!_settingsObj._isFinite);
	_showGrid->Check(_settingsObj._isShowGrid);
	_show10x10->Check(_settingsObj._isShow10x10);
	_showHUD->Check(_settingsObj._isShowHUD);
	Refresh();
	_settingsObj.Settings::SaveSettings();
}

// Event handler for the timer - calls NextGeneration method constantly while timer is running
void MainWindow::TimerOn(wxTimerEvent& timer) {
	NextGeneration();
}

// Calculates number of live neighbors (value of true) around the given cell at (x, y)
int MainWindow::CountLiveNeighbors(std::vector<std::vector<bool>>& grid, int x, int y) {
	// count variable for number of live cells
	int count = 0;
	// Two arrays representing the relative x and y offsets for the eight neighboring cells
	int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	int numRows = grid.size();
	int numCols = grid[0].size();
	// iterate through all eight neighbor cells
	// Changes logic based on if Finite or Toroidal universe is selected
	for (int i = 0; i < 8; i++) {
		// If Finite universe is selected
		if (_settingsObj.GetFinite() == true) {
			int newX = x + dx[i];
			int newY = y + dy[i];
			if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid.size()) {
				if (grid[newX][newY] == true) {
					count++;
				}
			}
		}
		// if Toroidal universe is selected
		else if (_settingsObj.GetFinite() == false) {
			int newX = (x + dx[i] + numRows) % numRows;
			int newY = (y + dy[i] + numCols) % numCols;
			if (grid[newX][newY] == true) {
				count++;
			}
		}
	}
	// Return total number of live neighbors
	return count;
}

// Initialize the game board with a specified size (_gridSize)
void MainWindow::InitializeGrid() {
	// Resize the game board and neighborGrid to have (_gridSize) rows
	_gameBoard.resize(_settingsObj.gridSize);
	_neighborGrid.resize(_settingsObj.gridSize);
	// For each row [i] resizes the row to have (_gridSize) columns
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			_gameBoard[i].resize(_settingsObj.gridSize);
			_neighborGrid[i].resize(_settingsObj.gridSize);
		}
	}
	// Call the SetGridSize function on the drawing panel (set to _gridSize)
	_panel->DrawingPanel::SetGridSize(_settingsObj.gridSize);
}

// Method to count cells, change living/dead cells based on game rules, and update the gameboard
void MainWindow::NextGeneration() {
	// Declare a temp gameboard to make changes to, and eventually replace real gameboard with 
	std::vector<std::vector<bool>> sandbox;
	// Resize sandbox to the size of the main gameboard (outer->inner)
	sandbox.resize(_settingsObj.gridSize);
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			sandbox[i].resize(_settingsObj.gridSize);
		}
	}
	
	// Iterate over each cell in sandbox
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			// For each cell at position [i][j], calls CountLiveNeighbors to count the live adjacent cells
			int currentCount = CountLiveNeighbors(_gameBoard, i, j);
			// Sets cell at [i][j] to live(true) or dead(false) based on game rules
			if (_gameBoard[i][j] == true && (currentCount < 2 || currentCount > 3)) {
				sandbox[i][j] = false;
			}
			else if (_gameBoard[i][j] == true && (currentCount == 2 || currentCount == 3)) {
				sandbox[i][j] = true;
			}
			else if (_gameBoard[i][j] == false && currentCount == 3) {
				sandbox[i][j] = true;
			}
		}
	}
	// After processing all cells in sandbox, updates gameboard with sandbox's values
	_gameBoard.swap(sandbox);

	// Update neighbor count grid vector for showing neighbor counts
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			// For each cell at position [i][j], calls CountLiveNeighbors to count the live adjacent cells
			int currentCount = CountLiveNeighbors(_gameBoard, i, j);
			_neighborGrid[i][j] = currentCount;
		}
	}

	// Count the live cells in the updated gameboard
	int NewCellCount = 0;
	for (int i = 0; i < _settingsObj.gridSize; i++) {
		for (int j = 0; j < _settingsObj.gridSize; j++) {
			if (_gameBoard[i][j] == true) {
				NewCellCount++;
			}
		}
	}
	// Set living cell count to updated count, increment generation count, update status bar, and refresh panel
	_numOfGenerations++;
	_settingsObj.SetGenerationCt(_numOfGenerations);
	_livingCells = NewCellCount;
	_settingsObj.SetLiveCells(_livingCells);
	StatusBarUpdate(_numOfGenerations, _livingCells);
	
	Refresh();
}

MainWindow::~MainWindow() {
	
}
