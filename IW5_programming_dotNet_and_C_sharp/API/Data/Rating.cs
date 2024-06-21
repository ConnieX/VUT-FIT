using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace API.Data {
    public class Rating {
        public int ID { get; set; }
        public double StarRating { get; set; }
        public string TextRating { get; set; }
        [JsonIgnore] public Goods Goods { get; set; }
        public int GoodsID { get; set; }
    }
}