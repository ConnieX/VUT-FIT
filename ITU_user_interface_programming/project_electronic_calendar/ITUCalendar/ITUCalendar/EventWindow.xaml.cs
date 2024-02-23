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
    /// <summary>
    /// Interaction logic for EventWindow.xaml
    /// </summary>
    public partial class EventWindow : Window {
        public EventWindow(object dataContext) {
            InitializeComponent();
            DataContext = dataContext;
            CalendarChoice.ItemsSource = CalendarModel.Calendars;
            var selected = ((Tuple<Event, Calendar>)DataContext).Item2;
            if (selected == null) {
                CalendarChoice.SelectedItem = -1;
            } else {
                CalendarChoice.SelectedItem = ((Tuple<Event, Calendar>)DataContext).Item2;
            }
        }

        private void CloseWindow(object sender, RoutedEventArgs e) {
            Close();
        }

        private void EventNameChanged(object sender, TextChangedEventArgs e) {
            ((Tuple<Event, Calendar>)DataContext).Item1.Name = ((TextBox)sender).Text;
        }

        private void DeleteEvent(object sender, RoutedEventArgs e) {
            CalendarModel.DeleteEvent(((Tuple<Event, Calendar>)DataContext).Item1);
            Close();
        }

        private void CalendarChoice_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            var ev = ((Tuple<Event, Calendar>)DataContext).Item1;
            CalendarModel.DeleteEvent(ev);
            CalendarModel.AddEvent(ev, (Calendar)CalendarChoice.SelectedItem);
        }

        private void EventInfoChanged(object sender, TextChangedEventArgs e) {
            ((Tuple<Event, Calendar>)DataContext).Item1.Description = ((TextBox)sender).Text;
        }
    }
}
