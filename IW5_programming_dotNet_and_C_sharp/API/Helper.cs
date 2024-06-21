using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace API {
    public static class Helper {
        public static Type[] GetInherited(Type t) {
            return Assembly.GetCallingAssembly().GetTypes().Where(x => x.IsClass && !x.IsAbstract && x.BaseType == t)
                .ToArray();
        }

        public static void Update<T>(this T old, T obj) {
            var properties = typeof(T).GetProperties(BindingFlags.Public | BindingFlags.Instance);
            foreach (var property in properties) {
                var value = property.GetValue(obj);
                if (IsNullOrDefault(value)) {
                    continue;
                }

                property.SetValue(old, value);
            }
        }

        private static bool IsNullOrDefault<T>(T argument) {
            if (argument == null) {
                return true;
            }

            var methodType = typeof(T);
            if (Nullable.GetUnderlyingType(methodType) != null) {
                return false;
            }

            var argumentType = argument.GetType();
            if (!argumentType.IsValueType || argumentType == methodType) {
                return false;
            }

            var obj = Activator.CreateInstance(argument.GetType());
            return obj.Equals(argument);
        }
    }
}