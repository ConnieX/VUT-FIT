using System.ComponentModel.DataAnnotations;
using API.Data;

namespace API.Validators {
    public class ManufacturerNewValidator : ValidationAttribute {
        protected override ValidationResult IsValid(object value, ValidationContext validationContext) {
            var context = (ApplicationDbContext) validationContext.GetService(typeof(ApplicationDbContext));
            if (context == null) {
                return new ValidationResult("Unknown err");
            }

            if (context.Manufacturers.Find((int) value) == null) {
                return new ValidationResult("Manufacturer does not exist");
            }

            return ValidationResult.Success;
        }
    }


    public class ManufacturerUpdateValidator : ValidationAttribute {
        protected override ValidationResult IsValid(object value, ValidationContext validationContext) {
            var context = (ApplicationDbContext) validationContext.GetService(typeof(ApplicationDbContext));
            if (context == null) {
                return new ValidationResult("Unknown err");
            }

            if ((int) value != 0 && context.Manufacturers.Find((int) value) == null) {
                return new ValidationResult("Manufacturer does not exist");
            }

            return ValidationResult.Success;
        }
    }
}