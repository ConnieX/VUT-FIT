// Petr Volf, xvolfp00
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ITUCalendar {
    public partial class Settings : Window {
        public Settings() {
            InitializeComponent();
            Calendars.ItemsSource = CalendarModel.Calendars;
            Groups.ItemsSource = CalendarModel.Groups;
            switch (Properties.Settings.Default.DefaultView) {
                case "Month":
                    MonthButton.IsEnabled = false;
                    break;
                case "Day":
                    DayButton.IsEnabled = false;
                    break;
                case "Week":
                    WeekButton.IsEnabled = false;
                    break;
            }
        }

        private void MonthClick(object sender, RoutedEventArgs e) {
            foreach (Window window in Application.Current.Windows) {
                if (window is WeekWindow || window is DayWindow) {
                    window.Close();
                }
            }
            var win = new MonthWindow();
            win.Show();
            Properties.Settings.Default.DefaultView = "Month";
            Properties.Settings.Default.Save();
            MonthButton.IsEnabled = false;
            DayButton.IsEnabled = true;
            WeekButton.IsEnabled = true;
        }
        private void DayClick(object sender, RoutedEventArgs e) {
            foreach (Window window in Application.Current.Windows) {
                if (window is WeekWindow || window is MonthWindow) {
                    window.Close();
                }
            }
            var win = new DayWindow();
            win.Show();
            Properties.Settings.Default.DefaultView = "Day";
            Properties.Settings.Default.Save();
            MonthButton.IsEnabled = true;
            DayButton.IsEnabled = false;
            WeekButton.IsEnabled = true;
        }
        private void WeekClick(object sender, RoutedEventArgs e) {
            foreach (Window window in Application.Current.Windows) {
                if (window is MonthWindow || window is DayWindow) {
                    window.Close();
                }
            }
            var win = new WeekWindow();
            win.Show();
            Properties.Settings.Default.DefaultView = "Week";
            Properties.Settings.Default.Save();
            MonthButton.IsEnabled = true;
            DayButton.IsEnabled = true;
            WeekButton.IsEnabled = false;
        }

        private void OnClosed(object sender, RoutedEventArgs e) {
            Close();
        }

        private void GoogleConnect(object sender, RoutedEventArgs e) {
            MessageBox.Show("Tohle tlacitko nic nedela");
        }
    }
}
