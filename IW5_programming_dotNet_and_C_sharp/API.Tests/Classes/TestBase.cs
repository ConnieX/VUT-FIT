using System.Net.Http;
using API.Data;
using Microsoft.Extensions.DependencyInjection;
using Xunit;

namespace API.Tests.Classes {
    public class TestBase : IClassFixture<ServerFactory<Startup>> {
        protected HttpClient Client { get; }
        protected ServerFactory<Startup> Server { get; }

        public TestBase(ServerFactory<Startup> fixture) {
            Client = fixture.CreateClient();
            Server = fixture;

            //get DB and fill it with data
            using var scope = Server.Services.CreateScope();
            var context = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();
            Database.Seed(context);
        }
    }
}