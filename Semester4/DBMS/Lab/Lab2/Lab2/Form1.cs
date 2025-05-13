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

namespace Lab2
{
    public partial class Form1: Form
    {
        // Configuration
        private string connectionString = AppConfig.GetConnectionString("MyLocalDb");
        private TableConfig parentTableConfig;
        private ChildTableConfig childTableConfig;

        // Data adapters and datasets
        private SqlDataAdapter parentAdapter, childAdapter;
        private DataSet parentDataSet = new DataSet();
        private DataSet childDataSet = new DataSet();

        // UI Controls
        private Button testConnectionButton;
        private Button loadParentDataButton;
        private DataGridView parentDataGrid;
        private DataGridView childDataGrid;
        private Button addChildRecordButton;
        private Button deleteChildRecordButton;
        private Button updateChildRecordButton;
        private List<Control> inputControls = new List<Control>();
        private List<Label> inputLabels = new List<Label>();

        // Selected parent ID
        private int? selectedParentId = null;

        public Form1()
        {
            InitializeComponent();
            LoadConfiguration();
            InitializeUI();
        }

        private void LoadConfiguration()
        {
            try
            {
                parentTableConfig = AppConfig.GetParentTableConfig();
                childTableConfig = AppConfig.GetChildTableConfig();
                
                // Debug output to verify configuration
                Debug.WriteLine($"Parent Table: {parentTableConfig.Name}, ID Column: {parentTableConfig.IdColumn}");
                Debug.WriteLine("Parent Display Columns: " + string.Join(", ", parentTableConfig.DisplayColumns));
                
                Debug.WriteLine($"Child Table: {childTableConfig.Name}, ID Column: {childTableConfig.IdColumn}, Foreign Key: {childTableConfig.ForeignKeyColumn}");
                Debug.WriteLine("Child Display Columns: " + string.Join(", ", childTableConfig.DisplayColumns));
                
                foreach (var field in childTableConfig.InputFields)
                {
                    Debug.WriteLine($"Input Field: {field.Name}, Label: {field.Label}, Type: {field.Type}");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading configuration: {ex.Message}", "Configuration Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void InitializeUI()
        {
            // Set form properties
            this.Text = $"Dynamic Parent-Child Table Form: {parentTableConfig.Name} - {childTableConfig.Name}";
            this.ClientSize = new Size(1200, 800);

            // Create test connection button
            testConnectionButton = new Button
            {
                Text = "Test Connection",
                Location = new Point(20, 20),
                Size = new Size(150, 40)
            };
            testConnectionButton.Click += TestConnectionButtonClick;
            this.Controls.Add(testConnectionButton);

            // Create load parent data button
            loadParentDataButton = new Button
            {
                Text = $"Load {parentTableConfig.Name}",
                Location = new Point(190, 20),
                Size = new Size(150, 40)
            };
            loadParentDataButton.Click += LoadParentDataButtonClick;
            this.Controls.Add(loadParentDataButton);

            // Create parent data grid
            parentDataGrid = new DataGridView
            {
                Location = new Point(20, 80),
                Size = new Size(500, 400),
                AllowUserToAddRows = false,
                AllowUserToDeleteRows = false,
                ReadOnly = true,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect,
                MultiSelect = false,
                AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill
            };
            parentDataGrid.SelectionChanged += ParentDataGridSelectionChanged;
            this.Controls.Add(parentDataGrid);

            // Create child data grid
            childDataGrid = new DataGridView
            {
                Location = new Point(540, 80),
                Size = new Size(500, 400),
                AllowUserToAddRows = false,
                AllowUserToDeleteRows = false,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect,
                MultiSelect = false,
                AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill
            };
            this.Controls.Add(childDataGrid);

            // Create input fields for child table
            int startX = 20;
            int startY = 500;
            int labelWidth = 150;
            int controlWidth = 200;
            int rowHeight = 30;

            foreach (var field in childTableConfig.InputFields)
            {
                // Create label
                Label label = new Label
                {
                    Text = field.Label,
                    Location = new Point(startX, startY),
                    Size = new Size(labelWidth, 20),
                    AutoSize = true
                };
                this.Controls.Add(label);
                inputLabels.Add(label);

                // Create input control based on type
                Control inputControl = null;
                switch (field.Type.ToLower())
                {
                    case "textbox":
                        inputControl = new TextBox
                        {
                            Name = field.Name,
                            Location = new Point(startX + labelWidth + 10, startY),
                            Size = new Size(controlWidth, 20)
                        };
                        break;
                    case "datetimepicker":
                        inputControl = new DateTimePicker
                        {
                            Name = field.Name,
                            Location = new Point(startX + labelWidth + 10, startY),
                            Size = new Size(controlWidth, 20),
                            Format = DateTimePickerFormat.Custom,
                            CustomFormat = "dd/MM/yyyy"
                        };
                        break;
                    // Add more control types as needed
                    default:
                        inputControl = new TextBox
                        {
                            Name = field.Name,
                            Location = new Point(startX + labelWidth + 10, startY),
                            Size = new Size(controlWidth, 20)
                        };
                        break;
                }
                this.Controls.Add(inputControl);
                inputControls.Add(inputControl);

                startY += rowHeight;
            }

            // Create buttons for child table operations
            addChildRecordButton = new Button
            {
                Text = $"Add {childTableConfig.Name}",
                Location = new Point(540, 500),
                Size = new Size(150, 40)
            };
            addChildRecordButton.Click += AddChildRecordButtonClick;
            this.Controls.Add(addChildRecordButton);

            deleteChildRecordButton = new Button
            {
                Text = $"Delete {childTableConfig.Name}",
                Location = new Point(700, 500),
                Size = new Size(150, 40)
            };
            deleteChildRecordButton.Click += DeleteChildRecordButtonClick;
            this.Controls.Add(deleteChildRecordButton);

            updateChildRecordButton = new Button
            {
                Text = $"Update {childTableConfig.Name}",
                Location = new Point(860, 500),
                Size = new Size(150, 40)
            };
            updateChildRecordButton.Click += UpdateChildRecordButtonClick;
            this.Controls.Add(updateChildRecordButton);
        }

        private void TestConnectionButtonClick(object sender, EventArgs e)
        {
            TestConnection();
        }

        private void LoadParentDataButtonClick(object sender, EventArgs e)
        {
            LoadParentData();
        }

        private void ParentDataGridSelectionChanged(object sender, EventArgs e)
        {
            if (parentDataGrid.CurrentRow != null)
            {
                selectedParentId = Convert.ToInt32(parentDataGrid.CurrentRow.Cells[parentTableConfig.IdColumn].Value);
                LoadChildData();
            }
        }

        private void AddChildRecordButtonClick(object sender, EventArgs e)
        {
            AddChildRecord();
        }

        private void DeleteChildRecordButtonClick(object sender, EventArgs e)
        {
            DeleteChildRecord();
        }

        private void UpdateChildRecordButtonClick(object sender, EventArgs e)
        {
            UpdateChildRecord();
        }

        private void TestConnection()
        {
            try
            {
                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();
                    MessageBox.Show("Connection successful!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Connection failed: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadParentData()
        {
            try
            {
                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();
                    string columns = string.Join(", ", parentTableConfig.DisplayColumns);
                    string query = $"SELECT {columns} FROM {parentTableConfig.Name}";
                    Debug.WriteLine("Query: " + query);
                    
                    parentAdapter = new SqlDataAdapter(query, connection);
                    parentDataSet.Clear();
                    parentAdapter.Fill(parentDataSet, parentTableConfig.Name);
                    
                    parentDataGrid.DataSource = parentDataSet.Tables[parentTableConfig.Name];
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading {parentTableConfig.Name}: {ex.Message}", "Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void LoadChildData()
        {
            if (selectedParentId == null)
            {
                MessageBox.Show($"Please select a {parentTableConfig.Name} first.", "Warning", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

            try
            {
                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();
                    string columns = string.Join(", ", childTableConfig.DisplayColumns);
                    string query = $"SELECT {columns} FROM {childTableConfig.Name} WHERE {childTableConfig.ForeignKeyColumn} = @parentId";
                    
                    SqlCommand cmd = new SqlCommand(query, connection);
                    cmd.Parameters.AddWithValue("@parentId", selectedParentId);
                    
                    childAdapter = new SqlDataAdapter(cmd);
                    childDataSet.Clear();
                    childAdapter.Fill(childDataSet, childTableConfig.Name);
                    
                    childDataGrid.DataSource = childDataSet.Tables[childTableConfig.Name];

                    // Format date columns if they exist
                    foreach (DataGridViewColumn column in childDataGrid.Columns)
                    {
                        if (column.ValueType == typeof(DateTime))
                        {
                            column.DefaultCellStyle.Format = "dd/MM/yyyy";
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading {childTableConfig.Name}: {ex.Message}", "Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void AddChildRecord()
        {
            if (selectedParentId == null)
            {
                MessageBox.Show($"Please select a {parentTableConfig.Name} first.", "Warning", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                // Validate input fields
                foreach (var control in inputControls)
                {
                    if (control is TextBox textBox && string.IsNullOrWhiteSpace(textBox.Text))
                    {
                        MessageBox.Show($"Please fill in all required fields.", "Validation Error", 
                            MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        return;
                    }
                }

                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();

                    // Build the INSERT query dynamically
                    StringBuilder columnList = new StringBuilder();
                    StringBuilder paramList = new StringBuilder();
                    List<SqlParameter> parameters = new List<SqlParameter>();

                    // Add foreign key parameter
                    columnList.Append(childTableConfig.ForeignKeyColumn);
                    paramList.Append("@parentId");
                    parameters.Add(new SqlParameter("@parentId", selectedParentId));

                    // Add input field parameters
                    foreach (var control in inputControls)
                    {
                        if (columnList.Length > 0)
                        {
                            columnList.Append(", ");
                            paramList.Append(", ");
                        }

                        columnList.Append(control.Name);
                        paramList.Append($"@{control.Name}");

                        object value = null;
                        if (control is TextBox textBox)
                        {
                            value = textBox.Text;
                        }
                        else if (control is DateTimePicker dateTimePicker)
                        {
                            value = dateTimePicker.Value.ToString("yyyy-MM-dd");
                        }

                        parameters.Add(new SqlParameter($"@{control.Name}", value));
                    }

                    string query = $"INSERT INTO {childTableConfig.Name} ({columnList}) VALUES ({paramList})";
                    SqlCommand cmd = new SqlCommand(query, connection);
                    
                    foreach (var param in parameters)
                    {
                        cmd.Parameters.Add(param);
                    }

                            cmd.ExecuteNonQuery();
                    MessageBox.Show($"{childTableConfig.Name} added successfully!", "Success", 
                        MessageBoxButtons.OK, MessageBoxIcon.Information);

                    // Clear input fields
                    foreach (var control in inputControls)
                    {
                        if (control is TextBox textBox)
                        {
                            textBox.Clear();
                        }
                        else if (control is DateTimePicker dateTimePicker)
                        {
                            dateTimePicker.Value = DateTime.Now;
                        }
                    }

                    // Reload child data
                    LoadChildData();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error adding {childTableConfig.Name}: {ex.Message}", "Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void DeleteChildRecord()
        {
            if (childDataGrid.SelectedRows.Count == 0)
            {
                MessageBox.Show($"Please select a {childTableConfig.Name} to delete.", "Warning", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            DialogResult result = MessageBox.Show($"Are you sure you want to delete this {childTableConfig.Name}?", 
                "Confirmation", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            
            if (result == DialogResult.Yes)
            {
                try
                {
                    using (SqlConnection connection = new SqlConnection(connectionString))
                    {
                        connection.Open();
                        int childId = Convert.ToInt32(childDataGrid.SelectedRows[0].Cells[childTableConfig.IdColumn].Value);
                        
                        string query = $"DELETE FROM {childTableConfig.Name} WHERE {childTableConfig.IdColumn} = @childId";
                        SqlCommand cmd = new SqlCommand(query, connection);
                        cmd.Parameters.AddWithValue("@childId", childId);
                        
                        cmd.ExecuteNonQuery();
                        MessageBox.Show($"{childTableConfig.Name} deleted successfully!", "Success", 
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        
                        // Reload child data
                        LoadChildData();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error deleting {childTableConfig.Name}: {ex.Message}", "Error", 
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void UpdateChildRecord()
        {
            if (childDataGrid.SelectedRows.Count == 0)
            {
                MessageBox.Show($"Please select a {childTableConfig.Name} to update.", "Warning", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            try
            {
                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();
                    int childId = Convert.ToInt32(childDataGrid.SelectedRows[0].Cells[childTableConfig.IdColumn].Value);
                    
                    // Build the UPDATE query dynamically
                    StringBuilder setClause = new StringBuilder();
                    List<SqlParameter> parameters = new List<SqlParameter>();

                    // Get the selected row
                    DataGridViewRow selectedRow = childDataGrid.SelectedRows[0];
                    
                    // Add parameters for each column in the child table (except the ID column)
                    foreach (DataGridViewCell cell in selectedRow.Cells)
                    {
                        // Skip the ID column as we don't want to update it
                        if (cell.OwningColumn.Name == childTableConfig.IdColumn)
                            continue;
                            
                        if (setClause.Length > 0)
                        {
                            setClause.Append(", ");
                        }

                        setClause.Append($"{cell.OwningColumn.Name} = @{cell.OwningColumn.Name}");
                        
                        // Get the value from the cell
                        object value = cell.Value;
                        
                        // Handle null values
                        if (value == DBNull.Value)
                        {
                            value = DBNull.Value;
                        }
                        
                        parameters.Add(new SqlParameter($"@{cell.OwningColumn.Name}", value));
                    }

                    string query = $"UPDATE {childTableConfig.Name} SET {setClause} WHERE {childTableConfig.IdColumn} = @childId";
                    SqlCommand cmd = new SqlCommand(query, connection);
                    
                    foreach (var param in parameters)
                    {
                        cmd.Parameters.Add(param);
                    }
                    
                    cmd.Parameters.AddWithValue("@childId", childId);
                    
                    int rowsAffected = cmd.ExecuteNonQuery();
                    if (rowsAffected > 0)
                    {
                        MessageBox.Show($"{childTableConfig.Name} updated successfully!", "Success", 
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        MessageBox.Show($"No changes were made to {childTableConfig.Name}.", "Information", 
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    
                    // Reload child data
                    LoadChildData();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error updating {childTableConfig.Name}: {ex.Message}", "Error", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
