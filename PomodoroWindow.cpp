#include "PomodoroWindow.h"
#include <iostream>
#include <cstdlib>

PomodoroWindow::PomodoroWindow()
{
    set_title("Pomodoro Timer");
    set_default_size(320, 180);
    m_vbox.set_margin_top(10);
    m_vbox.set_margin_bottom(10);
    m_vbox.set_margin_start(10);
    m_vbox.set_margin_end(10);

    set_child(m_vbox);

    m_mode.append("Work — 25:00");
    m_mode.append("Short Break — 5:00");
    m_mode.append("Long Break — 15:00");
    m_mode.set_active(0);
    m_mode.signal_changed().connect(sigc::mem_fun(*this, &PomodoroWindow::on_mode_changed));

    m_vbox.append(m_mode);

    m_time_label.set_margin_top(6);
    m_time_label.set_margin_bottom(6);
    m_time_label.set_halign(Gtk::Align::CENTER);
    m_time_label.set_valign(Gtk::Align::CENTER);
    m_time_label.set_markup("<span size=22000 weight='bold'>25:00</span>");
    m_vbox.append(m_time_label);

    m_progress.set_show_text(true);
    m_progress.set_fraction(0.0);
    m_vbox.append(m_progress);

    m_start_pause_btn.signal_clicked().connect(sigc::mem_fun(*this, &PomodoroWindow::on_start_pause));
    m_reset_btn.signal_clicked().connect(sigc::mem_fun(*this, &PomodoroWindow::on_reset));

    m_hbox_controls.append(m_start_pause_btn);
    m_hbox_controls.append(m_reset_btn);
    m_vbox.append(m_hbox_controls);

    set_mode_seconds(m_total_seconds);
    update_ui();
}

void PomodoroWindow::set_mode_seconds(int seconds)
{
    m_total_seconds = seconds;
    m_remaining_seconds = seconds;
}

void PomodoroWindow::on_mode_changed()
{
    int idx = m_mode.get_active_row_number();
    switch (idx)
    {
        case 0: 
            set_mode_seconds(25 * 60);
            break;
        
        case 1:
            set_mode_seconds(5 * 60);
            break;
        
        case 2: 
            set_mode_seconds(15 * 60);
            break;

        default: set_mode_seconds(25 * 60);
    }

    if (m_running)
    {
        m_conn.disconnect();
        m_running = false;
        m_start_pause_btn.set_label("Start");
    }

    update_ui();
}

void PomodoroWindow::on_start_pause()
{
    if (!m_running)
    {
        m_running = true;
        m_start_pause_btn.set_label("Pause");

        if (m_conn.connected())
            m_conn.disconnect();

        m_conn = Glib::signal_timeout().connect_seconds(sigc::mem_fun(*this, &PomodoroWindow::on_tick), 1);
    }
    else
    {
        m_running = false;
        m_start_pause_btn.set_label("Start");
        
        if (m_conn.connected())
            m_conn.disconnect();
    }
}

void PomodoroWindow::on_reset()
{
    if (m_conn.connected())
        m_conn.disconnect();
    
    m_running = false;
    m_start_pause_btn.set_label("Start");
    m_remaining_seconds = m_total_seconds;
    update_ui();
}

bool PomodoroWindow::on_tick()
{
    if (m_remaining_seconds > 0)
    {
        --m_remaining_seconds;
        update_ui();

        if (m_remaining_seconds == 0)
        {
            if (m_conn.connected())
                m_conn.disconnect();
            m_running = false;
            m_start_pause_btn.set_label("Start");
            return false;
        }

        return true;
    }

    return false;
}

void PomodoroWindow::update_ui()
{
    int mins = m_remaining_seconds / 60;
    int secs = m_remaining_seconds % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", mins, secs);

    std::string markup = "<span size=22000 weight='bold'>" + std::string(buf) + "</span>";
    m_time_label.set_markup(markup);

    if (m_total_seconds > 0)
    {
        double frac = double(m_total_seconds - m_remaining_seconds) / double(m_total_seconds);
        m_progress.set_fraction(frac);
        m_progress.set_text(buf);
    }
    else
    {
        m_progress.set_fraction(0.0);
        m_progress.set_text("");
    }
}
