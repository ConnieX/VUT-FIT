using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace API.Data {
    public class Category {
        public int ID { get; set; }
        public string Name { get; set; }
        [JsonIgnore] public List<Goods> Goods { get; set; }
    }
}