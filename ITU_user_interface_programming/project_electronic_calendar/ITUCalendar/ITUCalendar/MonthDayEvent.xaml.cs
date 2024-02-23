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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ITUCalendar {
    /// <summary>
    /// Interaction logic for MonthDayEvent.xaml
    /// </summary>
    public partial class MonthDayEvent : UserControl {
        public MonthDayEvent() {
            InitializeComponent();
        }

        private void OpenEventWindow(object sender, RoutedEventArgs e) {
            var ev = new EventWindow(
                new Tuple<Event, Calendar>((Event)DataContext, CalendarModel.FindCalendarByEvent((Event)DataContext))
            );
            ev.Show();
        }
    }
}
