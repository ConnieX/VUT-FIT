using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using API.Data;
using API.Models.GoodsModels;
using API.Models.ManufacturerModels;
using API.Models.RatingModels;
using API.Tests.Classes;
using FluentAssertions;
using Microsoft.Extensions.DependencyInjection;
using Newtonsoft.Json;
using Xunit;

namespace API.Tests {
    public class GetAllTests : TestBase {
        public GetAllTests(ServerFactory<Startup> fixture) : base(fixture) {
        }

        [Theory]
        [InlineData("/api/Goods", 0)]
        [InlineData("/api/Manufacturer", 1)]
        [InlineData("/api/Rating", 2)]
        public async Task GetAll(string endpoint, int type) {
            var response = await Client.GetAsync(endpoint);
            response.StatusCode.Should().Be(HttpStatusCode.OK);

            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var responseStream = await response.Content.ReadAsStringAsync();
            switch (type) {
                case 0:
                    await CompareAllGoods(responseStream, context);
                    break;
                case 1:
                    await CompareAllManufacturers(responseStream, context);
                    break;
                case 2:
                    await CompareAllRatings(responseStream, context);
                    break;
            }
        }


        [Theory]
        [InlineData("/api/Goods/1", 0)]
        [InlineData("/api/Manufacturer/1", 1)]
        [InlineData("/api/Rating/1", 2)]
        public async Task GetSpecific_Ok(string endpoint, int type) {
            var response = await Client.GetAsync(endpoint);
            response.StatusCode.Should().Be(HttpStatusCode.OK);
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();
            var responseStream = await response.Content.ReadAsStringAsync();
            switch (type) {
                case 0:
                    await CompareGoods(responseStream, context);
                    break;
                case 1:
                    await CompareManufacturer(responseStream, context);
                    break;
                case 2:
                    await CompareRatings(responseStream, context);
                    break;
            }
        }

        [Theory]
        [InlineData("/api/Goods/")]
        [InlineData("/api/Manufacturer/")]
        [InlineData("/api/Rating/")]
        public async Task GetSpecific_NotFound(string endpoint) {
            var response = await Client.GetAsync(endpoint + "0");
            response.StatusCode.Should().Be(HttpStatusCode.NotFound);
            response = await Client.GetAsync(endpoint + int.MaxValue);
            response.StatusCode.Should().Be(HttpStatusCode.NotFound);
            response = await Client.GetAsync(endpoint + int.MinValue);
            response.StatusCode.Should().Be(HttpStatusCode.NotFound);
        }

        [Theory]
        [InlineData("/api/Goods/")]
        [InlineData("/api/Manufacturer/")]
        [InlineData("/api/Rating/")]
        public async Task GetSpecific_BadRequest(string endpoint) {
            var response = await Client.GetAsync(endpoint + "wefiuhwaefiouhawerfiuh");
            response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
            response = await Client.GetAsync(endpoint + double.MaxValue);
            response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
            response = await Client.GetAsync(endpoint + double.MinValue);
            response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
            response = await Client.GetAsync(endpoint + "3.14");
            response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
        }

        private async Task CompareGoods(string content, ApplicationDbContext context) {
            var goods = JsonConvert.DeserializeObject<GoodsDetail>(content);
            (await context.Goods.FindAsync(goods.ID)).Should().NotBeNull();
            (await context.Manufacturers.FindAsync(goods.ManufacturerID)).Should().NotBeNull();
            foreach (var rating in goods.Ratings) {
                (await context.Ratings.FindAsync(rating)).Should().NotBeNull();
            }
        }

        private async Task CompareManufacturer(string content, ApplicationDbContext context) {
            var manufacturer = JsonConvert.DeserializeObject<ManufacturerDetail>(content);
            (await context.Manufacturers.FindAsync(manufacturer.ID)).Should().NotBeNull();
            foreach (var good in manufacturer.Goods) {
                (await context.Goods.FindAsync(good)).Should().NotBeNull();
            }
        }

        private async Task CompareRatings(string content, ApplicationDbContext context) {
            var ratings = JsonConvert.DeserializeObject<Rating>(content);
            (await context.Ratings.FindAsync(ratings.ID)).Should().NotBeNull();
            (await context.Goods.FindAsync(ratings.GoodsID)).Should().NotBeNull();
        }

        private async Task CompareAllGoods(string content, ApplicationDbContext context) {
            var goods = JsonConvert.DeserializeObject<List<GoodsList>>(content);
            goods.Count.Should().Be(context.Goods.Count());
            foreach (var item in goods) {
                item.Name.Should().Be((await context.Goods.FindAsync(item.ID)).Name);
            }
        }

        private async Task CompareAllManufacturers(string content, ApplicationDbContext context) {
            var manufacturer = JsonConvert.DeserializeObject<List<ManufacturerList>>(content);
            manufacturer.Count.Should().Be(context.Manufacturers.Count());
            foreach (var item in manufacturer) {
                item.Name.Should().Be((await context.Manufacturers.FindAsync(item.ID)).Name);
            }
        }

        private async Task CompareAllRatings(string content, ApplicationDbContext context) {
            var ratings = JsonConvert.DeserializeObject<List<RatingList>>(content);
            ratings.Count.Should().Be(context.Ratings.Count());
            foreach (var item in ratings) {
                item.Stars.Should().Be((await context.Ratings.FindAsync(item.ID)).StarRating);
            }
        }
    }
}