using System.ComponentModel.DataAnnotations;
using API.Validators;
using AutoMapper;

namespace API.Models.RatingModels {
    public class RatingUpdate {
        [Range(0, 5.0)] public double? StarRating { get; set; }
        public string TextRating { get; set; }
        [GoodsUpdateValidator] public int GoodsID { get; set; }
    }

    public class RatingUpdateProfile : Profile {
        public RatingUpdateProfile() {
            CreateMap<RatingUpdate, Data.Rating>();
        }
    }
}