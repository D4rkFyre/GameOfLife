#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include <wx/wx.h>

// Event table for OnPaint and a click handler
wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
	EVT_PAINT(DrawingPanel::OnPaint)
	EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

// DrawingPanel constructor using the MainWindow as a parent and a reference to the gameboard
DrawingPanel::DrawingPanel(wxFrame* parent, wxSize size, std::vector<std::vector<bool>>& gameBoard, Settings* settings, std::vector<std::vector<int>>& neighborGrid)
	: wxPanel(parent), pGameBoard(gameBoard), pSettingsObj(settings), pNeighborGrid(neighborGrid) {
	
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	Refresh();
}
      
// Method to repaint the drawing panel, drawing cells based on the state of the gameboard reference
void DrawingPanel::OnPaint(wxPaintEvent& event) {
	// Create an instance of wxAutoBufferedPaintDC named dc with drawing panel as the argument
	wxAutoBufferedPaintDC dc(this);
	// Clear content of the device context(dc), to start with a clean slate
	dc.Clear();
	// Create a graphics context that allows drawing operations on the panel
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	// If the creation fails, the function returns early
	if (!context) { return; }
	
	// Set pen and brush colors (outlines, and filling, respectively)
	context->SetBrush(pSettingsObj->GetDeadCellColour());
	// If _isShowGrid is false, set pen to transparent color
	if (pSettingsObj->GetShowGrid() == false) {
		context->SetPen(wxTransparentColour);
	}
	else {
		context->SetPen(pSettingsObj->GetGridColour());
	}

	// Get the width and height for each cell based on grid size and size of window
	float CellWidth = this->GetSize().x / (float)pSettingsObj->gridSize;
	float CellHeight = this->GetSize().y / (float)pSettingsObj->gridSize;

	// Set font/color for text in grid
	context->SetFont(wxFontInfo(8), *wxRED);
	// iterate through positions on grid based on size of grid
	for (int i = 0; i < pSettingsObj->gridSize; ++i) {
		for (int j = 0; j < pSettingsObj->gridSize; ++j) {
			// If cell is alive, brush is set to living cell color, if dead, brush remains dead cell color
			if (pGameBoard[i][j] == true) {
				context->SetBrush(pSettingsObj->GetLivingCellColour());
			}
			if (pGameBoard[i][j] == false) {
				context->SetBrush(pSettingsObj->GetDeadCellColour());
			}
			// Draw a rectangle at each position with the CellWidth and CellHeight dimensions
			context->DrawRectangle(i * CellWidth, j * CellHeight, CellWidth, CellHeight);
		}
	}
	
	// Display the 10x10 grid if the option is selected
	if (pSettingsObj->GetShow10x10() == true) {
		for (int i = 0; i < pSettingsObj->gridSize; ++i) {
			for (int j = 0; j < pSettingsObj->gridSize; ++j) {
				int DarkerLines = (pSettingsObj->gridSize) / 10;
				dc.SetPen(wxPen(*wxBLACK, 5));
				for (int i = 0; i < DarkerLines; i++) {

					wxPoint vStart(CellWidth * 10 * (i + 1), 0);
					wxPoint vEnd(CellWidth * 10 * (i + 1), this->GetSize().y);
					dc.DrawLine(vStart, vEnd);

					wxPoint hStart(0, CellHeight * 10 * (i + 1));
					wxPoint hEnd(this->GetSize().x, CellHeight * 10 * (i + 1));
					dc.DrawLine(hStart, hEnd);
				}
			}
		}
	}

	if (pSettingsObj->GetNeighborShow() == true) {
		for (int i = 0; i < pSettingsObj->gridSize; ++i) {
			for (int j = 0; j < pSettingsObj->gridSize; ++j) {
				// Display text in cells if Show neighbor option is checked, and cell has living neighbors
				if (pNeighborGrid[i][j] > 0) {
					if (pGameBoard[i][j] == true && (pNeighborGrid[i][j] < 2 || pNeighborGrid[i][j] > 3)) {
						context->SetFont(wxFontInfo(8), *wxRED);
					}
					else if (pGameBoard[i][j] == true && (pNeighborGrid[i][j] == 2 || pNeighborGrid[i][j] == 3)) {
						context->SetFont(wxFontInfo(8), *wxGREEN);
					}
					else if (pGameBoard[i][j] == false && pNeighborGrid[i][j] == 3) {
						context->SetFont(wxFontInfo(8), *wxGREEN);
					}

					wxString cellText = std::to_string(pNeighborGrid[i][j]);
					double textWidth, textHeight;
					context->GetTextExtent(cellText, &textWidth, &textHeight);
					context->DrawText(cellText, (i * CellWidth) + CellWidth / 2 - textWidth / 2, (j * CellHeight) + CellHeight / 2 - textHeight / 2);
					context->SetFont(wxFontInfo(8), *wxRED);
				}
			}
		}
	}

	if (pSettingsObj->GetShowHUD() == true) {
		
		double xText = 0;
		double yText = 0;
		
		wxString genText = "Generation: " + std::to_string(pSettingsObj->GetGenerationCt());
		wxString cellText = "Live Cells: " + std::to_string(pSettingsObj->GetLiveCells());
		wxString boundaryText = "Boundary Type: " + pSettingsObj->GetUniverseType();
		wxString universeText = "Window Size: " + std::to_string(GetSize().x) + " x " + std::to_string(GetSize().y);
		wxString gridText = "Grid Size: " + std::to_string(pSettingsObj->GetGridSize()) + " x " + std::to_string(pSettingsObj->GetGridSize());

		context->SetFont(wxFontInfo(12), *wxRED);

		context->GetTextExtent(genText, &xText, &yText);
		context->DrawText(genText, 5, GetSize().y - (yText * 5) - 10);

		context->GetTextExtent(cellText, &xText, &yText);
		context->DrawText(cellText, 5, GetSize().y - (yText * 4) - 10);

		context->GetTextExtent(boundaryText, &xText, &yText);
		context->DrawText(boundaryText, 5, GetSize().y - (yText * 3) - 10);

		context->GetTextExtent(universeText, &xText, &yText);
		context->DrawText(universeText, 5, GetSize().y - (yText * 2) - 10);

		context->GetTextExtent(gridText, &xText, &yText);
		context->DrawText(gridText, 5, GetSize().y - yText - 10);
	}
}

// Function for when left mouse button is released (EVT_LEFT_UP)
void DrawingPanel::OnMouseUp(wxMouseEvent& click) {
	// Get x and y coordinates of the mouse click event
	int x = click.GetX();
	int y = click.GetY();
	
	// Calculate cell width and height
	float CellWidth = this->GetSize().x / (float)pSettingsObj->gridSize;
	float CellHeight = this->GetSize().y / (float)pSettingsObj->gridSize;

	// xPos and yPos represent the row and column of the clicked cell in the grid
	int xPos = x / CellWidth;
	int yPos = y / CellHeight;

	// Toggle the state of the cell at (xPos, yPos) in the gameboard reference
	pGameBoard[xPos][yPos] = !pGameBoard[xPos][yPos];
	if (pGameBoard[xPos][yPos] == true) {
		pSettingsObj->liveCells++;
	}
	else {
		pSettingsObj->liveCells--;
	}

	// Update the Living Neighbor count in the neighbor count grid (int)
	for (int i = 0; i < pSettingsObj->gridSize; i++) {
		for (int j = 0; j < pSettingsObj->gridSize; j++) {
			// For each cell at position [i][j], calls CountLivingNeighbors to count the live adjacent cells
			int neighborCount = CountLivingNeighbors(pGameBoard, i, j);
			pNeighborGrid[i][j] = neighborCount;
		}
	}

	// Refresh the display
	Refresh();
}

// Panel Size setter
void DrawingPanel::SetNewPanelSize(wxSize& size) {
	wxPanel::SetSize(size);
	Refresh();
}

// Grid size setter
void DrawingPanel::SetGridSize(int newGridSize) {
	pSettingsObj->gridSize = newGridSize;
}

int DrawingPanel::CountLivingNeighbors(std::vector<std::vector<bool>>& grid, int x, int y)
{
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
		if (pSettingsObj->GetFinite() == true) {
			int newX = x + dx[i];
			int newY = y + dy[i];
			if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid.size()) {
				if (grid[newX][newY] == true) {
					count++;
				}
			}
		}
		// if Toroidal universe is selected
		else if (pSettingsObj->GetFinite() == false) {
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

DrawingPanel::~DrawingPanel() {}