using System;
using System.Collections.Generic;

namespace FestivalTracker.Data
{
    public partial class User
    {
        public User()
        {
            Festival = new HashSet<Festival>();
        }

        public int PersonId { get; set; }
        public string Password { get; set; }
        public int Role { get; set; }
        public string ProfilePic { get; set; }
        public int? PhoneNumber { get; set; }
        public string Address { get; set; }

        public virtual Person Person { get; set; }
        public virtual ICollection<Festival> Festival { get; set; }
    }
}
