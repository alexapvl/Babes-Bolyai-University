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

        private void testConnectionButtonClick(object sender, EventArgs e)
        {
            TestConnection();
        }
        private void dataGridTeamsSelectionChanged(object sender, EventArgs e)
        {
            using(SqlConnection conn = new SqlConnection(connectionString))
            {
                conn.Open();
                
                if (dataGridTeams.CurrentRow != null)
                {
                    int teamId = Convert.ToInt32(dataGridTeams.CurrentRow.Cells["teamId"].Value);
                    SqlCommand cmd = new SqlCommand("select * from driver where teamId=@teamId", conn);
                    cmd.Parameters.AddWithValue("@teamId", teamId);

                    SqlDataAdapter adapter = new SqlDataAdapter(cmd);
                    DataSet dataSet = new DataSet();
                    adapter.Fill(dataSet, "driver");
                    dataGridDrivers.DataSource = dataSet.Tables["driver"];
                }

                conn.Close();
            }
        }

        private void LoadAllTeams() {
            using (SqlConnection conn = new SqlConnection(connectionString)) {
                conn.Open();
                SqlCommand cmd = new SqlCommand("SELECT * FROM team", conn);

                // SqlDataReader reader = cmd.ExecuteReader();
                // while (reader.Read())
                // {
                //     Console.WriteLine($"Driver ID: {reader["driverId"]}, Name: {reader["driverName"]}");
                // }
                // reader.Close();

                SqlDataAdapter daChild = new SqlDataAdapter(cmd);
                DataSet ds = new DataSet();

                daChild.Fill(ds, "team");
                dataGridTeams.DataSource = ds.Tables["team"];
                conn.Close();
            }
        }

        private void TestConnection()
        {
            try
            {
                SqlConnection connection = new SqlConnection(connectionString);
                if (connection.State == System.Data.ConnectionState.Closed)
                    connection.Open();

                MessageBox.Show("Test connection succeded.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadTeamsButtonClick(object sender, EventArgs e)
        {
            LoadAllTeams();
        }


    }
}
