using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Person
    {
        public Person()
        {
            Reservation = new HashSet<Reservation>();
        }

        public int Id { get; set; }
        public string Name { get; set; }
        public string Surname { get; set; }
        public string Email { get; set; }
        public DateTime? Birth { get; set; }

        public virtual User User { get; set; }
        public virtual ICollection<Reservation> Reservation { get; set; }
    }
}
