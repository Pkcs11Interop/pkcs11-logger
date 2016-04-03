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

using System;
using System.IO;
using Net.Pkcs11Interop.Common;
using Net.Pkcs11Interop.HighLevelAPI;
using NUnit.Framework;

namespace Pkcs11Logger.Tests
{
    /// <summary>
    /// Basic tests for PKCS11-LOGGER library
    /// </summary>
    public class BasicPkcs11LoggerTests
    {
        #region Imports from PKCS11-LOGGER

        /// <summary>
        /// Environment variable that specifies path to the original PKCS#11 library
        /// </summary>
        public const string PKCS11_LOGGER_LIBRARY_PATH = "PKCS11_LOGGER_LIBRARY_PATH";

        /// <summary>
        /// Environment variable that specifies path to the log file
        /// </summary>
        public const string PKCS11_LOGGER_LOG_FILE_PATH = "PKCS11_LOGGER_LOG_FILE_PATH";

        /// <summary>
        /// Environment variable that specifies pkcs11-logger flags
        /// </summary>
        public const string PKCS11_LOGGER_FLAGS = "PKCS11_LOGGER_FLAGS";

        /// <summary>
        /// Flag that disables logging into the log file
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE = 0x00000001;

        /// <summary>
        /// Flag that disables logging of process ID
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID = 0x00000002;

        /// <summary>
        /// Flag that disables logging of thread ID
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID = 0x00000004;

        /// <summary>
        /// Flag that enables logging of PINs
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_ENABLE_PIN = 0x00000008;

        /// <summary>
        /// Flag that enables logging to the stdout
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_ENABLE_STDOUT = 0x00000010;

        /// <summary>
        /// Flag that enables logging to the stderr
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_ENABLE_STDERR = 0x00000020;

        /// <summary>
        /// Flag that enables reopening of log file
        /// </summary>
        public const uint PKCS11_LOGGER_FLAG_ENABLE_FCLOSE = 0x00000040;

        #endregion

        /// <summary>
        /// Deletes environment variables used by PKCS11-LOGGER library 
        /// </summary>
        private void DeleteEnvironmentVariables()
        {
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, null);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, null);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, null);
        }

        /// <summary>
        /// Test environment variable processing
        /// </summary>
        [Test()]
        public void EnvironmentVariableProcessingTest()
        {
            DeleteEnvironmentVariables();

            // PKCS11_LOGGER_LIBRARY_PATH is required
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, null);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, null);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, null);
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_LOG_FILE_PATH and PKCS11_LOGGER_FLAGS are optional
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, null);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, null);
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // PKCS11_LOGGER_LIBRARY_PATH has to be provided without enclosing quotes
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, "\"" + Settings.Pkcs11LibraryPath + "\"");
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, null);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, null);
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_LOG_FILE_PATH has to be provided without enclosing quotes
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, "\"" + Settings.Pkcs11LoggerLogPath1 + "\"");
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, null);
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_FLAGS must contain a number
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, "InvalidValue");
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }
        }

        /// <summary>
        /// Test PKCS11_LOGGER_LIBRARY_PATH environment variable
        /// </summary>
        [Test()]
        public void LibraryPathTest()
        {
            DeleteEnvironmentVariables();

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Existing PKCS#11 library path should work
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");

            // Non-existing PKCS#11 library path should not work
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, @"NonExistingLibrary.dll");
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // Unspecified PKCS#11 library should not work
            try
            {
                System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, null);
                using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                    pkcs11.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is Pkcs11Exception);
                Assert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }
        }

        /// <summary>
        /// Test PKCS11_LOGGER_LOG_FILE_PATH environment variable
        /// </summary>
        [Test()]
        public void LogFilePathTest()
        {
            DeleteEnvironmentVariables();

            // Delete log files
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);
            if (File.Exists(Settings.Pkcs11LoggerLogPath2))
                File.Delete(Settings.Pkcs11LoggerLogPath2);

            // Log to Pkcs11LoggerLogPath1
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");
            if (File.Exists(Settings.Pkcs11LoggerLogPath2))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath2 + " exists");

            // Delete log files
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);
            if (File.Exists(Settings.Pkcs11LoggerLogPath2))
                File.Delete(Settings.Pkcs11LoggerLogPath2);

            // Log to Pkcs11LoggerLogPath2
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath2);
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath2 exists
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");
            if (!File.Exists(Settings.Pkcs11LoggerLogPath2))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath2 + " does not exist");

            // Delete log files
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);
            if (File.Exists(Settings.Pkcs11LoggerLogPath2))
                File.Delete(Settings.Pkcs11LoggerLogPath2);

            // Settings.LogFilePath may also be null
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, null);
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");
            if (File.Exists(Settings.Pkcs11LoggerLogPath2))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath2 + " exists");
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE flag
        /// </summary>
        [Test()]
        public void DisableLogFileTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with disabled log file
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");

            // Log to Pkcs11LoggerLogPath1 with enabled log file
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID and PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID flags
        /// </summary>
        [Test()]
        public void DisableProcessIdAndThreadIdTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with both IDs enabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Read first line from the log file
            string line = null;
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();

            // Get ProcessID and ThreadID
            string[] parts = line.Split(new string[] { " : " }, StringSplitOptions.RemoveEmptyEntries);
            Assert.IsTrue(parts != null && parts.Length == 3);
            string processId = parts[0];
            string threadId = parts[1];
            Assert.IsTrue(processId.StartsWith("0x") && threadId.StartsWith("0x"));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with ProcessID disabled
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check if the first line starts with ThreadID
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();
            Assert.IsTrue(line.StartsWith(threadId));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with both IDs disabled
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();

            // Check if both IDs are missing
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();
            Assert.IsTrue(!line.StartsWith(processId) && !line.StartsWith(threadId));
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_PIN flag
        /// </summary>
        [Test()]
        public void EnablePinTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with PIN logging disabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_PIN;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            using (Session session = pkcs11.GetSlotList(true)[0].OpenSession(true))
                session.Login(CKU.CKU_USER, Settings.NormalUserPin);

            Assert.IsTrue(File.ReadAllText(Settings.Pkcs11LoggerLogPath1).Contains(" *pPin: *** Intentionally hidden ***"));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with PIN logging enabled
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_PIN;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            using (Session session = pkcs11.GetSlotList(true)[0].OpenSession(true))
                session.Login(CKU.CKU_USER, Settings.NormalUserPin);

            Assert.IsTrue(File.ReadAllText(Settings.Pkcs11LoggerLogPath1).Contains(" *pPin: " + Settings.NormalUserPin));
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_STDOUT flag
        /// </summary>
        [Test()]
        public void EnableStdOutTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Test result needs to be verified visually in NUnit console.
            // To see stdout NUnit GUI needs to be executed with console switch: "nunit-x86.exe /console"
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_STDOUT;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_STDERR flag
        /// </summary>
        [Test()]
        public void EnableStdErrTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Test result needs to be verified visually in NUnit console.
            // To see stdout NUnit GUI needs to be executed with console switch: "nunit-x86.exe /console"
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_STDERR;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
                pkcs11.GetInfo();
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_FCLOSE flag
        /// </summary>
        [Test()]
        public void EnableFcloseTest()
        {
            DeleteEnvironmentVariables();

            // Tested file locking behavior is valid only on Windows platform
            if (!Platform.IsWindows)
                Assert.Inconclusive("Test cannot be executed on this platform");

            uint flags = 0;

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with fclose disabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            {
                pkcs11.GetInfo();

                // Check whether log file exists
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");

                try
                {
                    // It should not be possible to delete log file
                    File.Delete(Settings.Pkcs11LoggerLogPath1);
                    Assert.Fail("Exception expected but not thrown");
                }
                catch (Exception ex)
                {
                    Assert.IsTrue(ex is IOException);
                }

                // Check whether log file was not deleted
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");
            }

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with fclose enabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            {
                pkcs11.GetInfo();

                // Check whether log file exists
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");

                // It should be possible to delete log file
                File.Delete(Settings.Pkcs11LoggerLogPath1);

                // Check whether log file was deleted
                if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");

                pkcs11.GetInfo();

                // Check whether log file exists
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");
            }

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);
        }

        /// <summary>
        /// Test performance with PKCS11_LOGGER_FLAG_ENABLE_FCLOSE flag
        /// </summary>
        [Test()]
        public void EnableFclosePerformanceTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;
            int fcloseDisabledTicks = 0;
            int fcloseEnabledTicks = 0;

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with fclose disabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            {
                int tickCountStart = Environment.TickCount;

                for (int i = 0; i < 10; i++)
                {
                    pkcs11.GetInfo();
                    foreach (Slot slot in pkcs11.GetSlotList(true))
                    {
                        slot.GetTokenInfo();
                        foreach (CKM mechanism in slot.GetMechanismList())
                        {
                            slot.GetMechanismInfo(mechanism);
                        }
                    }
                }

                int tickCountStop = Environment.TickCount;
                fcloseDisabledTicks = tickCountStop - tickCountStart;
            }

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with fclose enabled
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            System.Environment.SetEnvironmentVariable(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (Pkcs11 pkcs11 = new Pkcs11(Settings.Pkcs11LoggerLibraryPath, true))
            {
                int tickCountStart = Environment.TickCount;

                for (int i = 0; i < 10; i++)
                {
                    pkcs11.GetInfo();
                    foreach (Slot slot in pkcs11.GetSlotList(true))
                    {
                        slot.GetTokenInfo();
                        foreach (CKM mechanism in slot.GetMechanismList())
                        {
                            slot.GetMechanismInfo(mechanism);
                        }
                    }
                }

                int tickCountStop = Environment.TickCount;
                fcloseEnabledTicks = tickCountStop - tickCountStart;
            }
            
            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // PKCS11_LOGGER_FLAG_ENABLE_FCLOSE decreases performance
            Assert.IsTrue(fcloseEnabledTicks > fcloseDisabledTicks);
        }
    }
}
