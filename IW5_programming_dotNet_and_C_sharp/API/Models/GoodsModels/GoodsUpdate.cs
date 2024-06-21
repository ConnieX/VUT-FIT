using System.ComponentModel.DataAnnotations;
using API.Validators;
using AutoMapper;

namespace API.Models.GoodsModels {
    public class GoodsUpdate {
        public string Name { get; set; }
        public string Photo { get; set; }
        public string Description { get; set; }
        [Range(0, 10_000_000)] public double? Price { get; set; }
        [Range(0, 10_000_000)] public double? Weight { get; set; }
        [Range(0, 100)] public int? Count { get; set; }
        [CategoryUpdateValidator] public int CategoryID { get; set; }
        [ManufacturerUpdateValidator] public int ManufacturerID { get; set; }
    }

    public class GoodsUpdateProfile : Profile {
        public GoodsUpdateProfile() {
            CreateMap<GoodsUpdate, Data.Goods>();
        }
    }
}