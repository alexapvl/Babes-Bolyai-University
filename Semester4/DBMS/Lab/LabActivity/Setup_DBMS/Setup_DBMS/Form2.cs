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
    public partial class Form2: Form
    {
        string connectionString = AppConfig.GetConnectionString("MyLocalDb");
        SqlDataAdapter constructorsAdapter, teamsAdapter;
        DataSet constructorsDataSet = new DataSet();
        DataSet teamsDataSet = new DataSet();

        int? selectedConstructorId = null;

        public Form2()
        {
            InitializeComponent();
        }

        private void testConnectionButton_Click(object sender, EventArgs e)
        {
            TestConnection();
        }

        private void dataGridConstructorsSelectionChanged(object sender, EventArgs e)
        {
            using(SqlConnection conn = new SqlConnection(connectionString))
            {
                conn.Open();
                
                if (dataGridConstructors.CurrentRow != null)
                {
                    this.selectedConstructorId = Convert.ToInt32(dataGridConstructors.CurrentRow.Cells["constructorId"].Value);
                    LoadTeamsBySelectedConstructorId();
                }
            }
        }

        private void LoadTeamsBySelectedConstructorId()
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if(this.selectedConstructorId == -1 || this.selectedConstructorId == null)
                {
                    MessageBox.Show("Please select a constructor first.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                SqlCommand cmd = new SqlCommand("SELECT teamId, constructorId, teamName, teamCountry FROM team WHERE constructorId = @constructorId", conn);
                cmd.Parameters.AddWithValue("@constructorId", this.selectedConstructorId);

                this.teamsAdapter = new SqlDataAdapter(cmd);
                this.teamsDataSet.Clear();
                this.teamsAdapter.Fill(this.teamsDataSet, "team");
                dataGridTeams.DataSource = this.teamsDataSet.Tables["team"];
            }
        }

        private void LoadAllConstructors() {
            using (SqlConnection conn = new SqlConnection(connectionString)) {
                conn.Open();
                SqlCommand cmd = new SqlCommand("SELECT * FROM constructor", conn);

                this.constructorsAdapter = new SqlDataAdapter(cmd);
                this.constructorsDataSet.Clear();

                this.constructorsAdapter.Fill(this.constructorsDataSet, "constructor");
                dataGridConstructors.DataSource = this.constructorsDataSet.Tables["constructor"];
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

                MessageBox.Show("Test connection succeeded.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadConstructorsButton_Click(object sender, EventArgs e)
        {
            LoadAllConstructors();
        }

        private int GetNextTeamId()
        {
            int nextId = 1; // Default starting ID if table is empty
            
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                SqlCommand cmd = new SqlCommand("SELECT TOP 1 teamId FROM team ORDER BY teamId DESC", conn);
                
                conn.Open();
                try
                {
                    // Execute the query and get the result
                    object result = cmd.ExecuteScalar();
                    
                    // If we got a result, increment it for the next ID
                    if (result != null && result != DBNull.Value)
                    {
                        nextId = Convert.ToInt32(result) + 1;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error getting next team ID: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                finally
                {
                    conn.Close();
                }
            }
            
            return nextId;
        }

        private void AddTeamButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if (this.selectedConstructorId == -1 || this.selectedConstructorId == null)
                {
                    MessageBox.Show("Please select a constructor first.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                // Get the next team ID
                int nextTeamId = GetNextTeamId();

                SqlCommand cmd = new SqlCommand("INSERT INTO team (teamId, teamName, constructorId, teamCountry) VALUES (@teamId, @teamName, @constructorId, @teamCountry)", conn);

                // Get params from textboxes
                string teamName = teamNameTextBox.Text;
                string teamCountry = teamCountryTextBox.Text;

                if (teamName == "" || teamCountry == "")
                {
                    MessageBox.Show("Please fill all the fields.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                cmd.Parameters.AddWithValue("@teamId", nextTeamId);
                cmd.Parameters.AddWithValue("@teamName", teamName);
                cmd.Parameters.AddWithValue("@constructorId", this.selectedConstructorId);
                cmd.Parameters.AddWithValue("@teamCountry", teamCountry);
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
                MessageBox.Show("Team added successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            teamNameTextBox.Clear();
            teamCountryTextBox.Clear();
            LoadTeamsBySelectedConstructorId();
        }

        private void DeleteTeamButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                if (dataGridTeams.SelectedRows.Count > 0)
                {
                    // Ask for confirmation before deleting
                    DialogResult result = MessageBox.Show("Are you sure you want to delete this team?", 
                        "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                    
                    if (result == DialogResult.Yes)
                    {
                        int teamId = Convert.ToInt32(dataGridTeams.SelectedRows[0].Cells["teamId"].Value);
                        SqlCommand cmd = new SqlCommand("DELETE FROM team WHERE teamId = @teamId", conn);
                        cmd.Parameters.AddWithValue("@teamId", teamId);
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
                        MessageBox.Show("Team deleted successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else 
                {
                    MessageBox.Show("Please select a team to delete.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            LoadTeamsBySelectedConstructorId();
        }

        private void UpdateTeamButton_Click(object sender, EventArgs e)
        {
            using (SqlConnection conn = new SqlConnection(connectionString))
            {
                // Get the selected row from the teams grid
                if (dataGridTeams.SelectedRows.Count > 0)
                {
                    // Get the ID of the selected team
                    int teamId = Convert.ToInt32(dataGridTeams.SelectedRows[0].Cells["teamId"].Value);
                    
                    // Get the updated values from the selected row
                    string teamName = dataGridTeams.SelectedRows[0].Cells["teamName"].Value.ToString();
                    int constructorId = Convert.ToInt32(dataGridTeams.SelectedRows[0].Cells["constructorId"].Value);
                    string teamCountry = dataGridTeams.SelectedRows[0].Cells["teamCountry"].Value.ToString();

                    SqlCommand cmd = new SqlCommand("UPDATE team SET teamName = @teamName, constructorId = @constructorId, teamCountry = @teamCountry WHERE teamId = @teamId", conn);
                    
                    cmd.Parameters.AddWithValue("@teamName", teamName);
                    cmd.Parameters.AddWithValue("@constructorId", constructorId);
                    cmd.Parameters.AddWithValue("@teamCountry", teamCountry);
                    cmd.Parameters.AddWithValue("@teamId", teamId);
                    
                    conn.Open();
                    int rowsAffected = 0;

                    try 
                    {
                        rowsAffected = cmd.ExecuteNonQuery();
                    } 
                    catch (Exception ex) 
                    {
                        if (ex.Message.Contains("FK_referencing_constructor"))
                        {
                            MessageBox.Show("You cannot update the constructor of a team to a constructor that does not exist.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
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
                        MessageBox.Show("Team updated successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        MessageBox.Show("No changes were made.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    MessageBox.Show("Please select a team to update.", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }
            LoadTeamsBySelectedConstructorId();
        }
    }
}
