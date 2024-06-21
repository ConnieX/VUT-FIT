namespace API.Models.Search {
    public class SearchModel {
        public enum ResultType {
            Goods,
            Category,
            Manufacturer
        }

        public int ID { get; }
        public string Name { get; }

        public string Type {
            get => _type.ToString();
        }

        private ResultType _type;

        public SearchModel(int id, string name, ResultType type) {
            ID = id;
            Name = name;
            _type = type;
        }
    }
}