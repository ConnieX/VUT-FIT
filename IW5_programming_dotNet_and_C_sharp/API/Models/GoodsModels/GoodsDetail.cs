using System.Collections.Generic;
using System.Linq;
using API.Data;
using AutoMapper;

namespace API.Models.GoodsModels {
    public class GoodsDetail {
        public int ID { get; set; }
        public string Name { get; set; }
        public string Photo { get; set; }
        public string Description { get; set; }
        public double Price { get; set; }
        public double Weight { get; set; }
        public int Count { get; set; }
        public Category Category { get; set; }
        public int ManufacturerID { get; set; }
        public List<int> Ratings { get; set; }
    }

    public class GoodsDetailProfile : Profile {
        public GoodsDetailProfile() {
            CreateMap<Goods, GoodsDetail>().ForMember(x => x.Ratings,
                expression => expression.MapFrom(x => x.Ratings.Select(y => y.ID)));
        }
    }
}