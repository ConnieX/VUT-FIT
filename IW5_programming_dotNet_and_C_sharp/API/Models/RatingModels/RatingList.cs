namespace API.Models.RatingModels {
    public class RatingList {
        public int ID { get; set; }
        public double Stars { get; set; }
        public int GoodsID { get; set; }

        public RatingList() {
        }

        public RatingList(Data.Rating rating) {
            ID = rating.ID;
            Stars = rating.StarRating;
            GoodsID = rating.GoodsID;
        }
    }
}