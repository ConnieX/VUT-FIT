using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Bogus;

namespace API.Tests.Classes {
    public static class Utilities {
        public static Dictionary<string, object> CreateDictFromObject<T>() {
            var r = new Random();
            var f = new Faker();
            var result = new Dictionary<string, object>();
            var types = new List<Type> {typeof(string), typeof(int), typeof(double), typeof(int?), typeof(double?)};

            var props = typeof(T).GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(x => x.CustomAttributes.Any()).ToList();
            foreach (var prop in props) {
                if (!types.Contains(prop.PropertyType)) {
                    continue;
                }

                if (prop.PropertyType == typeof(string)) {
                    result.Add(prop.Name, f.Lorem.Word());
                }
                else {
                    result.Add(prop.Name, r.Next(1, 10));
                }
            }

            return result;
        }

        public static List<Dictionary<string, object>> CreateRandomItems<T>() {
            var r = new Random();
            var f = new Faker();
            var result = new List<Dictionary<string, object>>();

            var type = typeof(T);
            var props = type.GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(x => x.CustomAttributes.Any()).ToList();

            var types = new List<Type>() {typeof(string), typeof(int), typeof(double)};
            foreach (var prop in props) {
                var item = new Dictionary<string, object>();
                foreach (var innerProp in props) {
                    if (innerProp == prop || !types.Contains(innerProp.PropertyType)) {
                        continue;
                    }

                    if (innerProp.PropertyType == typeof(string)) {
                        item.Add(innerProp.Name, f.Lorem.Word());
                    }
                    else {
                        item.Add(innerProp.Name, r.Next(0, 10));
                    }
                }

                result.Add(item);
            }

            return result;
        }

        public static bool PropertiesEqual<T>(this T oldObj, T newObj) {
            var sourceType = oldObj.GetType();
            var destinationType = newObj.GetType();
            var sourceProperties = sourceType.GetProperties();
            foreach (var pi in sourceProperties) {
                if ((sourceType.GetProperty(pi.Name)?.GetValue(oldObj) == null &&
                     destinationType.GetProperty(pi.Name)?.GetValue(newObj) == null)) {
                    continue;
                }

                if (sourceType.GetProperty(pi.Name)?.GetValue(oldObj)?.ToString() !=
                    destinationType.GetProperty(pi.Name)?.GetValue(newObj, null)?.ToString()) {
                    return false;
                }
            }

            return true;
        }
    }
}