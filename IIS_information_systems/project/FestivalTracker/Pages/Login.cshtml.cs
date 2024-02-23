using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics.CodeAnalysis;
using System.Security.Claims;
using System.Threading.Tasks;
using FestivalTracker.Data;
using FestivalTracker.Services;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace FestivalTracker {
    [SuppressMessage("ReSharper", "InconsistentNaming")]
    public class LoginModel {
        public string email { get; set; }
        public string password { get; set; }

        public string redirect { get; set; }
    }
}

namespace FestivalTracker.Pages {

    [IgnoreAntiforgeryToken]
    public class Login : PageModel {
        private UserManager _manager;

        public Login(UserManager manager) {
            _manager = manager;
        }


        public async Task<IActionResult> OnPostAsync([FromBody] LoginModel login) {
            if (login == null) {
                return BadRequest();
            }
            try {
                // Clear the existing external cookie
                await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
            }
            catch (Exception) {
            }
            if (!_manager.CheckLogin(login.email, login.password, out var user)) {
                return Unauthorized();
            }
            var claims = new List<Claim> {
                new Claim (ClaimTypes.Email, user.Person.Email),
                new Claim (ClaimTypes.Role, user.Role.ToString()),
                new Claim (ClaimTypes.Name, user.Person.Name ?? ""),
                new Claim (ClaimTypes.Surname, user.Person.Surname ?? ""),
                new Claim (ClaimTypes.NameIdentifier, user.Person.Id.ToString())
            };
            var claimsIdentity = new ClaimsIdentity(
                claims, CookieAuthenticationDefaults.AuthenticationScheme);
            var authProperties = new AuthenticationProperties {
                IsPersistent = true,
                RedirectUri = Request.Host.Value
            };
            try {
                await HttpContext.SignInAsync(
                    CookieAuthenticationDefaults.AuthenticationScheme,
                    new ClaimsPrincipal(claimsIdentity),
                    authProperties);
            }
            catch (Exception ex) {
                var error = ex.Message;
            }
            var returnUrl = Url.Content("~/" + login.redirect);
            return new RedirectResult(returnUrl);
        }

        public IActionResult OnGet() {
            return BadRequest();
        }
    }
}