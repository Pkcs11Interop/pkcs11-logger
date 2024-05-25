/*
 *  Copyright 2011-2024 The Pkcs11Interop Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  Written for the Pkcs11Interop project by:
 *  Jaroslav IMRICH <jimrich@jimrich.sk>
 */

using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using Net.Pkcs11Interop.Common;
using Net.Pkcs11Interop.HighLevelAPI;

namespace Pkcs11Logger.Tests
{
    /// <summary>
    /// Test settings
    /// </summary>
    public static class Settings
    {
        /// <summary>
        /// Factories to be used by Pkcs11Interop library
        /// </summary>
        public static Pkcs11InteropFactories Pkcs11InteropFactories = new Pkcs11InteropFactories();

        /// <summary>
        /// The PKCS#11 unmanaged library path
        /// </summary>
        public static string Pkcs11LibraryPath = null;

        /// <summary>
        /// The normal user pin
        /// </summary>
        public static string NormalUserPin = @"11111111";

        /// <summary>
        /// The PKCS11-LOGGER unmanaged library path
        /// </summary>
        public static string Pkcs11LoggerLibraryPath = null;

        /// <summary>
        /// Primary log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath1 = null;

        /// <summary>
        /// Alternative log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath2 = null;

        /// <summary>
        /// Class constructor
        /// </summary>
        static Settings()
        {
            string testBasePath = Path.GetDirectoryName(typeof(Settings).Assembly.Location);
            string repoBasePath = testBasePath.Replace(Path.Combine("test", "Pkcs11LoggerTests", "bin", "Debug", "net60"), string.Empty);
            string platform = Platform.Uses32BitRuntime ? "x86" : "x64";

            if (Platform.IsWindows)
            {
                Pkcs11LibraryPath = Path.Combine(testBasePath, "pkcs11-mock", "windows", $"pkcs11-mock-{platform}.dll");
                Pkcs11LoggerLibraryPath = Path.Combine(repoBasePath, "build", "windows", $"pkcs11-logger-{platform}.dll");
                Pkcs11LoggerLogPath1 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-1.txt");
                Pkcs11LoggerLogPath2 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-2.txt");
            }
            else if (Platform.IsLinux)
            {
                // Set callback for resolving native library imports from Pkcs11Interop
                NativeLibrary.SetDllImportResolver(typeof(Pkcs11InteropFactories).Assembly, LinuxDllImportResolver);

                Pkcs11LibraryPath = Path.Combine(testBasePath, "pkcs11-mock", "linux", $"pkcs11-mock-{platform}.so");
                Pkcs11LoggerLibraryPath = Path.Combine(repoBasePath, "build", "linux", $"pkcs11-logger-{platform}.so");
                Pkcs11LoggerLogPath1 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-1.txt");
                Pkcs11LoggerLogPath2 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-2.txt");
            }
            else if (Platform.IsMacOsX)
            {
                Pkcs11LibraryPath = Path.Combine(testBasePath, "pkcs11-mock", "macos", $"pkcs11-mock-{platform}.dylib");
                Pkcs11LoggerLibraryPath = Path.Combine(repoBasePath, "build", "macos", $"pkcs11-logger.dylib");
                Pkcs11LoggerLogPath1 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-1.txt");
                Pkcs11LoggerLogPath2 = Path.Combine(testBasePath, $"pkcs11-logger-{platform}-2.txt");
            }
        }

        /// <summary>
        /// Callback for resolving native library imports
        /// </summary>
        /// <param name="libraryName">Name of the native library that needs to be resolved</param>
        /// <param name="assembly">The assembly loading the native library</param>
        /// <param name="dllImportSearchPath">The search path for native library</param>
        /// <returns>The OS handle for the loaded native library library</returns>
        static IntPtr LinuxDllImportResolver(string libraryName, Assembly assembly, DllImportSearchPath? dllImportSearchPath)
        {
            // Note: Pkcs11Interop tries to load "libdl" but Ubuntu 22.04 provides "libdl.so.2"
            string mappedLibraryName = (libraryName == "libdl") ? "libdl.so.2" : libraryName;
            return NativeLibrary.Load(mappedLibraryName, assembly, dllImportSearchPath);
        }
    }
}
