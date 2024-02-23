// Petr Volf, xvolfp00
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Media;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ITUCalendar {
    public class MonthViewModel : INotifyPropertyChanged {
        public DateTime CurrentMonth { get; set; }
        public List<DateTime> Days { get; set; }
        public List<List<Event>> Events { get; set; }
        public DateTime Today { get; set; }
        public bool DayNameVisibility{ get; set; }

        public MonthViewModel() {
            this.CurrentMonth = new DateTime(DateTime.Now.Year, DateTime.Now.Month, 1);
            this.Days = new List<DateTime>();
            this.Today = DateTime.Now;
            this.DayNameVisibility = true;
            this.Events = new List<List<Event>>();

            var month = this.CurrentMonth;

            var start = month.AddDays(-((int)month.DayOfWeek + 6) % 7);

            //populate field
            for (var i = 0; i < 42; i++) {
                Days.Add(start);
                Events.Add(CalendarModel.GetEventsByDate(start));
                start = start.AddDays(1);
            }
        }

        public void MoveLeft() {
            CurrentMonth = CurrentMonth.AddMonths(-1);
            OnPropertyChanged("CurrentMonth");
            var month = this.CurrentMonth;
            var start = month.AddDays(-((int)month.DayOfWeek + 6) % 7);

            //populate field
            for (var i = 0; i < 42; i++) {
                Days[i] = start;
                Events[i] = CalendarModel.GetEventsByDate(start);
                start = start.AddDays(1);
            }
            OnPropertyChanged("Days");
            OnPropertyChanged("Events");
        }

        public void MoveRight() {
            CurrentMonth = CurrentMonth.AddMonths(1);
            OnPropertyChanged("CurrentMonth");
            var month = this.CurrentMonth;
            var start = month.AddDays(-((int)month.DayOfWeek + 6) % 7);

            //populate field
            for (var i = 0; i < 42; i++) {
                Days[i] = start;
                Events[i] = CalendarModel.GetEventsByDate(start);
                start = start.AddDays(1);
            }
            OnPropertyChanged("Days");
            OnPropertyChanged("Events");
        }

        public List<Event> GetEventsForDay(DateTime day) {
            return CalendarModel.GetEventsByDate(day);
        }

        public void DayNameVisibilityToggle() {
            this.DayNameVisibility = !this.DayNameVisibility;
            OnPropertyChanged("DayNameVisibility");
        }

        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}
