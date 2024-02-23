using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class PerformerHasArtist
    {
        public int PerformerId { get; set; }
        public int ArtistId { get; set; }

        public virtual Artist Artist { get; set; }
        public virtual Performer Performer { get; set; }
    }
}
