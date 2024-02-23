using System;
using System.Net.Http;
using AspNetMonsters.Blazor.Geolocation;
using FestivalTracker.Data;
using FestivalTracker.Services;
using Microsoft.AspNetCore.Authentication.Cookies;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.AspNetCore.Http;

namespace FestivalTracker {
    public class Startup {
        public Startup(IConfiguration configuration) {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        public void ConfigureServices(IServiceCollection services) {
            services.AddRazorPages();
            services.AddDbContext<DatabaseContext>(builder => {
                builder.UseSqlServer(Configuration.GetValue<string>("DbConnection"));
            });

            services.Configure<CookiePolicyOptions>(options => {
                options.CheckConsentNeeded = context => true;
                options.MinimumSameSitePolicy = SameSiteMode.None;
            });
            services.ConfigureApplicationCookie(options => {
                options.ExpireTimeSpan = TimeSpan.FromMinutes(30);
                options.LoginPath = "loginpage";
                options.SlidingExpiration = true;
            });
            services.AddAuthentication(CookieAuthenticationDefaults.AuthenticationScheme).AddCookie();
            services.AddAuthorization(options => {
                options.AddPolicy("Admin", policy => { policy.Requirements.Add(new PageAuthReq(3)); });
                options.AddPolicy("Organizer", policy => { policy.Requirements.Add(new PageAuthReq(2)); });
                options.AddPolicy("Cashier", policy => { policy.Requirements.Add(new PageAuthReq(1)); });
                options.AddPolicy("CreatedBy", policy => { policy.Requirements.Add(new FestivalAuth()); });
            });


            services.AddHttpContextAccessor();
            services.AddSingleton<IHttpContextAccessor, HttpContextAccessor>();
            services.AddScoped<IAuthorizationHandler, PagePermissionHandler>();

            services.AddHttpClient("HttpClient", client => { client.BaseAddress = new Uri("https://0.0.0.0:5001/"); });
            services.AddScoped<HttpClient>();
            services.AddScoped<CookieService>();
            services.AddScoped<UserManager>();
            services.AddScoped<TicketManager>();

            //services.AddTransient<LocationService>();
            services.AddScoped<GeoService>();

            services.AddServerSideBlazor();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env, IServiceProvider srv) {
            if (env.IsDevelopment()) {
                app.UseDeveloperExceptionPage();
            }
            else {
                app.UseExceptionHandler("/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }

            app.UseHttpsRedirection();
            app.UseStaticFiles();
            app.UseCookiePolicy();
            app.UseAuthentication();
            app.UseRouting();

            app.UseEndpoints(endpoints => {
                endpoints.MapBlazorHub();
                endpoints.MapFallbackToPage("/_Host");
            });
        }
        
    }
}