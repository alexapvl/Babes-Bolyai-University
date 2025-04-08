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
            this.f1DataSetBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.f1DataSet = new Setup_DBMS.f1DataSet();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSetBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSet)).BeginInit();
            this.SuspendLayout();
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
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(2166, 1286);
            this.Name = "Form1";
            this.Text = "Dynamic Parent-Child Table Form";
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSetBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.f1DataSet)).EndInit();
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.BindingSource f1DataSetBindingSource;
        private f1DataSet f1DataSet;
    }
}

