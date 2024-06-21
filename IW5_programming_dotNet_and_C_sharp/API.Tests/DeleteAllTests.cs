using System;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using API.Data;
using API.Tests.Classes;
using FluentAssertions;
using Microsoft.Extensions.DependencyInjection;
using Xunit;

namespace API.Tests {
    public class DeleteAllTests : TestBase {
        public DeleteAllTests(ServerFactory<Startup> fixture) : base(fixture) {
        }

        [Theory]
        [InlineData("/api/Goods/13", 13, "Goods")]
        [InlineData("/api/Goods/43", 43, "Goods")]
        [InlineData("/api/Goods/50", 50, "Goods")]
        [InlineData("/api/Manufacturer/5", 5, "Man")]
        [InlineData("/api/Manufacturer/15", 15, "Man")]
        [InlineData("/api/Rating/2", 2, "Ratings")]
        [InlineData("/api/Rating/500", 500, "Ratings")]
        // type - witch entity are we testing
        public async Task SuccDelete(string endpoint, int id, string type) {
            var response = await Client.DeleteAsync(endpoint); //send request and get response

            //get DB
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var found = type switch {
                "Goods" => (await context.Goods.FindAsync(id) != null),
                "Man" => (await context.Manufacturers.FindAsync(id) != null),
                "Ratings" => (await context.Ratings.FindAsync(id) != null),
                _ => true
            };

            response.StatusCode.Should().Be(HttpStatusCode.OK);
            found.Should().Be(false);
        }

        [Theory]
        [InlineData("/api/Goods/0")]
        [InlineData("/api/Goods/47483646")]
        [InlineData("/api/Goods/-47483646")]
        [InlineData("/api/Goods/some_text")]
        [InlineData("/api/Manufacturer/0")]
        [InlineData("/api/Manufacturer/47483646")]
        [InlineData("/api/Manufacturer/-47483646")]
        [InlineData("/api/Manufacturer/some_text")]
        [InlineData("/api/Rating/0")]
        [InlineData("/api/Rating/47483646")]
        [InlineData("/api/Rating/-47483646")]
        [InlineData("/api/Rating/some_text")]
        public async Task FailedDelete(string endpoint) {
            var response = await Client.DeleteAsync(endpoint + "0");
            if (endpoint.EndsWith("some_text")) {
                response.StatusCode.Should().Be(HttpStatusCode.BadRequest);
            }
            else {
                response.StatusCode.Should().Be(HttpStatusCode.NotFound);
            }
        }
    }
}