/*
 *  Copyright 2011-2025 The Pkcs11Interop Project
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
using System.Runtime.InteropServices;
using Net.Pkcs11Interop.Common;

namespace Pkcs11Logger.Tests
{
    /// <summary>
    /// Utility class for environment variables
    /// </summary>
    public class EnvVarUtils
    {
        /// <summary>
        /// Creates, modifies, or deletes an environment variable
        /// </summary>
        /// <param name="name">The name of an environment variable</param>
        /// <param name="value">A value to assign to variable</param>
        public static void SetEnvVar(string name, string value)
        {
            if (Platform.IsLinux || Platform.IsMacOsX)
            {
                if (value == null)
                {
                    if (0 != NativeMethods.unsetenv(name))
                        throw new Exception("Unmanaged function unsetenv failed");
                }
                else
                {
                    if (0 != NativeMethods.setenv(name, value, 1))
                        throw new Exception("Unmanaged function setenv failed");
                }
            }
            else
            {
                System.Environment.SetEnvironmentVariable(name, value);
            }
        }

        /// <summary>
        /// Definition of unmanaged functions
        /// </summary>
        private static class NativeMethods
        {
            /// <summary>
            /// Creates or modifies an environment variable
            /// </summary>
            /// <param name="name">The name of an environment variable</param>
            /// <param name="value">A value to assign to variable</param>
            /// <param name="overwrite">Indicates whether value of existing variable should be overwritten (nonzero) or not (zero)</param>
            /// <returns>Zero on success, or -1 on error, with errno set to indicate the cause of the error</returns>
            [DllImport("libc", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, BestFitMapping = false, ThrowOnUnmappableChar = true)]
            internal static extern int setenv(string name, string value, int overwrite);

            /// <summary>
            /// Deletes an environment variable
            /// </summary>
            /// <param name="name">The name of an environment variable</param>
            /// <returns>Zero on success, or -1 on error, with errno set to indicate the cause of the error</returns>
            [DllImport("libc", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, BestFitMapping = false, ThrowOnUnmappableChar = true)]
            internal static extern int unsetenv(string name);
        }
    }
}
