using Microsoft.AspNetCore.Authorization;

namespace FestivalTracker {
    public class PageAuthReq : IAuthorizationRequirement {

        public int AuthLevel { get; }

        public PageAuthReq(int level) {
            AuthLevel = level;
        }
    }
}