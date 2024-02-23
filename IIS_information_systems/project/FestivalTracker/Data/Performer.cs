using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class Performer
    {
        public Performer()
        {
            Concert = new HashSet<Concert>();
            PerformerHasArtist = new HashSet<PerformerHasArtist>();
        }

        public int Id { get; set; }
        public string Name { get; set; }
        public string Logo { get; set; }
        public string Genre { get; set; }
        public double? Rating { get; set; }

        public virtual ICollection<Concert> Concert { get; set; }
        public virtual ICollection<PerformerHasArtist> PerformerHasArtist { get; set; }
    }
}
