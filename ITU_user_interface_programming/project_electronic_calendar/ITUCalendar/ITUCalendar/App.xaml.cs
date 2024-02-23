// Petr Volf, xvolfp00
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace ITUCalendar {
    public partial class App : Application {
        protected override void OnStartup(StartupEventArgs e) {
            base.OnStartup(e);

            CalendarModel.LoadFromDisc();

            var okenko = ITUCalendar.Properties.Settings.Default.DefaultView switch {
                "Week" => (Window)new WeekWindow(),
                "Month" => (Window)new MonthWindow(),
                "Day" => (Window)new DayWindow(),
                    _ => (Window)new WeekWindow(),
            };
            okenko.Show();
        }

        protected override void OnExit(ExitEventArgs e) {
            base.OnExit(e);

            CalendarModel.SaveToDisc();
        }
    }
}
