using System;
using System.Collections.Generic;
using System.Device.Location;

namespace FestivalTracker.Data {
    public partial class Festival {
        public Festival() {
            Concert = new HashSet<Concert>();
            TicketType = new HashSet<TicketType>();
        }

        public int Id { get; set; }
        public DateTime StartDate { get; set; }
        public string Name { get; set; }
        public DateTime EndDate { get; set; }
        public string Place { get; set; }
        public string Genre { get; set; }
        public int Capacity { get; set; }
        public string Details { get; set; }
        public int CreatorId { get; set; }
        public string Geolocation { get; set; }

        public GeoCoordinate GetCoordinate() {
            var parts = Geolocation.Split(", ");
            return new GeoCoordinate(double.Parse(parts[0]), double.Parse(parts[1]));
        }

        public virtual User Creator { get; set; }
        public virtual ICollection<Concert> Concert { get; set; }
        public virtual ICollection<TicketType> TicketType { get; set; }
    }
}