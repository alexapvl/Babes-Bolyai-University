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
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace Setup_DBMS
{
    public partial class Form1: Form
    {
        string connectionString = @"Data Source=DESKTOP-55MRTN0;Initial Catalog=f1;Integrated Security=True;Encrypt=False;TrustServerCertificate=True";
        SqlDataAdapter teamsAdapter, driversAdapter;
        DataSet teamsDataSet = new DataSet();
        DataSet driversDataSet = new DataSet();

        int? selectedTeamId = null;

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
                    this.selectedTeamId = Convert.ToInt32(dataGridTeams.CurrentRow.Cells["teamId"].Value);
                    LoadDriversBySelectedTeamId();
                }
            }
        }

        private void LoadDriversBySelectedTeamId()
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if(this.selectedTeamId == -1 || this.selectedTeamId == null)
                {
                    MessageBox.Show("Please select a team first.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                SqlCommand cmd = new SqlCommand("SELECT driverId, driverName, teamId, nationality, dateOfBirth FROM driver WHERE teamId = @teamId", conn);
                cmd.Parameters.AddWithValue("@teamId", this.selectedTeamId);

                this.driversAdapter = new SqlDataAdapter(cmd);
                this.driversDataSet.Clear();
                this.driversAdapter.Fill(this.driversDataSet, "driver");
                dataGridDrivers.DataSource = this.driversDataSet.Tables["driver"];

                // Format the dateOfBirth column
                if (dataGridDrivers.Columns["dateOfBirth"] != null)
                {
                    dataGridDrivers.Columns["dateOfBirth"].DefaultCellStyle.Format = "dd/MM/yyyy"; // the date in the database is not formatted as dd/MM/yyyy, so we need to format it
                }
            }
        }

        private void LoadAllTeams() {
            using (SqlConnection conn = new SqlConnection(connectionString)) {
                conn.Open();
                SqlCommand cmd = new SqlCommand("SELECT * FROM team", conn);

                this.teamsAdapter = new SqlDataAdapter(cmd);
                this.teamsDataSet.Clear();

                this.teamsAdapter.Fill(this.teamsDataSet, "team");
                dataGridTeams.DataSource = this.teamsDataSet.Tables["team"];
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

        private void AddRecordInChildTableButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if (this.selectedTeamId == -1 || this.selectedTeamId == null)
                {
                    MessageBox.Show("Please select a team in order to add a driver first.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                SqlCommand cmd = new SqlCommand("INSERT INTO driver (driverName, teamId, nationality, dateOfBirth) VALUES (@driverName, @teamId, @nationality, @dateOfBirth)", conn);

                // take params from the textboxes and dateTimePicker
                string driverName = driverNameTextBox.Text;
                string driverNationality = driverNationalityTextBox.Text;
                string driverDateOfBirth = driverDoBDateTimePicker.Value.ToString("MM/dd/yyyy"); // this is how the date is formatted in the database

                if (driverName == "" || driverNationality == "" || driverDateOfBirth == "")
                {
                    MessageBox.Show("Please fill all the fields.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                cmd.Parameters.AddWithValue("@driverName", driverName);
                cmd.Parameters.AddWithValue("@teamId", this.selectedTeamId);
                cmd.Parameters.AddWithValue("@nationality", driverNationality);
                cmd.Parameters.AddWithValue("@dateOfBirth", driverDateOfBirth);
                conn.Open();
                try {
                    cmd.ExecuteNonQuery();
                } catch (Exception ex) {
                    MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                finally {
                    conn.Close();
                }
                MessageBox.Show("Driver added successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            driverNameTextBox.Clear();
            driverNationalityTextBox.Clear();
            driverDoBDateTimePicker.Value = DateTime.Now;
            LoadDriversBySelectedTeamId();
        }

        private void DeleteChildRowButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if (dataGridDrivers.SelectedRows.Count > 0)
                {
                    // Ask for confirmation before deleting
                    DialogResult result = MessageBox.Show("Are you sure you want to delete this driver?", 
                        "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    
                    if (result == DialogResult.Yes)
                    {
                        int driverId = Convert.ToInt32(dataGridDrivers.SelectedRows[0].Cells["driverId"].Value);
                        SqlCommand cmd = new SqlCommand("DELETE FROM driver WHERE driverId = @driverId", conn);
                        cmd.Parameters.AddWithValue("@driverId", driverId);
                        try 
                        {
                            conn.Open();
                            cmd.ExecuteNonQuery();
                        } 
                        catch (Exception ex) 
                        {
                            MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
                            return;
                        }
                        finally 
                        {
                            conn.Close();
                        }
                        MessageBox.Show("Driver deleted successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else 
                {
                    MessageBox.Show("Please select a driver to delete.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            LoadDriversBySelectedTeamId();
        }

        private void UpdateChildTableButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                // Get the selected row from the drivers grid
                if (dataGridDrivers.SelectedRows.Count > 0)
                {
                    // Get the ID of the selected driver
                    int driverId = Convert.ToInt32(dataGridDrivers.SelectedRows[0].Cells["driverId"].Value);
                    
                    // Get the updated values from the selected row
                    string driverName = dataGridDrivers.SelectedRows[0].Cells["driverName"].Value.ToString();
                    int teamId = Convert.ToInt32(dataGridDrivers.SelectedRows[0].Cells["teamId"].Value);
                    string nationality = dataGridDrivers.SelectedRows[0].Cells["nationality"].Value.ToString();
                    string dateOfBirth = dataGridDrivers.SelectedRows[0].Cells["dateOfBirth"].Value.ToString();

                    SqlCommand cmd = new SqlCommand("UPDATE driver SET driverName = @driverName, teamId = @teamId, nationality = @nationality, dateOfBirth = @dateOfBirth WHERE driverId = @driverId", conn);
                    
                    cmd.Parameters.AddWithValue("@driverName", driverName);
                    cmd.Parameters.AddWithValue("@teamId", teamId);
                    cmd.Parameters.AddWithValue("@nationality", nationality);
                    cmd.Parameters.AddWithValue("@dateOfBirth", dateOfBirth);
                    cmd.Parameters.AddWithValue("@driverId", driverId);
                    
                    conn.Open();
                    int rowsAffected = 0;

                    try 
                    {
                        rowsAffected = cmd.ExecuteNonQuery();
                    } 
                    catch (Exception ex) 
                    {
                        if (ex.Message.Contains("FK_referencing_table_team"))
                        {
                            MessageBox.Show("You cannot update the team of a driver to a team that does not exist.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                            return;
                        }
                        MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                    finally 
                    {
                        conn.Close();
                    }

                    if (rowsAffected > 0)
                    {
                        MessageBox.Show("Driver updated successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        MessageBox.Show("No changes were made.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    MessageBox.Show("Please select a driver to update.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            LoadDriversBySelectedTeamId();
        }
    }
}
