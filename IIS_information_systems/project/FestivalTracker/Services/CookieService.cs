using System;
using System.Threading.Tasks;
using Microsoft.JSInterop;
using Microsoft.Net.Http.Headers;

namespace FestivalTracker.Services {
    public class CookieService {

        private IJSRuntime _js;

        public CookieService(IJSRuntime js) {
            _js = js;
        }


        public async Task SetCookie(string name, string value, TimeSpan expiresIn) {
            await _js.InvokeVoidAsync("setCookie", name, value, expiresIn.Milliseconds);
        }

        public async Task<string> GetCookie(string name) {
            return await _js.InvokeAsync<string>("getCookie", name);
        }

        public async Task DeleteCookie(string name) {
            await _js.InvokeVoidAsync("deleteCookie", name);
        }

    }
}