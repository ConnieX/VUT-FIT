namespace API.Models.ManufacturerModels {
    public class ManufacturerList {
        public int ID { get; set; }
        public string Name { get; set; }

        public ManufacturerList() {
        }

        public ManufacturerList(Data.Manufacturer man) {
            ID = man.ID;
            Name = man.Name;
        }
    }
}