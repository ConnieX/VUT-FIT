using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using API.Data;
using API.Models.Search;
using API.Tests.Classes;
using FluentAssertions;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Newtonsoft.Json;
using Xunit;

namespace API.Tests {
    public class SearchTests : TestBase {
        public SearchTests(ServerFactory<Startup> fixture) : base(fixture) {
        }

        private async Task<SearchModel> ParseResult(HttpResponseMessage msg, string expected) {
            var content = await msg.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<List<SearchModel>>(content)
                .FirstOrDefault(x => x.Name.ToLower().Contains(expected.ToLower()));
        }


        private async Task CheckGoods(SearchModel model, ApplicationDbContext context) {
            model.Type.Should().Be("Goods");
            (await context.Goods.FindAsync(model.ID)).Name.Should().Be(model.Name);
        }

        private async Task CheckManufacturer(SearchModel model, ApplicationDbContext context) {
            model.Type.Should().Be("Manufacturer");
            (await context.Manufacturers.FindAsync(model.ID)).Name.Should().Be(model.Name);
        }

        private async Task CheckCategory(SearchModel model, ApplicationDbContext context) {
            model.Type.Should().Be("Category");
            (await context.Categories.FindAsync(model.ID)).Name.Should().Be(model.Name);
        }

        [Fact]
        public async Task Search_Ok() {
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            var firstGood = await context.Goods.FirstAsync();
            var result = await Client.GetAsync("/api/search/" + firstGood.Name);
            result.StatusCode.Should().Be(HttpStatusCode.OK);
            await CheckGoods(await ParseResult(result, firstGood.Name), context);

            var firstMan = await context.Manufacturers.FirstAsync();
            result = await Client.GetAsync("/api/search/" + firstMan.Name);
            result.StatusCode.Should().Be(HttpStatusCode.OK);
            await CheckManufacturer(await ParseResult(result, firstMan.Name), context);

            var firstCat = await context.Categories.FirstAsync();
            result = await Client.GetAsync("/api/search/" + firstCat.Name);
            result.StatusCode.Should().Be(HttpStatusCode.OK);
            await CheckCategory(await ParseResult(result, firstCat.Name), context);
        }

        [Fact]
        public async Task Search_Err() {
            var result = await Client.GetAsync("/api/search/");
            result.StatusCode.Should().Be(HttpStatusCode.NotFound);
        }
    }
}