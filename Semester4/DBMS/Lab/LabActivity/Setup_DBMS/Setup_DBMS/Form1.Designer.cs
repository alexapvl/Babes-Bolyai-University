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
            this.loadTeamsButton.Location = new System.Drawing.Point(247, 14);
            this.loadTeamsButton.Name = "loadTeamsButton";
            this.loadTeamsButton.Size = new System.Drawing.Size(191, 123);
            this.loadTeamsButton.TabIndex = 2;
            this.loadTeamsButton.Text = "Load Teams";
            this.loadTeamsButton.UseVisualStyleBackColor = true;
            this.loadTeamsButton.Click += new System.EventHandler(this.LoadTeamsButtonClick);
            // 
            // dataGridDrivers
            // 
            this.dataGridDrivers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridDrivers.Location = new System.Drawing.Point(1121, 181);
            this.dataGridDrivers.Name = "dataGridDrivers";
            this.dataGridDrivers.RowHeadersWidth = 62;
            this.dataGridDrivers.RowTemplate.Height = 28;
            this.dataGridDrivers.Size = new System.Drawing.Size(1016, 1007);
            this.dataGridDrivers.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(2166, 1286);
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

        }

        #endregion

        private System.Windows.Forms.Button testConnectionButton;
        private System.Windows.Forms.BindingSource f1DataSetBindingSource;
        private f1DataSet f1DataSet;
        private System.Windows.Forms.DataGridView dataGridTeams;
        private System.Windows.Forms.Button loadTeamsButton;
        private System.Windows.Forms.DataGridView dataGridDrivers;
    }
}

