using System.Linq;
using API.Data;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc.Testing;
using Microsoft.AspNetCore.TestHost;
using Microsoft.Data.Sqlite;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

namespace API.Tests.Classes {
    public class ServerFactory<TStartup> : WebApplicationFactory<TStartup> where TStartup : class {
        private SqliteConnection _connection;

        protected override void ConfigureWebHost(IWebHostBuilder builder) {
            builder.ConfigureTestServices(services => {
                var descriptor = services.SingleOrDefault(
                    d => d.ServiceType ==
                         typeof(DbContextOptions<ApplicationDbContext>));

                services.Remove(descriptor);
                // Add ApplicationDbContext using an in-memory database for testing.
                _connection = new SqliteConnection("Data Source=:memory:;");
                _connection.Open();


                services.AddDbContext<ApplicationDbContext>(options => { options.UseSqlite(_connection); },
                    ServiceLifetime.Transient);
                var sp = services.BuildServiceProvider();
                using var scope = sp.CreateScope();
                var scopedServices = scope.ServiceProvider;
                var db = scopedServices.GetRequiredService<ApplicationDbContext>();
                db.Database.EnsureCreated();
            });
        }

        protected override void Dispose(bool disposing) {
            _connection.Close();
            base.Dispose(disposing);
        }
    }
}