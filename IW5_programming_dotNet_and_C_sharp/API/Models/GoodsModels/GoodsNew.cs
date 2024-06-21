using System.ComponentModel.DataAnnotations;
using API.Validators;
using AutoMapper;

namespace API.Models.GoodsModels {
    public class GoodsNew {
        [Required] public string Name { get; set; }

        public string Photo { get; set; }
        [Required] public string Description { get; set; }

        [Required] [Range(1, 10_000_000)] public double? Price { get; set; }

        [Required] [Range(1, 10_000_000)] public double? Weight { get; set; }

        [Required] [Range(0, 10_000_000)] public int? Count { get; set; }
        [Required] [CategoryNewValidator] public int CategoryID { get; set; }
        [Required] [ManufacturerNewValidator] public int ManufacturerID { get; set; }
    }

    public class GoodsNewProfile : Profile {
        public GoodsNewProfile() {
            CreateMap<GoodsNew, Data.Goods>();
        }
    }
}