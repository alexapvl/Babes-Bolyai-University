namespace Setup_DBMS
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using Microsoft.Extensions.Configuration;
    using System.Collections.Generic;
    using System.Text.Json;
    using System.Linq;

    /// <summary>
    /// AppConfig class.
    /// Used for the configuration of the connection string which will be stored in the appsettings.json file (file which is not commited).
    /// </summary>
    public static class AppConfig
    {
        static AppConfig()
        {
            LoadConfiguration();
        }

        /// <summary>
        /// Gets the configuration.
        /// </summary>
        public static IConfiguration Configuration { get; private set; }

        /// <summary>
        /// Get the connection string
        /// !! Use throught the app like this: AppConfig.GetConnectionString("MyLocalDb");
        /// This will get the connection string from the appsettings.json file in the project.
        /// </summary>
        /// <param name="name">The name of the connection string to get, use MyLocalDb.</param>
        /// <returns>The connection string.</returns>
        public static string GetConnectionString(string name)
        {
            return Configuration?[$"ConnectionStrings:{name}"];
        }

        /// <summary>
        /// Get the configuration type
        /// </summary>
        /// <returns>The configuration type</returns>
        public static string GetConfigurationType()
        {
            return Configuration?["TableConfigurationType"];
        }

        /// <summary>
        /// Get the parent table configuration
        /// </summary>
        /// <returns>The parent table configuration</returns>
        public static TableConfig GetParentTableConfig()
        {
            var section = Configuration.GetSection("TableConfiguration:" + GetConfigurationType() + ":ParentTable");
            return new TableConfig
            {
                Name = section["Name"],
                IdColumn = section["IdColumn"],
                DisplayColumns = GetDisplayColumns(section.GetSection("DisplayColumns"))
            };
        }

        /// <summary>
        /// Get the child table configuration
        /// </summary>
        /// <returns>The child table configuration</returns>
        public static ChildTableConfig GetChildTableConfig()
        {
            var section = Configuration.GetSection("TableConfiguration:" + GetConfigurationType() + ":ChildTable");
            var inputFieldsSection = section.GetSection("InputFields");
            
            var inputFields = new List<InputFieldConfig>();
            foreach (var field in inputFieldsSection.GetChildren())
            {
                inputFields.Add(new InputFieldConfig
                {
                    Name = field["Name"],
                    Label = field["Label"],
                    Type = field["Type"]
                });
            }

            return new ChildTableConfig
            {
                Name = section["Name"],
                IdColumn = section["IdColumn"],
                ForeignKeyColumn = section["ForeignKeyColumn"],
                DisplayColumns = GetDisplayColumns(section.GetSection("DisplayColumns")),
                InputFields = inputFields
            };
        }

        /// <summary>
        /// Get display columns from configuration section
        /// </summary>
        /// <param name="section">The configuration section containing display columns</param>
        /// <returns>Array of column names</returns>
        private static string[] GetDisplayColumns(IConfigurationSection section)
        {
            if (section == null)
                return new string[0];

            // Get all children of the section and select their values
            return section.GetChildren().Select(item => item.Value).ToArray();
        }

        /// <summary>
        /// Load the configuration.
        /// </summary>
        private static void LoadConfiguration()
        {
            string jsonFilePath = Path.GetFullPath(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, @"..\..\appsettings.json"));

            var builder = new ConfigurationBuilder()
                .AddJsonFile(jsonFilePath, optional: true, reloadOnChange: true);

            Configuration = builder.Build();
        }
    }

    /// <summary>
    /// Table configuration class
    /// </summary>
    public class TableConfig
    {
        /// <summary>
        /// Gets or sets the table name
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the ID column name
        /// </summary>
        public string IdColumn { get; set; }

        /// <summary>
        /// Gets or sets the display columns
        /// </summary>
        public string[] DisplayColumns { get; set; }
    }

    /// <summary>
    /// Child table configuration class
    /// </summary>
    public class ChildTableConfig : TableConfig
    {
        /// <summary>
        /// Gets or sets the foreign key column name
        /// </summary>
        public string ForeignKeyColumn { get; set; }

        /// <summary>
        /// Gets or sets the input fields
        /// </summary>
        public List<InputFieldConfig> InputFields { get; set; }
    }

    /// <summary>
    /// Input field configuration class
    /// </summary>
    public class InputFieldConfig
    {
        /// <summary>
        /// Gets or sets the field name
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the field label
        /// </summary>
        public string Label { get; set; }

        /// <summary>
        /// Gets or sets the field type
        /// </summary>
        public string Type { get; set; }
    }
}
