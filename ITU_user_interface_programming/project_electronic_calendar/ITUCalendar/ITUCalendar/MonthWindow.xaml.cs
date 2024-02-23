// Petr Volf, xvolfp00

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
    public partial class MonthWindow : Window {
        public MonthWindow() {
            this.DataContext = new MonthViewModel();

            InitializeComponent();
            Left = Properties.Settings.Default.WindowX;
            Top = Properties.Settings.Default.WindowY;

            //populate field
            for (var i = 0; i < 42; i++) {
                var md = new MonthDay(i) {
                    DataContext = this.DataContext
                };

                var bind_name = new Binding($"Days[{i}]") {
                    Converter = new DateTimeToContent()
                };
                BindingOperations.SetBinding(md.DayName, TextBlock.TextProperty, bind_name);

                var bind_days = new Binding($"Days[{i}]");

                var bind_month = new Binding("CurrentMonth");

                var bind_color = new MultiBinding() {
                    Bindings = {bind_days, bind_month},
                    Converter = new DateTimeToDayForeground()
                };

                BindingOperations.SetBinding(md.DayName, TextBlock.ForegroundProperty, bind_color);

                var bind_today = new Binding("Today");

                var bind_style = new MultiBinding() {
                    Bindings = {bind_days, bind_today},
                    Converter = new DateTimeToWeekendBackground()
                };

                BindingOperations.SetBinding(md.Background, Grid.StyleProperty, bind_style);

                var bind_events = new Binding($"Events[{i}]");

                BindingOperations.SetBinding(md.EventList, ListView.ItemsSourceProperty, bind_events);

                if (i % 7 == 0) {
                    var thicc = md.Border.BorderThickness;
                    thicc.Left = 0;
                    md.Border.BorderThickness = thicc;
                }
                
                Grid.SetColumn(md, i % 7);
                Grid.SetRow(md, i / 7);
                DayContainer.Children.Add(md);
            }
        }

        private void OpenSettings(object sender, RoutedEventArgs e) {
            var settings = new Settings();
            settings.Show();
        }

        private void MoveMonthRight(object sender, RoutedEventArgs e) {
            ((MonthViewModel)this.DataContext).MoveRight();
        }

        private void MoveMonthLeft(object sender, RoutedEventArgs e) {
            ((MonthViewModel)this.DataContext).MoveLeft();
        }

        private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e) {
            Properties.Settings.Default.DefaultView = "Month";
            Properties.Settings.Default.WindowX = Left;
            Properties.Settings.Default.WindowY = Top;
            Properties.Settings.Default.Save();
        }

        private void CloseWindow(object sender, RoutedEventArgs e) {
            this.Close();
            Application.Current.Shutdown();
        }

        private void ToggleDayNameVisibility(object sender, RoutedEventArgs e) {
            ((MonthViewModel)this.DataContext).DayNameVisibilityToggle();
        }

        private void AddCalendarEvent(object sender, RoutedEventArgs e) {
            throw new NotImplementedException();
        }
    }

    public class MonthToName : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) {
            return ((DateTime)value).ToString("MMMM yyyy", new CultureInfo("cs-CZ"));
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    public class BoolToVisibility : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) {
            if (parameter == null) {
                return ((bool)value) switch
                {
                    true => Visibility.Visible,
                    false => Visibility.Collapsed
                };
            } else {
                if (parameter is bool) {
                    return (((bool)value) ^ ((bool)parameter)) switch
                    {
                        true => Visibility.Visible,
                        false => Visibility.Collapsed
                    };
                } else {
                    var b = Boolean.Parse((string)parameter);
                    return (((bool)value) ^ b) switch
                    {
                        true => Visibility.Visible,
                        false => Visibility.Collapsed
                    };
                }
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    public class BoolToRowHeight : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) =>
        ((bool)value) switch
        {
            true => "30px",
            false => "0px"
        };

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    public class BoolToIcon : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) =>
        ((bool)value) switch
        {
            true => "\uf077",
            false => "\uf078"
        };

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }
}
