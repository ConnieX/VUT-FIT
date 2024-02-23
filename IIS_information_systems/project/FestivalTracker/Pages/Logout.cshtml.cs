using System;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;

namespace FestivalTracker.Pages {
    public class Logout : PageModel {
        public async Task<IActionResult> OnGetAsync(string redirect) {
            var returnUrl = Url.Content("~/" + redirect);
            try {
                await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
            }
            catch (Exception) {
            }
            return LocalRedirect(returnUrl);
        }
    }
}