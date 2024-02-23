// Petr Volf, xvolfp00
// Kryštof Škrdlík, xskrdl01
// Monika Rosinská, xrosin00
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;
using System.ComponentModel;

namespace ITUCalendar {
    public class CalendarModel {
        static public List<Calendar> Calendars { get; set; } = new List<Calendar>();
        static public List<Group> Groups { get; set; } = new List<Group>();
        static public List<Event> GetEventsByDate(DateTime day) {
            var list = new List<Event>();
            foreach (var calendar in Calendars){
                var events = calendar.Events.Where(e => e.StartDate.Date.CompareTo(day) >= 0 && e.EndDate.Date.CompareTo(day) <= 0);
                list.AddRange(events);
            }
            return list;
        }

        static public void DeleteEvent(Event e){            foreach (var calendar in Calendars) {
                calendar.Events.Remove(e);
            }
        }

        static public void AddEvent(Event e, Calendar c){
            c.Events.Add(e);
        }

        static public Calendar FindCalendarByEvent(Event ev) {
            foreach (var calendar in Calendars) {
                foreach (var e in calendar.Events) {
                    if (e.Equals(ev)) {
                        return calendar;
                    }
                }
            }
            return null;
        }

        static public void Init() {
            Calendars = new List<Calendar>();
            var calendar = new Calendar() {
                Name = "Svátky",
                IsPrivate = false,
                IsVisible = true,
                Events = new List<Event>()
            };
            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 24, 0, 0, 0),
                EndDate = new DateTime(2019, 12, 24, 23, 59, 59),
                IsEntireDay = true,
                Name = "Štědrý den",
                Color = Color.FromRgb(120, 186, 214)
            });
            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 31, 0, 0, 0),
                EndDate = new DateTime(2019, 12, 31, 23, 59, 59),
                IsEntireDay = true,
                Name = "Silvestr",
                Color = Color.FromRgb(120, 186, 214)
            });
            Calendars.Add(calendar);
            calendar = new Calendar() {
                Name = "Škola",
                IsPrivate = true,
                IsVisible = true,
                Events = new List<Event>()
            };

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 11, 0, 0, 0),
                EndDate = new DateTime(2019, 12, 11, 23, 59, 59),
                IsEntireDay = true,
                Name = "IFJ odevzdání",
                Color = Color.FromRgb(160, 209, 86)
            });
            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 16, 0, 0, 0),
                EndDate = new DateTime(2019, 12, 16, 23, 59, 59),
                IsEntireDay = true,
                Name = "ISS odevzdání",
                Color = Color.FromRgb(160, 209, 86)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 19, 14, 0, 0),
                EndDate = new DateTime(2019, 12, 19, 16, 0, 0),
                IsEntireDay = false,
                Name = "ITU Test",
                Color = Color.FromRgb(160, 209, 86)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 24, 13, 30, 0),
                EndDate = new DateTime(2019, 12, 24, 16, 0, 0),
                IsEntireDay = false,
                Name = "Ježíškovy narozeniny",
                Color = Color.FromRgb(160, 209, 86)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 8, 13, 40, 0),
                EndDate = new DateTime(2019, 12, 8, 16, 0, 0),
                IsEntireDay = false,
                Name = "ITU odevzdávání",
                Color = Color.FromRgb(160, 209, 86)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 8, 11, 20, 0),
                EndDate = new DateTime(2019, 12, 8, 13, 0, 0),
                IsEntireDay = false,
                Name = "Matika učení",
                Color = Color.FromRgb(160, 20, 86)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 8, 8, 20, 0),
                EndDate = new DateTime(2019, 12, 8, 10, 20, 0),
                IsEntireDay = false,
                Name = "Matika učení 2",
                Color = Color.FromRgb(160, 209, 30)
            });

            calendar.Events.Add(new Event() {
                StartDate = new DateTime(2019, 12, 8, 1, 48, 0),
                EndDate = new DateTime(2019, 12, 8, 3, 55, 0),
                IsEntireDay = false,
                Name = "Matika učení 3",
                Color = Color.FromRgb(160, 209, 255)
            });
            Calendars.Add(calendar);
        }

        static public void InitPeople() {
            String[] people = {
                "Rayyan Key",
                "Pawel Maldonado",
                "Ajwa Dillard",
                "Rafael Read",
                "Taslima Baldwin",
                "Bruce Burt",
                "Reginald Clegg",
                "Nico Bell",
            };
            var contacts = new List<Contact>();
            foreach (var contact in people) {
                var splits = contact.Split(' ');
                var email = $"{splits[0]}.{splits[1]}@mail.com";
                contacts.Add(new Contact() {
                    Name = contact,
                    Email = email
                });
            }
            var group1 = new Group() {
                   Name = "Kolegové",
                   People = contacts.GetRange(0, 4),
                   VisibilitySettings = Group.NewVisibilitySettings()
            };

            var group2 = new Group() {
                Name = "Rodina",
                People = contacts.GetRange(4, 4),
                VisibilitySettings = Group.NewVisibilitySettings()
            };

            Groups.Add(group1);
            Groups.Add(group2);
        }

        static public void LoadFromDisc() {
            var appFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            var filename = Path.Combine(appFolder, "ITUCalendar", "data.json");
            if (File.Exists(filename)) {
                Calendars = JsonConvert.DeserializeObject<List<Calendar>>(File.ReadAllText(filename));
                if (Calendars == null || Calendars.Count == 0) {
                    Init();
                }
            } else {
                Init();
            }
            InitPeople();
        }

        static public void SaveToDisc() {
            var str = JsonConvert.SerializeObject(Calendars);
            var appFolder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            var filename = Path.Combine(appFolder, "ITUCalendar", "data.json");
            if (!Directory.Exists(filename)) {
                Directory.CreateDirectory(Directory.GetParent(filename).FullName);
            }
            if (File.Exists(filename)) {
                File.WriteAllText(filename, str);
            } else {
                using (var file = File.Create(filename)) {
                    using (var fw = new StreamWriter(file)) {
                        fw.Write(str);
                        fw.Flush();
                    }
                }
            }
        }
    }

    public class Calendar {
        public String Name { get; set; }
        public List<Event> Events { get; set; }
        public bool IsPrivate { get; set; }
        public bool IsVisible { get; set; }
    }

    public class Event {
        public bool IsEntireDay { get; set; }
        public DateTime StartDate { get; set; }
        public DateTime EndDate { get; set; }
        public String Name { get; set; }
        public String Description { get; set; }
        public Color Color { get; set; }
    }

    public class Contact {
        public String Name { get; set; }
        public String Email { get; set; }
    }

    public class Group {
        public List<Contact> People { get; set; }
        public String Name { get; set; }
        public List<VisibilitySetting> VisibilitySettings { get; set; }

        static public List<VisibilitySetting> NewVisibilitySettings() {
            var list = new List<VisibilitySetting>();

            foreach (var eventType in (EventType[])Enum.GetValues(typeof(EventType))) {
                var vis = new VisibilitySetting() {
                    EventType = eventType,
                    Visibility = CalendarVisibility.Visible,
                    PlaceholderName = null
                };
                list.Add(vis);
            }

            return list;
        }
    }

    public class VisibilitySetting {
        public EventType EventType { get; set; }
        public CalendarVisibility Visibility { get; set; }
        public String PlaceholderName { get; set; }
    }

    public enum EventType {
        School,
        Work,
        Home,
        Other
    }

    public enum CalendarVisibility {
        Visible,
        Hidden,
        DifferentName
    }
}
