using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Setup_DBMS
{
    public partial class Form1: Form
    {
        string connectionString = @"Data Source=DESKTOP-55MRTN0;Initial Catalog=f1;Integrated Security=True;Encrypt=False;TrustServerCertificate=True";

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                SqlConnection connection = new SqlConnection(connectionString);
                if (connection.State == System.Data.ConnectionState.Closed)
                    connection.Open();

                MessageBox.Show("Test connection succeded.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            } catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            
        }
    }
}
