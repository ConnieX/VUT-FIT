using System.ComponentModel.DataAnnotations;
using API.Data;

namespace API.Validators {
    public class GoodsNewValidator : ValidationAttribute {
        protected override ValidationResult IsValid(object value, ValidationContext validationContext) {
            var context = (ApplicationDbContext) validationContext.GetService(typeof(ApplicationDbContext));
            if (context == null) {
                return new ValidationResult("Unknown err");
            }

            if (context.Goods.Find((int) value) == null) {
                return new ValidationResult("Good does not exist");
            }

            return ValidationResult.Success;
        }
    }

    public class GoodsUpdateValidator : ValidationAttribute {
        protected override ValidationResult IsValid(object value, ValidationContext validationContext) {
            var context = (ApplicationDbContext) validationContext.GetService(typeof(ApplicationDbContext));
            if (context == null) {
                return new ValidationResult("Unknown err");
            }

            if ((int) value != 0 && context.Goods.Find((int) value) == null) {
                return new ValidationResult("Good does not exist");
            }

            return ValidationResult.Success;
        }
    }
}