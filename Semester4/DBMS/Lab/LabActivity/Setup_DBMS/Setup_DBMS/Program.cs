using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Setup_DBMS
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            string configurationType = AppConfig.GetConnectionString("ConfigurationType");
            if (configurationType == "one")
            {
                Application.Run(new Form1());
            }
            else if (configurationType == "two")
            {
                Application.Run(new Form2());
            }
            else
            {
                MessageBox.Show("Invalid configuration type in appsettings.json");
            }

        }
    }
}
