using System;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Web;

namespace FestivalTracker {
    public static class Helper {
        public static string Encode(string param) {
            return string.IsNullOrEmpty(param) ? "" : HttpUtility.UrlEncode(param);
        }

        public static string ComputeSha256Hash(string rawData) {
            using var sha256Hash = SHA256.Create();
            var bytes = sha256Hash.ComputeHash(Encoding.UTF8.GetBytes(rawData));
            var builder = new StringBuilder();
            foreach (var t in bytes) {
                builder.Append(t.ToString("x2"));
            }

            return builder.ToString();
        }

        public static string TranslateRole(int role) => role switch {
            0 => "Uživatel",
            1 => "Pokladní",
            2 => "Pořadatel",
            3 => "Administrátor",
            _ => ""
        };
    }

    public static class StringExtensions {
        public static string FirstCharToUpper(this string input) =>
            input switch {
                null => throw new ArgumentNullException(nameof(input)),
                "" => throw new ArgumentException($"{nameof(input)} cannot be empty", nameof(input)),
                _ => input.First().ToString().ToUpper() + input.Substring(1)
            };
    }
}