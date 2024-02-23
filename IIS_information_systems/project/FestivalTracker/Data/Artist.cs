using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Artist
    {
        public Artist()
        {
            PerformerHasArtist = new HashSet<PerformerHasArtist>();
        }

        public int Id { get; set; }
        public string Name { get; set; }
        public string Surname { get; set; }

        public virtual ICollection<PerformerHasArtist> PerformerHasArtist { get; set; }
    }
}
