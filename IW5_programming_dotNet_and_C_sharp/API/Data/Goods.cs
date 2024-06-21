using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace API.Data {
    public class Goods {
        public int ID { get; set; }
        public string Name { get; set; }
        public string Photo { get; set; }
        public string Description { get; set; }
        public double Price { get; set; }
        public double Weight { get; set; }
        public int Count { get; set; }
        public Category Category { get; set; }
        public int CategoryID { get; set; }
        public Manufacturer Manufacturer { get; set; }
        public int ManufacturerID { get; set; }
        public List<Rating> Ratings { get; set; }
    }
}