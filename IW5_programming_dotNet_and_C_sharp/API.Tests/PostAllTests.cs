using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using API.Data;
using API.Models.GoodsModels;
using API.Models.ManufacturerModels;
using API.Models.RatingModels;
using API.Tests.Classes;
using FluentAssertions;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.DependencyInjection;
using Xunit;

namespace API.Tests {
    public class PostAllTests : TestBase {
        public PostAllTests(ServerFactory<Startup> fixture) : base(fixture) {
        }

        [Fact]
        public async Task PostTests_Ok() {
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var dict = Utilities.CreateDictFromObject<ManufacturerNew>();
            var content = new StringContent(JsonSerializer.Serialize(dict), Encoding.UTF8, "application/json");
            var result = await Client.PostAsync("/api/manufacturer", content);
            if (result.StatusCode != HttpStatusCode.Created) {
                var response = result.Content.ReadAsStringAsync().Result;
            }

            result.StatusCode.Should().Be(HttpStatusCode.Created);

            var id = int.Parse(result.Headers.Location.OriginalString.Split("/").Last());
            dict = Utilities.CreateDictFromObject<GoodsNew>();
            dict["ManufacturerID"] = id;
            dict["CategoryID"] = context.Categories.First().ID;
            content = new StringContent(JsonSerializer.Serialize(dict), Encoding.UTF8, "application/json");
            result = await Client.PostAsync("/api/goods", content);
            if (result.StatusCode != HttpStatusCode.Created) {
                var response = result.Content.ReadAsStringAsync().Result;
            }

            result.StatusCode.Should().Be(HttpStatusCode.Created);

            id = int.Parse(result.Headers.Location.OriginalString.Split("/").Last());
            dict = Utilities.CreateDictFromObject<RatingNew>();
            dict["StarRating"] = 3.0;
            dict["GoodsID"] = id;
            content = new StringContent(JsonSerializer.Serialize(dict), Encoding.UTF8, "application/json");
            result = await Client.PostAsync("/api/rating", content);
            if (result.StatusCode != HttpStatusCode.Created) {
                var response = result.Content.ReadAsStringAsync().Result;
            }

            result.StatusCode.Should().Be(HttpStatusCode.Created);
        }


        [Theory]
        [InlineData(10_000_002)]
        [InlineData(-1)]
        [InlineData("osawiej")]
        public async Task PostGoods_BadRequest(object value) {
            var types = new[] {"Price", "Weight", "Count", "CategoryID", "ManufacturerID"};
            foreach (var type in types) {
                var item = Utilities.CreateDictFromObject<GoodsNew>();
                item[type] = value;
                var httpContent = new StringContent(JsonSerializer.Serialize(item), Encoding.UTF8, "application/json");
                var result = await Client.PostAsync("/api/Goods", httpContent);
                result.StatusCode.Should().Be(StatusCodes.Status400BadRequest);
            }
        }

        [Theory]
        [InlineData(5.1)]
        [InlineData(600)]
        [InlineData(-1)]
        [InlineData("osawiej")]
        public async Task PostRatings_BadRequest(object value) {
            var types = new[] {"GoodsID", "StarRating"};
            foreach (var type in types) {
                var item = Utilities.CreateDictFromObject<RatingNew>();
                item[type] = value;
                var httpContent = new StringContent(JsonSerializer.Serialize(item), Encoding.UTF8, "application/json");
                var result = await Client.PostAsync("/api/Rating", httpContent);
                result.StatusCode.Should().Be(StatusCodes.Status400BadRequest);
            }
        }


        [Fact]
        public async Task PostTest_BadRequest() {
            var items = Utilities.CreateRandomItems<GoodsNew>();
            foreach (var item in items) {
                var httpContent = new StringContent(JsonSerializer.Serialize(item), Encoding.UTF8, "application/json");
                var result = await Client.PostAsync("/api/Goods", httpContent);
                result.StatusCode.Should().Be(StatusCodes.Status400BadRequest);
            }

            items = Utilities.CreateRandomItems<ManufacturerNew>();
            foreach (var item in items) {
                var httpContent = new StringContent(JsonSerializer.Serialize(item), Encoding.UTF8, "application/json");
                var result = await Client.PostAsync("/api/Manufacturer", httpContent);
                result.StatusCode.Should().Be(StatusCodes.Status400BadRequest);
            }

            items = Utilities.CreateRandomItems<RatingNew>();
            foreach (var item in items) {
                var httpContent = new StringContent(JsonSerializer.Serialize(item), Encoding.UTF8, "application/json");
                var result = await Client.PostAsync("/api/Rating", httpContent);
                result.StatusCode.Should().Be(StatusCodes.Status400BadRequest);
            }
        }
    }
}