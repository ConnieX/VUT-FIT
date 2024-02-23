/* Autor: Monika Rosinská, xrosin00
   ITU projekt
   8. 12. 2019 */
using System;
using System.Collections.Generic;
using System.Globalization;
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
    public partial class WeekWindow : Window {
        DateTime date;
        public WeekWindow() {
            InitializeComponent();

            Left = Properties.Settings.Default.WindowX;
            Top = Properties.Settings.Default.WindowY;

            date = DateTime.Now;
            InitializeComponent();
            DayDate(date);
            RowsGen(true);
        }

        //open setting window
        private void OpenSettings(object sender, RoutedEventArgs e) {
            var settings = new Settings();
            settings.Show();
        }

        //app remebers last used view and next time opens this view
        private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e) {
            Properties.Settings.Default.DefaultView = "Week";
            Properties.Settings.Default.WindowX = Left;
            Properties.Settings.Default.WindowY = Top;
            Properties.Settings.Default.Save();
        }

        private void CloseWindow(object sender, RoutedEventArgs e) {
            this.Close();
        }

        //hides row with day names
        private void ToggleDayNameVisibility(object sender, RoutedEventArgs e) {
            if (DaysHeader.Visibility == Visibility.Visible) {
                DaysHeader.Visibility = Visibility.Collapsed;
                DaysRow.Height = new GridLength(0);
                Arrow.Content = "\uf078";
            }
            else {
                DaysHeader.Visibility = Visibility.Visible;
                DaysRow.Height = new GridLength(30);
                Arrow.Content = "\uf077";
            }
        }

        //opens window for creating new event
        private void AddCalendarEvent(object sender, RoutedEventArgs e) {
            var ev = new Event() {
                Name = "Jméno události",
                Description = "Popis události",
                StartDate = date,
                EndDate = date,
                IsEntireDay = false,
                Color = Color.FromArgb(200, 140, 140, 50)
            };
            var newEvent = new EventWindow(new Tuple<Event, Calendar>(ev, null));
            newEvent.Show();
        }

        //printing row with day names and date 
        private void DayDate(DateTime date) {
            DateTimeFormatInfo dfi = DateTimeFormatInfo.CurrentInfo;
            System.Globalization.Calendar cal = dfi.Calendar;
            MonthName.Content = date.ToString("MMMM yyyy, ", new CultureInfo("cs-CZ")) + cal.GetWeekOfYear(date, dfi.CalendarWeekRule, dfi.FirstDayOfWeek) + ". týden";
            int diff = (7 + (date.DayOfWeek - DayOfWeek.Monday)) % 7;
            date = date.AddDays(-1 * diff).Date;
            Mon.Text = "Pondělí" + date.ToString("  d.");
            date = date.AddDays(1);
            Tues.Text = "Úterý" + date.ToString("  d.");
            date = date.AddDays(1);
            Wed.Text = "Středa" + date.ToString("  d.");
            date = date.AddDays(1);
            Thr.Text = "Čtvrtek" + date.ToString("  d.");
            date = date.AddDays(1);
            Fri.Text = "Pátek" + date.ToString("  d.");
            date = date.AddDays(1);
            Sut.Text = "Sobota" + date.ToString("  d.");
            date = date.AddDays(1);
            Sun.Text = "Neděle" + date.ToString("  d.");
            DataFill(date);
        }

        //generating border of rows
        private void RowsGen(bool first) {
            for (int j = 0; j < 24; j++) {
                if (first) {
                    Days.RowDefinitions.Add(new RowDefinition());
                }
                TextBlock time = new TextBlock();
                time.Text = j.ToString();
                time.HorizontalAlignment = HorizontalAlignment.Right;
                time.Margin = new Thickness(0, 0, 3, 0);
                Grid.SetRow(time, j);
                Days.Children.Add(time);
                time.Foreground = Brushes.White;
                Border border = new Border();
                border.BorderThickness = new Thickness(0, 1, 0, 0);
                border.BorderBrush = Brushes.Gray;
                Grid.SetRow(border, j);
                Grid.SetColumnSpan(border, 8);
                Days.Children.Add(border);
                border.MouseEnter += (s, ev) => border.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#7FFFFFFF"));
                border.MouseLeave += (s, ev) => border.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#1000"));
            }

            for (int i = 1; i < 8; i++) {
                Border border = new Border();
                border.BorderThickness = new Thickness(1, 0, 0, 0);
                border.BorderBrush = Brushes.White;
                Grid.SetColumn(border, i);
                Grid.SetRowSpan(border, 24);
                Days.Children.Add(border);
            }
        }

        //move to next week
        private void MoveWeekRight(object sender, RoutedEventArgs e) {
            date = date.AddDays(7);
            DayDate(date);
        }

        //move to previous week
        private void MoveWeekLeft(object sender, RoutedEventArgs e) {
            date = date.AddDays(-7);
            DayDate(date);
        }

        //showing all events of week
        private void DataFill(DateTime date) {
            Days.Children.Clear();
            RowsGen(false);
            int diff = (7 + (date.DayOfWeek - DayOfWeek.Monday)) % 7;
            date = date.AddDays(-1 * diff);
            for(int i = 0; i < 7; i++) {
                var dayEvent = CalendarModel.GetEventsByDate(date);
                foreach(var even in dayEvent) {
                    Button newEvent = new Button();
                    newEvent.Content = even.Name;
                    newEvent.Background = new SolidColorBrush(even.Color);
                    newEvent.FontFamily = (FontFamily)FindResource("Overpass");
                    newEvent.Click += (object sender, RoutedEventArgs e) => {
                        var showEvent = new EventWindow(
                            new Tuple<Event, Calendar>(even, CalendarModel.FindCalendarByEvent(even))
                        );
                        showEvent.Show();
                    };
                    if (even.IsEntireDay == true) {
                        Grid.SetRowSpan(newEvent, 1);
                        Grid.SetRow(newEvent, 0);
                    }
                    else {
                        Grid.SetRowSpan(newEvent, even.EndDate.Hour - even.StartDate.Hour);
                        Grid.SetRow(newEvent, even.StartDate.Hour);
                    }
                    Grid.SetColumn(newEvent, i + 1);
                    Days.Children.Add(newEvent);
                }
                date = date.AddDays(1);
            }
        }
    }
}