using System.ComponentModel.DataAnnotations;
using AutoMapper;

namespace API.Models.ManufacturerModels {
    public class ManufacturerNew {
        [Required] public string Name { get; set; }
        [Required] public string Description { get; set; }
        public string Logo { get; set; }
        [Required] public string Country { get; set; }
    }

    public class ManufacturerNewProfile : Profile {
        public ManufacturerNewProfile() {
            CreateMap<ManufacturerNew, Data.Manufacturer>();
        }
    }
}