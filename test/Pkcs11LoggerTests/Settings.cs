/*
 *  Copyright 2011-2016 The Pkcs11Interop Project
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
            if (Platform.IsWindows)
            {
                if (Platform.Uses32BitRuntime)
                {
                    Pkcs11LibraryPath = @"c:\pkcs11-logger-test\pkcs11-mock-x86.dll";
                    Pkcs11LoggerLibraryPath = @"c:\pkcs11-logger-test\pkcs11-logger-x86.dll";
                    Pkcs11LoggerLogPath1 = @"c:\pkcs11-logger-test\pkcs11-logger-x86-1.txt";
                    Pkcs11LoggerLogPath2 = @"c:\pkcs11-logger-test\pkcs11-logger-x86-2.txt";
                }
                else
                {
                    Pkcs11LibraryPath = @"c:\pkcs11-logger-test\pkcs11-mock-x64.dll";
                    Pkcs11LoggerLibraryPath = @"c:\pkcs11-logger-test\pkcs11-logger-x64.dll";
                    Pkcs11LoggerLogPath1 = @"c:\pkcs11-logger-test\pkcs11-logger-x64-1.txt";
                    Pkcs11LoggerLogPath2 = @"c:\pkcs11-logger-test\pkcs11-logger-x64-2.txt";
                }
            }
            else if (Platform.IsLinux)
            {
                if (Platform.Uses32BitRuntime)
                {
                    Pkcs11LibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-mock-x86.so";
                    Pkcs11LoggerLibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86.so";
                    Pkcs11LoggerLogPath1 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86-1.txt";
                    Pkcs11LoggerLogPath2 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86-2.txt";
                }
                else
                {
                    Pkcs11LibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-mock-x64.so";
                    Pkcs11LoggerLibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64.so";
                    Pkcs11LoggerLogPath1 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64-1.txt";
                    Pkcs11LoggerLogPath2 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64-2.txt";
                }
            }
            else if (Platform.IsMacOsX)
            {
                if (Platform.Uses32BitRuntime)
                {
                    Pkcs11LibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-mock-x86.dylib";
                    Pkcs11LoggerLibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86.dylib";
                    Pkcs11LoggerLogPath1 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86-1.txt";
                    Pkcs11LoggerLogPath2 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x86-2.txt";
                }
                else
                {
                    Pkcs11LibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-mock-x64.dylib";
                    Pkcs11LoggerLibraryPath = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64.dylib";
                    Pkcs11LoggerLogPath1 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64-1.txt";
                    Pkcs11LoggerLogPath2 = @"/tmp/pkcs11-logger-test/pkcs11-logger-x64-2.txt";
                }
            }
        }
    }
}
