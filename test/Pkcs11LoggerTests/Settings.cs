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

namespace Pkcs11Logger.Tests
{
    /// <summary>
    /// Test settings
    /// </summary>
    public static class Settings
    {
        /// <summary>
        /// The PKCS#11 unmanaged library path
        /// </summary>
        public static string Pkcs11LibraryPath = @"c:\test\pkcs11-mock-x86.dll";

        /// <summary>
        /// The normal user pin
        /// </summary>
        public static string NormalUserPin = @"11111111";

        /// <summary>
        /// The PKCS11-LOGGER unmanaged library path
        /// </summary>
        public static string Pkcs11LoggerLibraryPath = @"c:\test\pkcs11-logger-x86.dll";

        /// <summary>
        /// Primary log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath1 = @"c:\test\pkcs11-logger-x86-1.txt";

        /// <summary>
        /// Alternative log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath2 = @"c:\test\pkcs11-logger-x86-2.txt";
    }
}
