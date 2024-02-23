using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Security.Policy;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using System.Web;
using System.Device.Location;
using System.Diagnostics;
using System.Linq;
using System.Net;
using FestivalTracker.Data;
using Microsoft.JSInterop;


namespace FestivalTracker.Services {
    public class GeoService {
        private HttpClient _client;
        private IJSRuntime _js;
        
        public GeoService(IJSRuntime runtime) {
            _js = runtime;
            _client = new HttpClient();
        }


        private Action<GeoCoordinate> _callback;
        
        public GeoCoordinate Default => new GeoCoordinate(49.1922443, 16.6113382);

        [JSInvokable]
        public void GetCoords(string coordinates) {
            if (string.IsNullOrEmpty(coordinates)) {
                _callback.Invoke(Default);
                return;
            }

            var items = coordinates.Split(", ");
            var coord = new GeoCoordinate {Latitude = double.Parse(items[0]), Longitude = double.Parse(items[1])};
            _callback.Invoke(coord);
            Debug.Write(coordinates);
        }
        

        public async void GetUserLocation(Action<GeoCoordinate> onFinished) {
            var dotNetObjRef = DotNetObjectReference.Create(this);
            var res = await _js.InvokeAsync<string>("getLocation", dotNetObjRef);
            _callback = onFinished;
        }
        
        public GeoCoordinate GetLocation(string location) {
            try {
                location = HttpUtility.UrlEncode(location);
                var url =
                    "https://us1.locationiq.com/v1/search.php?key=pk.5e69eae186174b24fc4af789a4d72913&q="+location+"&countrycodes=cz&format=json";
                var responseMessage = _client.GetAsync(url).Result;
                if (responseMessage.StatusCode != HttpStatusCode.OK) {
                    return Default;
                }
                var res = responseMessage.Content.ReadAsStringAsync().Result;
            
                var options = new JsonSerializerOptions {PropertyNameCaseInsensitive = true};
                options.Converters.Add(new JsonStringEnumConverter());

                var loc = JsonSerializer.Deserialize<List<Geolocation>>(res, options);
                if (loc.Count == 0) {
                    return Default;
                }
                var first = loc.OrderByDescending(x => x.Importance).First();
                return new GeoCoordinate(double.Parse(first.Lat),double.Parse(first.Lon));
                
            }
            catch (Exception e) {
                return GeoCoordinate.Unknown;
            }
           
        }
        
        
    }
}