using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Reservation
    {
        public Reservation()
        {
            Ticket = new HashSet<Ticket>();
        }

        public string State { get; set; }
        public DateTime ExpirationDate { get; set; }
        public int Id { get; set; }
        public int PersonId { get; set; }

        public virtual Person Person { get; set; }
        public virtual ICollection<Ticket> Ticket { get; set; }
    }
}
