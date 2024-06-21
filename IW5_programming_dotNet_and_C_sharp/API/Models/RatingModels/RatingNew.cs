using System.ComponentModel.DataAnnotations;
using API.Validators;
using AutoMapper;

namespace API.Models.RatingModels {
    public class RatingNew {
        [Required] [Range(0, 5.0)] public double? StarRating { get; set; }
        [Required] public string TextRating { get; set; }
        [Required] [GoodsNewValidator] public int GoodsID { get; set; }
    }

    public class RatingNewProfile : Profile {
        public RatingNewProfile() {
            CreateMap<RatingNew, Data.Rating>();
        }
    }
}