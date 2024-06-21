using System.Net;
using System.Net.Http;
using System.Text;
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
using JsonSerializer = System.Text.Json.JsonSerializer;

namespace API.Tests {
    public class PutAllTests : TestBase {
        public PutAllTests(ServerFactory<Startup> fixture) : base(fixture) {
        }

        [Theory]
        [InlineData("/api/Goods/", 20, "Goods", false)]
        [InlineData("/api/Goods/", 50, "Goods", true)]
        [InlineData("/api/Manufacturer/", 10, "Man", false)]
        [InlineData("/api/Manufacturer/", 5, "Man", true)]
        [InlineData("/api/Rating/", 1, "Ratings", true)]
        [InlineData("/api/Rating/", 499, "Ratings", false)]
        // type - witch entity are we testing
        // all - true if need to fill all params, false if need to fill only some params
        public async Task SuccPut(string endpoint, int id, string type, bool all) {
            var json = "";
            switch (type) {
                case "Goods":
                    var newGoods = new GoodsUpdate();
                    newGoods.Name = "Something Cool";
                    newGoods.Count = 47;
                    if (all) {
                        newGoods.Description = "asdasdfa";
                        newGoods.Price = 456;
                        newGoods.Weight = 200;
                    }

                    json = JsonSerializer.Serialize(newGoods);
                    break;
                case "Man":
                    var newMan = new ManufacturerUpdate();
                    newMan.Name = "Something Cool";
                    if (all) {
                        newMan.Country = "Czech";
                        newMan.Description = "fadfas";
                    }

                    json = JsonSerializer.Serialize(newMan);
                    break;
                case "Ratings":
                    var newRating = new RatingUpdate();
                    newRating.StarRating = 5;
                    if (all) {
                        newRating.TextRating = "afasf";
                    }

                    json = JsonSerializer.Serialize(newRating);
                    break;
            }

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request
            var response = await Client.PutAsync(endpoint + "1", data); //send request and get the response 

            response.StatusCode.Should().Be(HttpStatusCode.OK);
        }

        [Theory]
        [InlineData("/api/Goods/0", "Goods")]
        [InlineData("/api/Goods/47483646", "Goods")]
        [InlineData("/api/Goods/-27483646", "Goods")]
        [InlineData("/api/Goods/some_text", "Goods")]
        [InlineData("/api/Manufacturer/0", "Man")]
        [InlineData("/api/Manufacturer/47483646", "Man")]
        [InlineData("/api/Manufacturer/-47483646", "Man")]
        [InlineData("/api/Manufacturer/some_text", "Man")]
        [InlineData("/api/Rating/0", "Ratings")]
        [InlineData("/api/Rating/47483647", "Ratings")]
        [InlineData("/api/Rating/-47483647", "Ratings")]
        [InlineData("/api/Rating/some_text", "Ratings")]
        // type - witch entity are we testing
        public async Task PutInvalidId(string endpoint, string type) {
            var json = "";
            switch (type) {
                case "Goods":
                    var newGoods = new GoodsUpdate();
                    newGoods.Name = "Something Cool";
                    json = JsonSerializer.Serialize(newGoods);
                    break;
                case "Man":
                    var newMan = new ManufacturerUpdate();
                    newMan.Name = "Something Cool";
                    json = JsonSerializer.Serialize(newMan);
                    break;
                case "Ratings":
                    var newRating = new RatingUpdate();
                    newRating.StarRating = 5;
                    json = JsonSerializer.Serialize(newRating);
                    break;
            }

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request
            var response = await Client.PutAsync(endpoint + "0", data); //send request and get the response 

            if (endpoint.EndsWith("some_text")) {
                response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
            }
            else {
                response.StatusCode.Should().Be(HttpStatusCode.NotFound);
            }
        }

        [Theory]
        [InlineData("/api/Goods/", "Goods", -1, "price")]
        [InlineData("/api/Goods/", "Goods", -1, "weight")]
        [InlineData("/api/Goods/", "Goods", -1, "count")]
        [InlineData("/api/Goods/", "Goods", 10_000_001, "price")]
        [InlineData("/api/Goods/", "Goods", 10_000_001, "weight")]
        [InlineData("/api/Goods/", "Goods", 10_000_001, "count")]
        [InlineData("/api/Rating/", "Ratings", 5.1, "star")]
        [InlineData("/api/Rating/", "Ratings", 6, "star")]
        [InlineData("/api/Rating/", "Ratings", -0.1, "star")]
        [InlineData("/api/Rating/", "Ratings", -5, "star")]
        // type - witch entity are we testing
        // param - witch parameter we want to put value in
        public async Task PutValueOutOfRange(string endpoint, string type, double value, string param) {
            string json = "";
            if (type == "Goods") {
                var newGoods = new GoodsUpdate();
                switch (param) {
                    case "price":
                        newGoods.Price = value;
                        break;
                    case "weight":
                        newGoods.Weight = value;
                        break;
                    case "count":
                        newGoods.Count = (int) value;
                        break;
                }

                json = JsonSerializer.Serialize(newGoods);
            }
            else {
                var newRating = new RatingUpdate {StarRating = value};
                json = JsonSerializer.Serialize(newRating);
            }

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request
            var response = await Client.PutAsync(endpoint + "1", data); //send request and get the response 

            response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
        }

        [Fact]
        public async Task PutManToRating() {
            //get DB
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var oldRating = await context.Ratings.FindAsync(1); //get rating ID 1 with old values
            //create object of type Manufacturer and put it in Rating
            var newMan = new ManufacturerUpdate {Name = "Something Totally Not Cool"};
            var json = JsonSerializer.Serialize(newMan);

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request 
            var response = await Client.PutAsync("/api/Rating/1", data); //send request and get the response 

            //get new values of rating ID 1
            var content = await response.Content.ReadAsStringAsync();
            var newRating = JsonConvert.DeserializeObject<Rating>(content);


            var equals = oldRating.PropertiesEqual(newRating);

            response.StatusCode.Should().Be(HttpStatusCode.OK); //DB should ignore unknown params
            equals.Should().Be(true); //record should not change
        }

        [Fact]
        public async Task PutGoodsToRating() {
            //get DB
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var oldRating = await context.Ratings.FindAsync(1); //get rating ID 1 with old values
            //create object of type Goods and put it in Rating
            var newGoods = new GoodsUpdate {Name = "Something Totally Not Cool"};
            var json = JsonSerializer.Serialize(newGoods);

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request
            var response = await Client.PutAsync("/api/Rating/1", data); //send request and get the response 

            //get new values of rating ID 1
            var content = await response.Content.ReadAsStringAsync();
            var newRating = JsonConvert.DeserializeObject<Rating>(content);
            var equals = oldRating.PropertiesEqual(newRating);

            response.StatusCode.Should().Be(HttpStatusCode.OK); //DB should ignore unknown params
            equals.Should().Be(true); //record should not change
        }

        [Fact]
        public async Task PutCountToMan() {
            //get DB
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var oldMan = await context.Manufacturers.FindAsync(1); //get manufacturer ID 1 with old values
            //create object of type Goods and put it in Manufacturer
            var newGoods = new GoodsUpdate {Count = 5};
            var json = JsonSerializer.Serialize(newGoods);

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request 
            var response = await Client.PutAsync("/api/Manufacturer/1", data); //send request and get the response

            //get new values of manufacturer ID 1
            var content = await response.Content.ReadAsStringAsync();
            var newMan = JsonConvert.DeserializeObject<Manufacturer>(content);

            var equals = oldMan.PropertiesEqual(newMan);

            response.StatusCode.Should().Be(HttpStatusCode.OK); //DB should ignore unknown params
            equals.Should().Be(true); //record should not change
        }

        [Fact]
        public async Task PutCountryToGoods() {
            //get DB
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var oldGoods = await context.Goods.FindAsync(1); //get good ID 1 with old values
            //create object of type Manufacturer and put it in Goods
            var newMan = new ManufacturerUpdate {Country = "Czech"};
            var json = JsonSerializer.Serialize(newMan);

            var data = new StringContent(json, Encoding.UTF8, "application/json"); //create request
            var response = await Client.PutAsync("/api/Goods/1", data); //send request and get the response 

            //get new values of rating ID 1
            var content = await response.Content.ReadAsStringAsync();
            var newGoods = JsonConvert.DeserializeObject<Goods>(content);
            var equals = oldGoods.PropertiesEqual(newGoods);

            response.StatusCode.Should().Be(HttpStatusCode.OK); //DB should ignore unknown params
            equals.Should().Be(true); //record should not change
        }
    }
}