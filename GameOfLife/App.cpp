#include "App.h"
#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "SettingsDialog.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

// MainWindow instantiation (essentially main.cpp)
bool App::OnInit() {
	MainWindow* window = new MainWindow();
	
	window->Show();

	return true;
}