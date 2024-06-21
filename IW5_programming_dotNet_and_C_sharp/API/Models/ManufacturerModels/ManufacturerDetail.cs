using System.Collections.Generic;
using System.Linq;
using API.Data;
using API.Models.GoodsModels;
using AutoMapper;

namespace API.Models.ManufacturerModels {
    public class ManufacturerDetail {
        public int ID { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string Logo { get; set; }
        public string Country { get; set; }
        public List<int> Goods { get; set; }
    }

    public class ManufacturerDetailProfile : Profile {
        public ManufacturerDetailProfile() {
            CreateMap<Manufacturer, ManufacturerDetail>().ForMember(x => x.Goods,
                expression => expression.MapFrom(x => x.Goods.Select(y => y.ID)));
        }
    }
}