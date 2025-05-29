using System;
using System.Data;
using System.Data.SqlClient;
using System.Windows.Forms;

namespace LabTest
{
    public partial class Form1: Form
    {
        SqlConnection conn = new SqlConnection("Data Source=DESKTOP-55MRTN0;Initial Catalog=MiniFacebook;Integrated Security = true");
        SqlDataAdapter usersAdapter, postsAdapter; // adapters for each of the tables
        DataSet dataSet;
        BindingSource usersBS, postsBS; // binding sources for each of the tables
        SqlCommandBuilder sqlCommandBuilder; // Dark magic stuff (do not delete)
        public Form1() 
        {
            InitializeComponent();
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            // Initialize the DataSet
            dataSet = new DataSet();

            // Initialize the adapters for each of the tables
            usersAdapter = new SqlDataAdapter("select * from Users", conn);
            postsAdapter = new SqlDataAdapter("select * from Posts", conn);

            // Initialize the SqlCommandBuilder
            sqlCommandBuilder = new SqlCommandBuilder(postsAdapter); // watch this adapter for changes

            // Fill the DataSet with the data from the tables
            usersAdapter.Fill(dataSet,"Users");
            postsAdapter.Fill(dataSet, "Posts");

            // Initialize the BindingSources
            usersBS = new BindingSource();
            postsBS = new BindingSource();

            // this actually does the relationship binding, creates a foreign key constraint
            DataRelation DR = new DataRelation("FK_name_users_posts", dataSet.Tables["Users"].Columns["UserId"], dataSet.Tables["Posts"].Columns["UserId"]);

            // Add the DataRelation to the DataSet
            dataSet.Relations.Add(DR);

            // Set the DataSource and DataMember for the BindingSources
            usersBS.DataSource = dataSet;
            usersBS.DataMember = "Users";

            postsBS.DataSource = usersBS;
            postsBS.DataMember = "FK_name_users_posts";

            // Set the DataSource and DataMember for the DataGridView
            dgvUsers.DataSource = usersBS;
            dgvPosts.DataSource= postsBS;
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            try
            {
                // Update the child table
                postsAdapter.Update(dataSet, "Posts"); // the sql command builder will automatically generate the sql command (see line 36)
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
