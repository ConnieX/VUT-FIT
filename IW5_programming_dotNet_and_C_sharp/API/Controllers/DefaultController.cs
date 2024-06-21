using Microsoft.AspNetCore.Mvc;
using NSwag.Annotations;

namespace API.Controllers {
    [Route("/")]
    [OpenApiIgnore]
    public class DefaultController : ControllerBase {
        [HttpGet]
        public IActionResult Redirect() {
            return new RedirectResult("/swagger/index.html");
        }
    }
}