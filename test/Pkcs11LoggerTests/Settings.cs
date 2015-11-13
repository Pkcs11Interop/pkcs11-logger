/*
 *  PKCS11-LOGGER - PKCS#11 logging proxy module
 *  Copyright (c) 2011-2015 JWC s.r.o. <http://www.jwc.sk>
 *  Author: Jaroslav Imrich <jimrich@jimrich.sk>
 *
 *  Licensing for open source projects:
 *  PKCS11-LOGGER is available under the terms of the GNU Affero General 
 *  Public License version 3 as published by the Free Software Foundation.
 *  Please see <http://www.gnu.org/licenses/agpl-3.0.html> for more details.
 *
 *  Licensing for other types of projects:
 *  PKCS11-LOGGER is available under the terms of flexible commercial license.
 *  Please contact JWC s.r.o. at <info@pkcs11interop.net> for more details.
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
        public static string Pkcs11LibraryPath = @"siecap11.dll";

        /// <summary>
        /// The normal user pin
        /// </summary>
        public static string NormalUserPin = @"11111111";

        /// <summary>
        /// The PKCS11-LOGGER unmanaged library path
        /// </summary>
        public static string Pkcs11LoggerLibraryPath = @"c:\pkcs11-logger-x86.dll";

        /// <summary>
        /// Primary log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath1 = @"c:\pkcs11-logger-x86-1.txt";

        /// <summary>
        /// Alternative log file path
        /// </summary>
        public static string Pkcs11LoggerLogPath2 = @"c:\pkcs11-logger-x86-2.txt";
    }
}
