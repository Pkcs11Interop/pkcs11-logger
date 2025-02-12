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
using System.IO;
using Net.Pkcs11Interop.Common;
using Net.Pkcs11Interop.HighLevelAPI;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Pkcs11Logger.Tests
{
    /// <summary>
    /// Basic tests for PKCS11-LOGGER library
    /// </summary>
    [TestFixture()]
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, null);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, null);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, null);
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
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, null);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, null);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, null);
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_LOG_FILE_PATH and PKCS11_LOGGER_FLAGS are optional
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, null);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, null);
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // PKCS11_LOGGER_LIBRARY_PATH has to be provided without enclosing quotes
            try
            {
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, "\"" + Settings.Pkcs11LibraryPath + "\"");
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, null);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, null);
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_LOG_FILE_PATH has to be provided without enclosing quotes
            try
            {
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, "\"" + Settings.Pkcs11LoggerLogPath1 + "\"");
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, null);
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // PKCS11_LOGGER_FLAGS must contain a number
            try
            {
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, "InvalidValue");
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");

            // Non-existing PKCS#11 library path should not work
            try
            {
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, @"NonExistingLibrary.dll");
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
            }

            // Unspecified PKCS#11 library should not work
            try
            {
                EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, null);
                using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                    pkcs11Library.GetInfo();

                Assert.Fail("Exception expected but not thrown");
            }
            catch (Exception ex)
            {
                ClassicAssert.IsTrue(ex is Pkcs11Exception);
                ClassicAssert.IsTrue(((Pkcs11Exception)ex).RV == CKR.CKR_GENERAL_ERROR);
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath2);
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, null);
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // Check whether Pkcs11LoggerLogPath1 exists
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");

            // Log to Pkcs11LoggerLogPath1 with enabled log file
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_LOG_FILE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // Read first line from the log file
            string line = null;
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();

            // Get ProcessID and ThreadID
            string[] parts = line.Split(new string[] { " : " }, StringSplitOptions.RemoveEmptyEntries);
            ClassicAssert.IsTrue(parts != null && parts.Length == 3);
            string processId = parts[0];
            string threadId = parts[1];
            ClassicAssert.IsTrue(processId.StartsWith("0x") && threadId.StartsWith("0x"));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with ProcessID disabled
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags & ~PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // Check if the first line starts with ThreadID
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();
            ClassicAssert.IsTrue(line.StartsWith(threadId));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with both IDs disabled
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_PROCESS_ID;
            flags = flags | PKCS11_LOGGER_FLAG_DISABLE_THREAD_ID;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();

            // Check if both IDs are missing
            using (StreamReader reader = new StreamReader(Settings.Pkcs11LoggerLogPath1))
                line = reader.ReadLine();
            ClassicAssert.IsTrue(!line.StartsWith(processId) && !line.StartsWith(threadId));
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_PIN;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            using (ISession session = pkcs11Library.GetSlotList(SlotsType.WithTokenPresent)[0].OpenSession(SessionType.ReadOnly))
                session.Login(CKU.CKU_USER, Settings.NormalUserPin);

            ClassicAssert.IsTrue(File.ReadAllText(Settings.Pkcs11LoggerLogPath1).Contains(" *pPin: *** Intentionally hidden ***"));

            // Delete log file
            File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with PIN logging enabled
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_PIN;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            using (ISession session = pkcs11Library.GetSlotList(SlotsType.WithTokenPresent)[0].OpenSession(SessionType.ReadOnly))
                session.Login(CKU.CKU_USER, Settings.NormalUserPin);

            ClassicAssert.IsTrue(File.ReadAllText(Settings.Pkcs11LoggerLogPath1).Contains(" *pPin: " + Settings.NormalUserPin));
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_STDOUT flag
        /// </summary>
        [Test()]
        public void EnableStdOutTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Test result needs to be verified visually in NUnit console:
            //   nunit3-console.exe Pkcs11LoggerTests.dll --test=Pkcs11Logger.Tests.BasicPkcs11LoggerTests.EnableStdOutTest
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_STDOUT;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();
        }

        /// <summary>
        /// Test PKCS11_LOGGER_FLAG_ENABLE_STDERR flag
        /// </summary>
        [Test()]
        public void EnableStdErrTest()
        {
            DeleteEnvironmentVariables();

            uint flags = 0;

            // Test result needs to be verified visually in NUnit console:
            //   nunit3-console.exe Pkcs11LoggerTests.dll --test=Pkcs11Logger.Tests.BasicPkcs11LoggerTests.EnableStdErrTest
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_STDERR;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
                pkcs11Library.GetInfo();
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            {
                pkcs11Library.GetInfo();

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
                    ClassicAssert.IsTrue(ex is IOException);
                }

                // Check whether log file was not deleted
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");
            }

            // Delete log file
            if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                File.Delete(Settings.Pkcs11LoggerLogPath1);

            // Log to Pkcs11LoggerLogPath1 with fclose enabled
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            {
                pkcs11Library.GetInfo();

                // Check whether log file exists
                if (!File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " does not exist");

                // It should be possible to delete log file
                File.Delete(Settings.Pkcs11LoggerLogPath1);

                // Check whether log file was deleted
                if (File.Exists(Settings.Pkcs11LoggerLogPath1))
                    Assert.Fail("File " + Settings.Pkcs11LoggerLogPath1 + " exists");

                pkcs11Library.GetInfo();

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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags & ~PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            {
                int tickCountStart = Environment.TickCount;

                for (int i = 0; i < 10; i++)
                {
                    pkcs11Library.GetInfo();
                    foreach (ISlot slot in pkcs11Library.GetSlotList(SlotsType.WithTokenPresent))
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
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LIBRARY_PATH, Settings.Pkcs11LibraryPath);
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_LOG_FILE_PATH, Settings.Pkcs11LoggerLogPath1);
            flags = flags | PKCS11_LOGGER_FLAG_ENABLE_FCLOSE;
            EnvVarUtils.SetEnvVar(PKCS11_LOGGER_FLAGS, Convert.ToString(flags));
            using (IPkcs11Library pkcs11Library = Settings.Pkcs11InteropFactories.Pkcs11LibraryFactory.LoadPkcs11Library(Settings.Pkcs11InteropFactories, Settings.Pkcs11LoggerLibraryPath, AppType.MultiThreaded))
            {
                int tickCountStart = Environment.TickCount;

                for (int i = 0; i < 10; i++)
                {
                    pkcs11Library.GetInfo();
                    foreach (ISlot slot in pkcs11Library.GetSlotList(SlotsType.WithTokenPresent))
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
            ClassicAssert.IsTrue(fcloseEnabledTicks > fcloseDisabledTicks);
        }
    }
}
