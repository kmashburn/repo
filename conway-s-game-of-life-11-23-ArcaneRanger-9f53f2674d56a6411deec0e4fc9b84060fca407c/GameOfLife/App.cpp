#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {
	MainWindow* view = new MainWindow();
	view->Show();
	return true;
}