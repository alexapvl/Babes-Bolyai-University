namespace Setup_DBMS
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.testConnectionButton = new System.Windows.Forms.Button();
            this.f1DataSetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.f1DataSet = new Setup_DBMS.f1DataSet();
            this.dataGridTeams = new System.Windows.Forms.DataGridView();
            this.loadTeamsButton = new System.Windows.Forms.Button();
            this.dataGridDrivers = new System.Windows.Forms.DataGridView();
            this.UpdateChildTableButton = new System.Windows.Forms.Button();
            this.DeleteChildRowButton = new System.Windows.Forms.Button();
            this.AddRecordInChildTableButton = new System.Windows.Forms.Button();
            this.driverNameTextBox = new System.Windows.Forms.TextBox();
            this.driverNationalityTextBox = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.driverDoBDateTimePicker = new System.Windows.Forms.DateTimePicker();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridTeams)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridDrivers)).BeginInit();
            this.SuspendLayout();
            // 
            // testConnectionButton
            // 
            this.testConnectionButton.Location = new System.Drawing.Point(12, 12);
            this.testConnectionButton.Name = "testConnectionButton";
            this.testConnectionButton.Size = new System.Drawing.Size(210, 125);
            this.testConnectionButton.TabIndex = 0;
            this.testConnectionButton.Text = "Tess Connection";
            this.testConnectionButton.UseVisualStyleBackColor = true;
            this.testConnectionButton.Click += new System.EventHandler(this.testConnectionButtonClick);
            // 
            // f1DataSetBindingSource
            // 
            this.f1DataSetBindingSource.DataSource = this.f1DataSet;
            this.f1DataSetBindingSource.Position = 0;
            // 
            // f1DataSet
            // 
            this.f1DataSet.DataSetName = "f1DataSet";
            this.f1DataSet.SchemaSerializationMode = System.Data.SchemaSerializationMode.IncludeSchema;
            // 
            // dataGridTeams
            // 
            this.dataGridTeams.AllowUserToAddRows = false;
            this.dataGridTeams.AllowUserToDeleteRows = false;
            this.dataGridTeams.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridTeams.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridTeams.Location = new System.Drawing.Point(12, 181);
            this.dataGridTeams.Name = "dataGridTeams";
            this.dataGridTeams.ReadOnly = true;
            this.dataGridTeams.RowHeadersWidth = 62;
            this.dataGridTeams.RowTemplate.Height = 28;
            this.dataGridTeams.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridTeams.Size = new System.Drawing.Size(1059, 1007);
            this.dataGridTeams.TabIndex = 1;
            this.dataGridTeams.SelectionChanged += new System.EventHandler(this.dataGridTeamsSelectionChanged);
            // 
            // loadTeamsButton
            // 
            this.loadTeamsButton.Location = new System.Drawing.Point(250, 12);
            this.loadTeamsButton.Name = "loadTeamsButton";
            this.loadTeamsButton.Size = new System.Drawing.Size(210, 125);
            this.loadTeamsButton.TabIndex = 2;
            this.loadTeamsButton.Text = "Load Teams";
            this.loadTeamsButton.UseVisualStyleBackColor = true;
            this.loadTeamsButton.Click += new System.EventHandler(this.LoadTeamsButtonClick);
            // 
            // dataGridDrivers
            // 
            this.dataGridDrivers.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridDrivers.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dataGridDrivers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridDrivers.Location = new System.Drawing.Point(1127, 181);
            this.dataGridDrivers.Name = "dataGridDrivers";
            this.dataGridDrivers.RowHeadersWidth = 62;
            this.dataGridDrivers.RowTemplate.Height = 28;
            this.dataGridDrivers.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridDrivers.Size = new System.Drawing.Size(1016, 1007);
            this.dataGridDrivers.TabIndex = 3;
            // 
            // UpdateChildTableButton
            // 
            this.UpdateChildTableButton.Location = new System.Drawing.Point(1933, 12);
            this.UpdateChildTableButton.Name = "UpdateChildTableButton";
            this.UpdateChildTableButton.Size = new System.Drawing.Size(210, 125);
            this.UpdateChildTableButton.TabIndex = 4;
            this.UpdateChildTableButton.Text = "Update Selected Child Row";
            this.UpdateChildTableButton.UseVisualStyleBackColor = true;
            this.UpdateChildTableButton.Click += new System.EventHandler(this.UpdateChildTableButton_Click);
            // 
            // DeleteChildRowButton
            // 
            this.DeleteChildRowButton.Location = new System.Drawing.Point(1673, 12);
            this.DeleteChildRowButton.Name = "DeleteChildRowButton";
            this.DeleteChildRowButton.Size = new System.Drawing.Size(210, 125);
            this.DeleteChildRowButton.TabIndex = 5;
            this.DeleteChildRowButton.Text = "Delete Selected Child Row";
            this.DeleteChildRowButton.UseVisualStyleBackColor = true;
            this.DeleteChildRowButton.Click += new System.EventHandler(this.DeleteChildRowButton_Click);
            // 
            // AddRecordInChildTableButton
            // 
            this.AddRecordInChildTableButton.Location = new System.Drawing.Point(1416, 12);
            this.AddRecordInChildTableButton.Name = "AddRecordInChildTableButton";
            this.AddRecordInChildTableButton.Size = new System.Drawing.Size(210, 125);
            this.AddRecordInChildTableButton.TabIndex = 6;
            this.AddRecordInChildTableButton.Text = "Add Record in Child Table";
            this.AddRecordInChildTableButton.UseVisualStyleBackColor = true;
            this.AddRecordInChildTableButton.Click += new System.EventHandler(this.AddRecordInChildTableButton_Click);
            // 
            // driverNameTextBox
            // 
            this.driverNameTextBox.Location = new System.Drawing.Point(1077, 12);
            this.driverNameTextBox.Name = "driverNameTextBox";
            this.driverNameTextBox.Size = new System.Drawing.Size(301, 26);
            this.driverNameTextBox.TabIndex = 7;
            // 
            // driverNationalityTextBox
            // 
            this.driverNationalityTextBox.Location = new System.Drawing.Point(1077, 61);
            this.driverNationalityTextBox.Name = "driverNationalityTextBox";
            this.driverNationalityTextBox.Size = new System.Drawing.Size(301, 26);
            this.driverNationalityTextBox.TabIndex = 8;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(928, 12);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(100, 26);
            this.textBox1.TabIndex = 10;
            this.textBox1.Text = "Driver name:";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(928, 61);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(131, 26);
            this.textBox2.TabIndex = 11;
            this.textBox2.Text = "Driver nationality:";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(928, 111);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(143, 26);
            this.textBox3.TabIndex = 12;
            this.textBox3.Text = "Driver date of birth:";
            // 
            // driverDoBDateTimePicker
            // 
            this.driverDoBDateTimePicker.Location = new System.Drawing.Point(1077, 111);
            this.driverDoBDateTimePicker.MinDate = new System.DateTime(1960, 1, 1, 0, 0, 0, 0);
            this.driverDoBDateTimePicker.Name = "driverDoBDateTimePicker";
            this.driverDoBDateTimePicker.Size = new System.Drawing.Size(301, 26);
            this.driverDoBDateTimePicker.TabIndex = 13;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(2166, 1286);
            this.Controls.Add(this.driverDoBDateTimePicker);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.driverNationalityTextBox);
            this.Controls.Add(this.driverNameTextBox);
            this.Controls.Add(this.AddRecordInChildTableButton);
            this.Controls.Add(this.DeleteChildRowButton);
            this.Controls.Add(this.UpdateChildTableButton);
            this.Controls.Add(this.dataGridDrivers);
            this.Controls.Add(this.loadTeamsButton);
            this.Controls.Add(this.dataGridTeams);
            this.Controls.Add(this.testConnectionButton);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridTeams)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridDrivers)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button testConnectionButton;
        private System.Windows.Forms.BindingSource f1DataSetBindingSource;
        private f1DataSet f1DataSet;
        private System.Windows.Forms.DataGridView dataGridTeams;
        private System.Windows.Forms.Button loadTeamsButton;
        private System.Windows.Forms.DataGridView dataGridDrivers;
        private System.Windows.Forms.Button UpdateChildTableButton;
        private System.Windows.Forms.Button DeleteChildRowButton;
        private System.Windows.Forms.Button AddRecordInChildTableButton;
        private System.Windows.Forms.TextBox driverNameTextBox;
        private System.Windows.Forms.TextBox driverNationalityTextBox;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.DateTimePicker driverDoBDateTimePicker;
    }
}

