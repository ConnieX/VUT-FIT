using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class TicketType
    {
        public TicketType()
        {
            Ticket = new HashSet<Ticket>();
        }

        public int Id { get; set; }
        public int FestivalId { get; set; }
        public string Name { get; set; }
        public DateTime ValidFrom { get; set; }
        public DateTime ValidTo { get; set; }
        public int Count { get; set; }
        public double Price { get; set; }

        public virtual Festival Festival { get; set; }
        public virtual ICollection<Ticket> Ticket { get; set; }
    }
}
