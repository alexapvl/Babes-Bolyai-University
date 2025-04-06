namespace Setup_DBMS
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.dataGridConstructors = new System.Windows.Forms.DataGridView();
            this.dataGridTeams = new System.Windows.Forms.DataGridView();
            this.teamNameTextBox = new System.Windows.Forms.TextBox();
            this.teamCountryTextBox = new System.Windows.Forms.TextBox();
            this.testConnectionButton = new System.Windows.Forms.Button();
            this.LoadConstructorsButton = new System.Windows.Forms.Button();
            this.AddTeamButton = new System.Windows.Forms.Button();
            this.DeleteTeamButton = new System.Windows.Forms.Button();
            this.UpdateTeamButton = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridConstructors)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridTeams)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridConstructors
            // 
            this.dataGridConstructors.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridConstructors.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridConstructors.Location = new System.Drawing.Point(12, 173);
            this.dataGridConstructors.Name = "dataGridConstructors";
            this.dataGridConstructors.RowHeadersWidth = 62;
            this.dataGridConstructors.RowTemplate.Height = 28;
            this.dataGridConstructors.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridConstructors.Size = new System.Drawing.Size(1061, 979);
            this.dataGridConstructors.TabIndex = 0;
            this.dataGridConstructors.SelectionChanged += new System.EventHandler(this.dataGridConstructorsSelectionChanged);
            // 
            // dataGridTeams
            // 
            this.dataGridTeams.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridTeams.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridTeams.Location = new System.Drawing.Point(1122, 173);
            this.dataGridTeams.Name = "dataGridTeams";
            this.dataGridTeams.RowHeadersWidth = 62;
            this.dataGridTeams.RowTemplate.Height = 28;
            this.dataGridTeams.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridTeams.Size = new System.Drawing.Size(997, 979);
            this.dataGridTeams.TabIndex = 1;
            // 
            // teamNameTextBox
            // 
            this.teamNameTextBox.Location = new System.Drawing.Point(971, 21);
            this.teamNameTextBox.Name = "teamNameTextBox";
            this.teamNameTextBox.Size = new System.Drawing.Size(222, 26);
            this.teamNameTextBox.TabIndex = 2;
            // 
            // teamCountryTextBox
            // 
            this.teamCountryTextBox.Location = new System.Drawing.Point(971, 67);
            this.teamCountryTextBox.Name = "teamCountryTextBox";
            this.teamCountryTextBox.Size = new System.Drawing.Size(102, 26);
            this.teamCountryTextBox.TabIndex = 3;
            // 
            // testConnectionButton
            // 
            this.testConnectionButton.Location = new System.Drawing.Point(12, 12);
            this.testConnectionButton.Name = "testConnectionButton";
            this.testConnectionButton.Size = new System.Drawing.Size(294, 118);
            this.testConnectionButton.TabIndex = 4;
            this.testConnectionButton.Text = "Test Connection";
            this.testConnectionButton.UseVisualStyleBackColor = true;
            this.testConnectionButton.Click += new System.EventHandler(this.testConnectionButton_Click);
            // 
            // LoadConstructorsButton
            // 
            this.LoadConstructorsButton.Location = new System.Drawing.Point(327, 12);
            this.LoadConstructorsButton.Name = "LoadConstructorsButton";
            this.LoadConstructorsButton.Size = new System.Drawing.Size(294, 116);
            this.LoadConstructorsButton.TabIndex = 5;
            this.LoadConstructorsButton.Text = "Load Constructors";
            this.LoadConstructorsButton.UseVisualStyleBackColor = true;
            this.LoadConstructorsButton.Click += new System.EventHandler(this.LoadConstructorsButton_Click);
            // 
            // AddTeamButton
            // 
            this.AddTeamButton.Location = new System.Drawing.Point(1213, 12);
            this.AddTeamButton.Name = "AddTeamButton";
            this.AddTeamButton.Size = new System.Drawing.Size(294, 116);
            this.AddTeamButton.TabIndex = 6;
            this.AddTeamButton.Text = "Add Record in Child Table";
            this.AddTeamButton.UseVisualStyleBackColor = true;
            this.AddTeamButton.Click += new System.EventHandler(this.AddTeamButton_Click);
            // 
            // DeleteTeamButton
            // 
            this.DeleteTeamButton.Location = new System.Drawing.Point(1531, 12);
            this.DeleteTeamButton.Name = "DeleteTeamButton";
            this.DeleteTeamButton.Size = new System.Drawing.Size(294, 118);
            this.DeleteTeamButton.TabIndex = 7;
            this.DeleteTeamButton.Text = "Delete Selected Child Row";
            this.DeleteTeamButton.UseVisualStyleBackColor = true;
            this.DeleteTeamButton.Click += new System.EventHandler(this.DeleteTeamButton_Click);
            // 
            // UpdateTeamButton
            // 
            this.UpdateTeamButton.Location = new System.Drawing.Point(1855, 12);
            this.UpdateTeamButton.Name = "UpdateTeamButton";
            this.UpdateTeamButton.Size = new System.Drawing.Size(275, 116);
            this.UpdateTeamButton.TabIndex = 8;
            this.UpdateTeamButton.Text = "Update Selected Child Row";
            this.UpdateTeamButton.UseVisualStyleBackColor = true;
            this.UpdateTeamButton.Click += new System.EventHandler(this.UpdateTeamButton_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(844, 21);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 26);
            this.textBox1.TabIndex = 9;
            this.textBox1.Text = "Team Name:";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(827, 67);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(117, 26);
            this.textBox2.TabIndex = 10;
            this.textBox2.Text = "Team Country:";
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(2131, 1307);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.UpdateTeamButton);
            this.Controls.Add(this.DeleteTeamButton);
            this.Controls.Add(this.AddTeamButton);
            this.Controls.Add(this.LoadConstructorsButton);
            this.Controls.Add(this.testConnectionButton);
            this.Controls.Add(this.teamCountryTextBox);
            this.Controls.Add(this.teamNameTextBox);
            this.Controls.Add(this.dataGridTeams);
            this.Controls.Add(this.dataGridConstructors);
            this.Name = "Form2";
            this.Text = "Form2";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridConstructors)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridTeams)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridConstructors;
        private System.Windows.Forms.DataGridView dataGridTeams;
        private System.Windows.Forms.TextBox teamNameTextBox;
        private System.Windows.Forms.TextBox teamCountryTextBox;
        private System.Windows.Forms.Button testConnectionButton;
        private System.Windows.Forms.Button LoadConstructorsButton;
        private System.Windows.Forms.Button AddTeamButton;
        private System.Windows.Forms.Button DeleteTeamButton;
        private System.Windows.Forms.Button UpdateTeamButton;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
    }
}