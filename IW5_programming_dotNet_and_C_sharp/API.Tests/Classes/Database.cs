using System;
using System.Collections.Generic;
using System.Linq;
using API.Data;
using Bogus;
using FizzWare.NBuilder;

namespace API.Tests.Classes {
    public static class Database {
        public static ApplicationDbContext Data { get; private set; }

        private static Faker _faker;

        private static Random _random;


        public static void Seed(ApplicationDbContext db) {
            db.Ratings.RemoveRange(db.Ratings);
            db.Goods.RemoveRange(db.Goods);
            db.Categories.RemoveRange(db.Categories);
            db.Manufacturers.RemoveRange(db.Manufacturers);

            _faker = new Faker("cz");
            _random = new Random();
            Data = db;
            var categories = CreateCategories();
            var manufacturers = CreateManufacturer();
            var goods = CreateGoods(manufacturers, categories);
            CreateRatings(goods);
        }


        private static List<Category> CreateCategories() {
            var categories = _faker.Commerce.Categories(20);
            var result = categories.Select(item => new Category() {Name = item}).ToList();
            Data.Categories.AddRange(result);
            Data.SaveChanges();
            return result;
        }

        private static List<Manufacturer> CreateManufacturer() {
            var manufacturers = Builder<Manufacturer>.CreateListOfSize(15).All()
                .With(x => x.Country = _faker.Address.Country())
                .With(x => x.Name = _faker.Company.CompanyName())
                .With(x => x.Logo = _faker.Image.PlaceholderUrl(100, 100))
                .With(x => x.Description = _faker.Lorem.Sentences())
                .Build();
            Data.Manufacturers.AddRange(manufacturers);
            Data.SaveChanges();
            return manufacturers.ToList();
        }

        private static List<Goods> CreateGoods(List<Manufacturer> man, List<Category> cat) {
            var goods = Builder<Goods>.CreateListOfSize(50).All()
                .With(x => x.CategoryID = cat[_random.Next(0, cat.Count - 1)].ID)
                .With(x => x.ManufacturerID = man[_random.Next(0, man.Count - 1)].ID)
                .With(x => x.Count = _random.Next(0, 50))
                .With(x => x.Weight = _random.NextDouble() * 30)
                .With(x => x.Price = double.Parse(_faker.Commerce.Price(0, 500)))
                .With(x => x.Description = _faker.Lorem.Sentences())
                .With(x => x.Name = _faker.Commerce.ProductName())
                .With(x => x.Photo = _faker.Image.LoremFlickrUrl())
                .Build();
            Data.Goods.AddRange(goods);
            Data.SaveChanges();
            return goods.ToList();
        }

        private static void CreateRatings(List<Goods> goods) {
            var reviews = Builder<Rating>.CreateListOfSize(500).All()
                .With(x => x.GoodsID = goods[_random.Next(0, goods.Count - 1)].ID)
                .With(x => x.StarRating = _random.NextDouble() * 5)
                .With(x => x.TextRating = _faker.Rant.Review())
                .Build();
            Data.Ratings.AddRange(reviews);
            Data.SaveChanges();
        }
    }
}