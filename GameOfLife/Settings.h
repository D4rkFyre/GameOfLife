#pragma once
#include <wx/wx.h>
#include "wx/colour.h"
#include <fstream>

struct Settings 
{
	int gridSize = 30;
	int timerInterval = 100;

	int generationCt = 0;
	int liveCells = 0;

	unsigned int livingCellRed = 128;
	unsigned int livingCellGreen = 128;
	unsigned int livingCellBlue = 128;
	unsigned int livingCellAlpha = 255;

	unsigned int deadCellRed = 255;
	unsigned int deadCellGreen = 255;
	unsigned int deadCellBlue = 255;
	unsigned int deadCellAlpha = 255;

	unsigned int gridColourRed = 0;
	unsigned int gridColourGreen = 0;
	unsigned int gridColourBlue = 0;
	unsigned int gridColourAlpha = 255;
	
	bool _isFinite = true;
	bool _isNeighborShow = true;
	bool _isShowGrid = true;
	bool _isShow10x10 = true;
	bool _isShowHUD = true;

	Settings() {
		LoadSettings();
	}

	~Settings() {}

	int GetGridSize() {
		return gridSize;
	}

	void SetGridSize(int newSize) {
		gridSize = newSize;
	}

	int GetTimerInterval() {
		return timerInterval;
	}

	void SetTimerInterval(int newInterval) {
		timerInterval = newInterval;
	}

	int GetGenerationCt() {
		return generationCt;
	}

	void SetGenerationCt(int newGeneration) {
		generationCt = newGeneration;
	}

	int GetLiveCells() {
		return liveCells;
	}

	void SetLiveCells(int newLiveCells) {
		liveCells = newLiveCells;
	}

	void SetLivingCellColour(wxColour colour) {
		livingCellRed = colour.GetRed();
		livingCellGreen = colour.GetGreen();
		livingCellBlue = colour.GetBlue();
		livingCellAlpha = colour.GetAlpha();
	}

	wxColour GetLivingCellColour() {
		wxColour livingCellColour(livingCellRed, livingCellGreen, livingCellBlue, livingCellAlpha);
		return livingCellColour;
	}

	void SetDeadCellColour(wxColour colour) {
		deadCellRed = colour.GetRed();
		deadCellGreen = colour.GetGreen();
		deadCellBlue = colour.GetBlue();
		deadCellAlpha = colour.GetAlpha();
	}

	wxColour GetDeadCellColour() {
		wxColour deadCellColour(deadCellRed, deadCellGreen, deadCellBlue, deadCellAlpha);
		return deadCellColour;
	}

	void SetGridColour(wxColour colour) {
		gridColourRed = colour.GetRed();
		gridColourGreen = colour.GetGreen();
		gridColourBlue = colour.GetBlue();
		gridColourAlpha = colour.GetAlpha();
	}

	wxColour GetGridColour() {
		wxColour gridColour(gridColourRed, gridColourGreen, gridColourBlue, gridColourAlpha);
		return gridColour;
	}

	void SetFinite(bool finite) {
		_isFinite = finite;
	}

	bool GetFinite() {
		bool isFinite = _isFinite;
		return isFinite;
	}

	wxString GetUniverseType() {
		wxString universe;
		if (_isFinite) {
			universe = wxT("Finite");
		}
		else {
			universe = wxT("Toroidal");
		}

		return universe;
	}

	void SetNeighborShow(bool neighborShow) {
		_isNeighborShow = neighborShow;
	}

	bool GetNeighborShow() {
		bool isNeighborShow = _isNeighborShow;
		return isNeighborShow;
	}

	void SetShowGrid(bool gridShow) {
		_isShowGrid = gridShow;
	}

	bool GetShowGrid() {
		bool isShowGrid = _isShowGrid;
		return isShowGrid;
	}

	void SetShow10x10(bool show10x10) {
		_isShow10x10 = show10x10;
	}

	bool GetShow10x10() {
		bool isShow10x10 = _isShow10x10;
		return isShow10x10;
	}

	void SetShowHUD(bool showHUD) {
		_isShowHUD = showHUD;
	}

	bool GetShowHUD() {
		bool isShowHUD = _isShowHUD;
		return isShowHUD;
	}

	void LoadSettings() {
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
		generationCt = 0;
		liveCells = 0;
	}

	void SaveSettings() {
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}

};