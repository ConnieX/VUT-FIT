using AutoMapper;

namespace API.Models.ManufacturerModels {
    public class ManufacturerUpdate {
        public string Name { get; set; }
        public string Description { get; set; }
        public string Logo { get; set; }
        public string Country { get; set; }
    }

    public class ManufacturerUpdateProfile : Profile {
        public ManufacturerUpdateProfile() {
            CreateMap<ManufacturerUpdate, Data.Manufacturer>();
        }
    }
}