namespace API.Models.GoodsModels {
    public class GoodsList {
        public int ID { get; set; }
        public string Name { get; set; }

        public GoodsList() {
        }

        public GoodsList(Data.Goods goods) {
            ID = goods.ID;
            Name = goods.Name;
        }
    }
}