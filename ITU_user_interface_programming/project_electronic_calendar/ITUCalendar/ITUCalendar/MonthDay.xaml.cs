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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ITUCalendar {
    public partial class MonthDay : UserControl {

        public int index { get; set; }

        public MonthDay(int index) {
            this.index = index;
            InitializeComponent();
        }

        private void GridClick(object sender, MouseButtonEventArgs e) {
            var day = ((MonthViewModel)this.DataContext).Days[this.index];

            var ev = new Event() {
                Name = "Zadejte jméno",
                Description = "Zadejte popis",
                StartDate = day,
                EndDate = day,
                IsEntireDay = false,
                Color = Color.FromArgb(255, 180, 40, 100)
            };
            var window = new EventWindow(
                new Tuple<Event, Calendar>(ev, null)
            );
            window.Show();
        }
    }

    public class DateTimeToContent : IValueConverter {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture) {
            return ((DateTime)value).Day.ToString();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    public class DateTimeToDayForeground : IMultiValueConverter {
        public object Convert(object[] value, Type targetType, object parameter, CultureInfo culture) {
            if (!(value[0] is DateTime) || !(value[1] is DateTime)) {
                throw new Exception("Invalid types passed into converter");
            }

            if (((DateTime)value[0]).Month != ((DateTime)value[1]).Month) {
                return new SolidColorBrush(Color.FromRgb(160, 160 ,160));
            } else {
                return new SolidColorBrush(Color.FromRgb(255, 255, 255));
            }
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }

    public class DateTimeToWeekendBackground : IMultiValueConverter {
        public object Convert(object[] value, Type targetType, object parameter, CultureInfo culture) {
            if (!(value[0] is DateTime) || !(value[1] is DateTime)) {
                throw new Exception("Invalid types passed into converter");
            }

            var day = ((DateTime)value[0]);

            var today = ((DateTime)value[1]);

            if (day.Date == today.Date) {
                return Application.Current.FindResource("MonthDayToday");
            } else if (day.DayOfWeek == DayOfWeek.Sunday || day.DayOfWeek == DayOfWeek.Saturday) {
                return Application.Current.FindResource("MonthDayWeekend");
            } else {
                return Application.Current.FindResource("MonthDay");
            }
        }

        public object[] ConvertBack(object value, Type[] targetType, object parameter, CultureInfo culture) {
            throw new NotImplementedException();
        }
    }
}
