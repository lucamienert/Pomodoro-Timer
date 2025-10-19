#include "PomodoroWindow.h"
#include <gtkmm.h>

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.example.pomodoro");

    PomodoroWindow win;

    return app->make_window_and_run<PomodoroWindow>(argc, argv);
}
