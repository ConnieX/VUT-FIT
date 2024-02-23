using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Ticket
    {
        public int Id { get; set; }
        public int TicketTypeId { get; set; }
        public int FestivalId { get; set; }
        public int ReservationId { get; set; }

        public virtual Reservation Reservation { get; set; }
        public virtual TicketType TicketType { get; set; }
    }
}
