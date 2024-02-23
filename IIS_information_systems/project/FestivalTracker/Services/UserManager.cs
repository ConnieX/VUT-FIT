using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Mail;
using System.Security.Claims;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using FestivalTracker.Data;
using Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Authorization;
using Microsoft.EntityFrameworkCore;
using Microsoft.JSInterop;


namespace FestivalTracker.Services {
    public class UserManager {
        private readonly DatabaseContext _database;
        private readonly AuthenticationStateProvider _provider;
        private readonly IJSRuntime _js;

        public User CurrentUser => GetCurrentUserAsync().Result;

        public Person DefaultReservation => GetDefaultUser();

        public UserManager(DatabaseContext database, AuthenticationStateProvider provider, IJSRuntime js) {
            _database = database;
            _provider = provider;
            _js = js;
        }


        /// <summary>
        /// Checks if user with this password exist in a database
        /// </summary>
        /// <param name="username">Username to check</param>
        /// <param name="password">Password to check</param>
        /// <returns>bool saying if login information is correct</returns>
        public async Task<bool> CheckLoginAsync(string username, string password) {
            var user = await _database.User.FirstOrDefaultAsync(x => x.Person.Email == username);
            return user != default && user.Password == Helper.ComputeSha256Hash(password);
        }

        public async Task<User> GetCurrentUserAsync() {
            var email = (await _provider.GetAuthenticationStateAsync()).User.Claims.FirstOrDefault(x =>
                x.Type == ClaimTypes.Email);
            if (email == default) {
                return null;
            }

            var user = _database.User.Include(x => x.Person).FirstOrDefault(x => x.Person.Email == email.Value);
            return user;
        }


        public bool CheckLogin(string username, string password, out User outUser) {
            var user = _database.User.FirstOrDefault(x => x.Person.Email == username);
            if (user != default && user.Password == Helper.ComputeSha256Hash(password)) {
                user.Person = _database.Person.Find(user.PersonId);
                outUser = user;
                return true;
            }
            outUser = null;
            return false;   
        }

        public bool CreateUser(string email, string password, string name, string surname, DateTime? birthDate,
            out string errorMsg) {
            if (string.IsNullOrEmpty(name) || string.IsNullOrEmpty(surname)) {
                errorMsg = "Nezadal jste jméno nebo příjmení";
                return false;
            }

            if (!ValidateEmail(email)) {
                errorMsg = "Zadaný email není správný";
                return false;
            }

            if (!ValidatePassword(password, out var err)) {
                errorMsg = err;
                return false;
            }

            if (_database.Person.Any(x => x.Email == email)) {
                errorMsg = "Uživatel s tímto emailem již existuje";
                return false;
            }


            var person = new Person {Email = email, Name = name, Surname = surname, Birth = birthDate};
            _database.Person.Add(person);
            _database.SaveChanges(); //We need this save to database to create the autoincrement ID
            var user = new User {Password = Helper.ComputeSha256Hash(password), Person = person, PersonId = person.Id};
            _database.User.Add(user);
            _database.SaveChanges();
            errorMsg = "";
            return true;
        }

        private Person GetDefaultUser() {
            var user = _database.Person.FirstOrDefault(x => x.Name == "Default" && x.Surname == "User");
            if (user != default) {
                return user;
            }

            var p = new Person {Birth = DateTime.Now, Name = "Default", Surname = "User", Email = "default"};
            _database.Person.Add(p);
            _database.SaveChanges();
            return p;
        }


        public async Task Login(string username, string password, string redirect) {
            await _js.InvokeVoidAsync("login", username, password, redirect);
        }

        private static bool ValidatePassword(string password, out string validationError) {
            if (password == null || password.Length < 4) {
                validationError = "Heslo je moc krátké. Minimum je 5 znaků";
                return false;
            }

            if (!password.Any(char.IsDigit)) {
                validationError = "Heslo neobsahuje číslo";
                return false;
            }

            validationError = "";
            return true;
        }


        public static bool ValidateEmail(string email) {
            if (string.IsNullOrWhiteSpace(email))
                return false;

            try {
                // Normalize the domain
                email = Regex.Replace(email, @"(@)(.+)$", DomainMapper,
                    RegexOptions.None, TimeSpan.FromMilliseconds(200));

                // Examines the domain part of the email and normalizes it.
                string DomainMapper(Match match) {
                    // Use IdnMapping class to convert Unicode domain names.
                    var idn = new IdnMapping();

                    // Pull out and process domain name (throws ArgumentException on invalid)
                    string domainName = idn.GetAscii(match.Groups[2].Value);

                    return match.Groups[1].Value + domainName;
                }
            }
            catch (RegexMatchTimeoutException e) {
                return false;
            }
            catch (ArgumentException e) {
                return false;
            }

            try {
                return Regex.IsMatch(email,
                    @"^[^@\s]+@[^@\s]+\.[^@\s]+$",
                    RegexOptions.IgnoreCase, TimeSpan.FromMilliseconds(250));
            }
            catch (RegexMatchTimeoutException) {
                return false;
            }
        }

        public async Task DeleteUser(int id) {
            var user = await _database.User.FindAsync(id);
            if (user != null) {
                await DeleteUser(user);
            }
        }

        public async Task DeleteUser(User user) {
            user.Person ??= await _database.Person.FindAsync(user.PersonId);
            var tickets = _database.Ticket.Include(x => x.Reservation)
                .Where(x => x.Reservation.PersonId == user.PersonId);
            _database.Ticket.RemoveRange(tickets);
            _database.Reservation.RemoveRange(tickets.Select(x=>x.Reservation));
            _database.User.Remove(user);
            _database.Person.Remove(user.Person);
            await _database.SaveChangesAsync();
        }
    }
}