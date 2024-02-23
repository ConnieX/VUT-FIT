using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Claims;
using System.Threading.Tasks;
using FestivalTracker.Data;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Components.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Http.Extensions;
using Microsoft.EntityFrameworkCore;

namespace FestivalTracker.Services {
    public class PagePermissionHandler : IAuthorizationHandler {
        private IHttpContextAccessor _accesor;
        private DatabaseContext _database;
        
        public PagePermissionHandler(IHttpContextAccessor httpContext, DatabaseContext database) {
            _database = database;
            _accesor = httpContext;
        }
        
        public Task HandleAsync(AuthorizationHandlerContext context) {
            var pendingRequirements = context.PendingRequirements.ToList();
            var claims = context.User.Claims;
            
            foreach (var requirement in pendingRequirements) {
                var result = requirement switch {
                    PageAuthReq pageAuth => IsRoleAuthorized(pageAuth, claims),
                    FestivalAuth festivalAuth => IsFestivalAuth(festivalAuth, claims),
                    _ => null
                };

                if (!result.HasValue) continue;
                if (result.Value) {
                    context.Succeed(requirement);
                }
                else {
                    context.Fail();
                }


            }
            return Task.CompletedTask;
        }

        private bool? IsFestivalAuth(FestivalAuth requirement, IEnumerable<Claim> claims) {
            var claim = claims.FirstOrDefault(x => x.Type == ClaimTypes.NameIdentifier);
            var role = claims.FirstOrDefault(x => x.Type == ClaimTypes.Role);
            if (int.Parse(role.Value) == 3) {
                return true;
            }

            if (claim == default) {
                return false;
            }
            var id = int.Parse(claim.Value);
            var festivalId = _accesor.HttpContext.Request.Path.Value.Split("/").Last();
            if (!int.TryParse(festivalId, out var festId)) {
                return true;    
            }
            var festival = _database.Festival.Find(festId);
            if (festival == default) {
                return false;
            }
            return festival.CreatorId == id;
        }

        private bool IsRoleAuthorized(PageAuthReq requirement, IEnumerable<Claim> claims) {
            var claim = claims.FirstOrDefault(x => x.Type == ClaimTypes.Role);
            if (claim == default) {
                return false;
            }

            var value = int.Parse(claim.Value);
            if (requirement.AuthLevel > value) {
                return false;
            }
            return true;
        }
    }
}