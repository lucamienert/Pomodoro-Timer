#pragma once

#include <gtkmm.h>

class PomodoroWindow : public Gtk::Window
{
public:
    PomodoroWindow();
    virtual ~PomodoroWindow() {}

protected:
    void on_start_pause();
    void on_reset();
    void on_mode_changed();
    bool on_tick();

    void set_mode_seconds(int seconds);
    void update_ui();

    Gtk::Box m_vbox{Gtk::Orientation::VERTICAL, 8};
    Gtk::Box m_hbox_controls{Gtk::Orientation::HORIZONTAL, 6};
    Gtk::ComboBoxText m_mode;
    Gtk::Label m_time_label;
    Gtk::ProgressBar m_progress;
    Gtk::Button m_start_pause_btn{"Start"};
    Gtk::Button m_reset_btn{"Reset"};

    int m_total_seconds{25 * 60};
    int m_remaining_seconds{25 * 60};
    bool m_running{false};
    sigc::connection m_conn;
};
