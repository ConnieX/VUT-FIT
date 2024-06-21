using Microsoft.EntityFrameworkCore;

namespace API.Data {
    public class ApplicationDbContext : DbContext {
        public DbSet<Category> Categories { get; set; }
        public DbSet<Goods> Goods { get; set; }
        public DbSet<Manufacturer> Manufacturers { get; set; }
        public DbSet<Rating> Ratings { get; set; }

        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
            : base(options) {
        }
    }
}