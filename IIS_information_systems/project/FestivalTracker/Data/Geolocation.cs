using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace FestivalTracker.Data
{
    public class Geolocation {
        public string Lat { get; set; } 
        public string Lon { get; set; } 
        [JsonPropertyName("display_name")]
        public string DisplayName { get; set; }
        public double Importance { get; set; }
    }
}