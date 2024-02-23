using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Concert
    {
        public int FestivalId { get; set; }
        public int PerformerId { get; set; }
        public DateTime Start { get; set; }
        public DateTime End { get; set; }
        public int? Number { get; set; }

        public virtual Festival Festival { get; set; }
        public virtual Performer Performer { get; set; }
    }
}
