using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace API.Data {
    public class Manufacturer {
        public int ID { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string Logo { get; set; }
        public string Country { get; set; }
        [JsonIgnore] public List<Goods> Goods { get; set; }
    }
}