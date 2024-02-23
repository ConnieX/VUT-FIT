// Autor xskrdl01

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

namespace ITUCalendar
{
    /// <summary>
    /// Interaction logic for DayWindow.xaml
    /// </summary>
    public partial class DayWindow : Window
    {
        DateTime date;
        public DayWindow()
        {
            Left = Properties.Settings.Default.WindowX; // souřadnice vykreslení okna
            Top = Properties.Settings.Default.WindowY;
            date = DateTime.Now;    // vezme aktuální datum


            InitializeComponent();
            GenerateGridRows(true); // vygeneruje grid
            AddContent(date);       // přidá do něj eventy
        }

        // Vygeneruje jednotlivé řádky pro hodiny //
        private void GenerateGridRows(bool first)
        {
            // generuje řádky pro každou půl hodinu od času 00:00
            for (int j = 0; j < 48; j++)
            {
                if (first)  // pokud se volá v GenerateGridRows poprvé
                {
                    Days.RowDefinitions.Add(new RowDefinition());
                }

                TextBlock time = new TextBlock();

                if (j % 2 == 0) // pro každý druhý řádek se připíše celá hodina
                {
                    double i = j / 2;
                    time.Text = i.ToString();
                    time.HorizontalAlignment = HorizontalAlignment.Center;
                    time.VerticalAlignment = VerticalAlignment.Top;
                    time.Margin = new Thickness(0, 0, 3, 0);
                }
                Grid.SetRow(time, j);   // přiřadí jednotlivé řádky do gridu
                Days.Children.Add(time);
                time.Foreground = Brushes.White;

                if (j % 2 == 0) // pro každou celou hodinu se vytvoří rámeček a nastaví se akce mouseenter mouseleave
                {
                    Border border = new Border();
                    Grid.SetColumnSpan(border, 2);
                    Grid.SetRow(border, j);
                    border.BorderThickness = new Thickness(0, 1, 0, 0);
                    border.BorderBrush = Brushes.Gray;
                    border.Height = 20;
                    Days.Children.Add(border);
                    border.MouseEnter += (s, ev) => border.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#70888888"));
                    border.MouseLeave += (s, ev) => border.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#1000"));
                    border.MouseDown += (s, ev) => NewEvent(s, ev);
                }
            }

            Border border1 = new Border();  // přidá se vertikální rámeček ohraničující časy hodin od událostí

            border1.BorderThickness = new Thickness(0, 0, 1, 0);
            border1.BorderBrush = Brushes.White;

            Grid.SetColumn(border1, 0);
            Grid.SetRowSpan(border1, 48);
            Days.Children.Add(border1);
        }
        // ____________________________________ //

        // Vloží všechny události do daného dne //
        private void AddContent(DateTime date)
        {
            DayName.Content = date.ToString("dddd d. MMMM ", new CultureInfo("cs-CZ")); // vepíše datum zobrazovaného dne do tlačítka DayName

            Days.Children.Clear();

            GenerateGridRows(false); // vygeneruje řádky

            date = date.Date; // převede datum do správné formy pro zavolání getEventsByDate

            var dayEvent = CalendarModel.GetEventsByDate(date);

            foreach (var ev in dayEvent)    // projde všechny eventy
            {
                Button newEvent = new Button(); // každy event vloží jako tlačítko s parametry eventu
                newEvent.Content = ev.Name;
                newEvent.Background = new SolidColorBrush(ev.Color);
                newEvent.FontFamily = (FontFamily)FindResource("Overpass");
                newEvent.Click += (object sender, RoutedEventArgs e) => {   // při kliknutí na event se otevře EventWindow
                    var showEvent = new EventWindow(
                            new Tuple<Event, Calendar>(ev, CalendarModel.FindCalendarByEvent(ev))
                        );
                    showEvent.Show();
                };
                if (ev.IsEntireDay == true) // pokud je event celodenní, zabere vrchní 2 řádky
                {
                    Grid.SetRowSpan(newEvent, 2);
                    Grid.SetRow(newEvent, 0);
                }
                else    // pokud event není celodenní, zarovná se do grid systému
                {
                    int startDiff = 0;  // rozdíl počátku posune počátek v gridu o 0 pokud event začíná mezi HH:00 a HH:15
                    if (ev.StartDate.Minute >= 15 && ev.StartDate.Minute <= 45)
                        startDiff = 1;  // o 1 pokud event začíná mezi HH:15 a HH:45
                    if (ev.StartDate.Minute > 45 && ev.StartDate.Minute <= 60)
                        startDiff = 2;  // o 2 pokud event začíná po HH:45

                    int endDiff = 0;                // rozdíl konce posune konec v gridu
                    if (ev.EndDate.Minute >= 0 && ev.EndDate.Minute < 15)
                        endDiff = 0 - startDiff;    // o -startDiff pokud event končí před HH:15
                    if (ev.EndDate.Minute >= 15 && ev.EndDate.Minute <= 45)
                        endDiff = 1 - startDiff;    // o 1-startDiff pokud event končí mezi HH:15 a HH:45
                    if (ev.EndDate.Minute > 45 && ev.EndDate.Minute <= 60)
                        endDiff = 2 - startDiff;    // o 2-startDiff pokud event končí po HH:45

                    Grid.SetRowSpan(newEvent, (ev.EndDate.Hour - ev.StartDate.Hour) * 2 + endDiff);
                    Grid.SetRow(newEvent, (ev.StartDate.Hour) * 2 + startDiff); // vybere pro event část vygenerovaného gridu
                }
                Grid.SetColumn(newEvent, 1);
                Days.Children.Add(newEvent);    // přidá event 
            }
        }
        //_______________________________________//

        // zapamatuje si poslední nastavení zobrazení
        private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Properties.Settings.Default.DefaultView = "Day";
            Properties.Settings.Default.WindowX = Left;
            Properties.Settings.Default.WindowY = Top;
            Properties.Settings.Default.Save();
        }

        // zavře zobrazení
        private void CloseWindow(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        // přidá do kalendáře nový event
        private void NewEvent(object sender, RoutedEventArgs e)
        {
            var ev = new Event()
            {
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

        //  otevře nastavení
        private void OpenSettings(object sender, RoutedEventArgs e)
        {
            var settings = new Settings();
            settings.Show();
        }

        //  přepne na měsíční pohled
        private void SwitchToMonth(object sender, RoutedEventArgs e)
        {
            var settings = new Settings();

            settings.MonthButton.RaiseEvent(new RoutedEventArgs(Button.ClickEvent));
            this.Close();
        }

        //  posune aktuální den doprava
        private void MoveDayRight(object sender, RoutedEventArgs e)
        {
            date = date.AddDays(1);
            AddContent(date);
        }

        //  posune aktuální den doleva
        private void MoveDayLeft(object sender, RoutedEventArgs e)
        {
            date = date.AddDays(-1);
            AddContent(date);
        }
    }
}