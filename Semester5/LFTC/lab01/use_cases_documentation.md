# Mini Language - Use Cases and Documentation

**Domain: Data Transformation and Processing**

---

## 1. Language Overview

The mini language is a domain-specific language (DSL) designed for simple data transformation and filtering operations. It provides a pipeline-based approach to loading, filtering, transforming, selecting, and saving data.

**Primary Use Case:** Data transformation tasks where users need to quickly process tabular data (similar to spreadsheet operations or SQL-like data manipulation) without writing complex code.

**Target Users:** Data analysts, scientists, and engineers who need to perform simple ETL (Extract, Transform, Load) operations on datasets.

### Key Semantic Rule: Variables vs. Columns

The language distinguishes between variables and dataset columns using a simple syntax rule:

- **Unquoted identifiers** (e.g., `temperature`, `counter`, `threshold`) → **Variables**

  - Declared with `VAR` statement
  - Store configuration values, counters, and program state
  - Used in control flow and expressions

- **Quoted strings** (e.g., `"temperature"`, `"price"`, `"region"`) → **Column names**
  - Reference columns in loaded datasets
  - Used in FILTER, MAP, and SELECT operations
  - Represent data fields from CSV/data files

**Example:**

```
VAR threshold = 100;           // Creates a variable
LOAD "data.csv";              // Loads dataset
FILTER "temperature" > threshold;  // "temperature" is column, threshold is variable
```

---

## 2. Statement-by-Statement Use Cases

### 2.1 VAR Statement

**Syntax:** `VAR identifier = operand;`

**Purpose:** Declares and initializes a variable that can be used in control flow (loops, conditions) and expressions. Variables are distinct from dataset columns.

**Note:** `operand` can be an identifier (variable), string (literal/column), or number (literal).

**Semantic Rule:**

- **Unquoted identifiers** (e.g., `temperature`, `counter`) refer to **variables**
- **Quoted strings** (e.g., `"temperature"`, `"price"`) refer to **column names** in the dataset

#### Use Case Example 1 - Loop Counter

```
VAR counter = 0;
FOR i IN 1 TO 10 DO
    LOAD "data.csv";
    MAP counter = counter + 1;
    SAVE "output.csv";
END;
```

**Justification:** A data pipeline needs to track how many iterations have been completed. The VAR statement declares a counter variable that can be incremented within the loop to maintain state across iterations.

#### Use Case Example 2 - Threshold Management

```
VAR max_temperature = 100;
LOAD "sensor_data.csv";
FILTER "temperature" < max_temperature;
SAVE "safe_readings.csv";
```

**Justification:** A monitoring system uses a configurable threshold stored in a variable. The variable `max_temperature` holds the threshold value, while `"temperature"` (quoted) refers to the temperature column in the CSV file.

#### Use Case Example 3 - Processing Flags

```
VAR processing_complete = 0;
WHILE processing_complete == 0 DO
    LOAD "queue.csv";
    FILTER "status" == "pending";
    MAP processing_complete = 1;
    SAVE "processed.csv";
END;
```

**Justification:** A batch processing system uses a flag variable to control when processing should stop. The variable `processing_complete` controls the loop, while `"status"` (quoted) refers to a column in the dataset.

#### Use Case Example 4 - Configuration Values

```
VAR discount_rate = 20;
VAR min_price = 50;
LOAD "products.csv";
FILTER "price" > min_price;
MAP "sale_price" = "price" - discount_rate;
SAVE "discounted_products.csv";
```

**Justification:** A sales system stores configuration values (discount rate, minimum price) in variables for easy modification. Variables hold the business logic parameters, while quoted strings reference dataset columns.

---

### 2.2 LOAD Statement

**Syntax:** `LOAD "filename";`

**Purpose:** Loads a data file into memory for processing. This is the entry point for any data transformation pipeline.

**Note:** Column names in the loaded dataset can be referenced using quoted strings (e.g., `"column_name"`).

#### Use Case Example 1 - Sales Data Analysis

```
LOAD "sales_data.csv";
```

**Justification:** A retail analyst needs to analyze monthly sales data. The LOAD statement reads the CSV file containing sales records (date, product, quantity, price, region) into memory for further processing.

#### Use Case Example 2 - IoT Sensor Data

```
LOAD "temperature_readings.csv";
```

**Justification:** An IoT engineer needs to process temperature sensor readings from a building. The LOAD statement imports the sensor log file containing timestamps and temperature values.

#### Use Case Example 3 - Student Records

```
LOAD "student_grades.csv";
```

**Justification:** An educational institution needs to process student grades. The LOAD statement imports the grade records for analysis and reporting.

---

### 2.3 FILTER Statement

**Syntax:** `FILTER operand comparison_op operand;`

**Purpose:** Filters the dataset based on a condition, keeping only rows that satisfy the specified criteria. Essential for data cleaning and subsetting.

**Usage:**

- Use unquoted identifier for a **variable**: `FILTER threshold > 100`
- Use quoted string for a **column**: `FILTER "price" > 100`
- Use number for a **literal**: `FILTER "price" > 1000`

#### Use Case Example 1 - High-Value Transactions (Column Filter)

```
LOAD "sales_data.csv";
FILTER "price" > 1000;
SAVE "high_value_sales.csv";
```

**Justification:** A financial analyst needs to identify and extract all transactions above $1000 for audit purposes. The FILTER statement uses `"price"` (quoted) to refer to the price column in the CSV, removing all lower-value transactions.

#### Use Case Example 2 - Variable-Based Filtering

```
VAR max_temp = 75;
LOAD "temperature_readings.txt";
FILTER "temperature" > max_temp;
SAVE "overheating_alerts.txt";
```

**Justification:** A building management system uses a configurable temperature threshold. The variable `max_temp` holds the limit value, while `"temperature"` (quoted) references the temperature column in the data file.

#### Use Case Example 3 - Failing Students (Column Filter)

```
LOAD "student_grades.csv";
FILTER "grade" < 50;
SAVE "students_at_risk.csv";
```

**Justification:** Academic advisors need to identify students who are failing (grade below 50) to provide additional support. The FILTER statement uses `"grade"` (quoted) to reference the grade column.

#### Use Case Example 4 - Regional Data (String Column Match)

```
LOAD "sales_data.csv";
FILTER "region" == "West";
SAVE "west_region_sales.csv";
```

**Justification:** A regional manager needs to analyze only the Western region's sales performance. The FILTER statement uses `"region"` (quoted) to reference the region column and matches it against the string value "West".

---

### 2.4 MAP Statement

**Syntax:** `MAP operand = expression;`

**Purpose:** Creates or transforms a column or variable by applying a calculation or assignment. This is essential for data transformation and derived value computation.

**Usage:**

- Use unquoted identifier to assign to a **variable**: `MAP counter = counter + 1`
- Use quoted string to create/modify a **column**: `MAP "revenue" = "quantity" * "price"`
- Use **parentheses** for complex expressions: `MAP "total" = ("a" + "b") * "c"`

#### Use Case Example 1 - Revenue Calculation (Column Operations)

```
LOAD "sales_data.csv";
MAP "revenue" = "quantity" * "price";
SAVE "sales_with_revenue.csv";
```

**Justification:** A business analyst needs to calculate total revenue for each sale. The MAP statement creates a new column `"revenue"` by multiplying existing columns `"quantity"` and `"price"` for each row.

#### Use Case Example 2 - Variable Assignment

```
VAR celsius = 0;
LOAD "temperature_readings.txt";
MAP celsius = "fahrenheit" - 32;
SAVE "temperature_celsius.txt";
```

**Justification:** A temperature conversion system uses a variable to store the converted value. The MAP statement assigns to the variable `celsius` (unquoted) using the `"fahrenheit"` column (quoted) from the dataset.

#### Use Case Example 3 - Grade Adjustment (Column Modification)

```
LOAD "student_grades.csv";
MAP "adjusted_grade" = "grade" + 5;
SAVE "curved_grades.csv";
```

**Justification:** An instructor decides to curve all grades by adding 5 bonus points. The MAP statement creates a new `"adjusted_grade"` column by adding 5 to the existing `"grade"` column.

#### Use Case Example 4 - Discount Application with Variable

```
VAR discount = 10;
LOAD "products.csv";
MAP "sale_price" = "price" - discount;
SAVE "sale_products.csv";
```

**Justification:** A store applies a configurable discount stored in a variable. The MAP statement creates `"sale_price"` column by subtracting the `discount` variable from the `"price"` column.

#### Use Case Example 5 - Complex Expression with Parentheses

```
VAR tax_rate = 10;
LOAD "orders.csv";
MAP "total_cost" = ("price" + "shipping") * tax_rate;
SAVE "invoices.csv";
```

**Justification:** An e-commerce system calculates total cost including tax on the sum of price and shipping. Parentheses ensure that addition happens before multiplication: `(price + shipping) * tax_rate`.

#### Use Case Example 6 - Nested Parentheses

```
LOAD "financial_data.csv";
MAP "profit_margin" = ("revenue" - ("cost" + "tax")) / "revenue";
SAVE "profit_analysis.csv";
```

**Justification:** A financial analysis requires calculating profit margin with multiple operations. Parentheses control the order: first calculate `cost + tax`, then subtract from `revenue`, then divide by `revenue`.

---

### 2.5 SELECT Statement

**Syntax:** `SELECT identifier_list;`

**Purpose:** Projects specific columns from the dataset, removing all others. This reduces data size and focuses on relevant fields.

**Note:** Column names in SELECT are specified as identifiers (unquoted) for simplicity.

#### Use Case Example 1 - Privacy Compliance

```
LOAD "customer_data.csv";
SELECT name, email, purchase_date;
SAVE "marketing_list.csv";
```

**Justification:** A marketing team needs customer contact information but should not access sensitive data like credit card numbers or addresses. The SELECT statement extracts only the necessary columns, ensuring compliance with privacy policies.

#### Use Case Example 2 - Summary Report

```
LOAD "sales_data.csv";
SELECT product, revenue, region;
SAVE "sales_summary.csv";
```

**Justification:** An executive needs a high-level sales summary without detailed transaction data. The SELECT statement extracts only the key metrics (product, revenue, region) for the report.

#### Use Case Example 3 - IoT Dashboard Data

```
LOAD "sensor_readings.txt";
SELECT timestamp, temperature;
SAVE "temperature_chart_data.txt";
```

**Justification:** A dashboard application needs to display temperature over time but doesn't need other sensor data (humidity, pressure, etc.). The SELECT statement provides only the required fields.

#### Use Case Example 4 - Student Contact List

```
LOAD "student_records.csv";
SELECT student_id, name, email;
SAVE "contact_list.csv";
```

**Justification:** An administrator needs to create a contact list without exposing grades or other academic information. The SELECT statement extracts only identification and contact information.

---

### 2.6 SAVE Statement

**Syntax:** `SAVE "filename";`

**Purpose:** Writes the processed data to a file. This is the output stage of the data transformation pipeline.

#### Use Case Example 1 - Processed Data Export

```
LOAD "raw_data.csv";
FILTER status == "active";
MAP total = quantity * price;
SELECT product, total, date;
SAVE "processed_report.csv";
```

**Justification:** After a series of transformations, the final cleaned and processed data needs to be saved for use in other systems or for archival. The SAVE statement exports the results to a new file.

#### Use Case Example 2 - Backup Creation

```
LOAD "critical_data.csv";
SAVE "critical_data_backup.csv";
```

**Justification:** Before performing risky operations, a backup of the original data is needed. The SAVE statement creates a copy for safety.

#### Use Case Example 3 - Format Conversion

```
LOAD "data.txt";
SAVE "data.csv";
```

**Justification:** Data needs to be converted from one format to another. The LOAD and SAVE statements handle the format conversion.

---

### 2.7 FOR Statement

**Syntax:** `FOR identifier IN range DO statement_list END;`

**Purpose:** Iterates over a numeric range, executing a series of statements for each value. Essential for batch processing and repetitive transformations.

**Note:** The loop variable (identifier) is automatically created and can be used within the loop body.

#### Use Case Example 1 - Multi-Year Data Processing

```
FOR year IN 2020 TO 2024 DO
    LOAD "sales_2020.csv";
    FILTER revenue > 5000;
    SAVE "high_revenue_2020.csv";
END;
```

**Justification:** A business analyst needs to process sales data for multiple years using the same filtering logic. The FOR loop iterates through years 2020-2024, applying identical transformations to each year's dataset, avoiding repetitive code.

#### Use Case Example 2 - Batch Sensor Calibration

```
FOR sensor_id IN 1 TO 100 DO
    LOAD "sensor_data.csv";
    FILTER id == sensor_id;
    MAP calibrated_value = raw_value - 5;
    SAVE "calibrated_sensor_data.csv";
END;
```

**Justification:** An IoT system has 100 sensors that need calibration. The FOR loop processes each sensor individually, applying a calibration offset to correct measurement drift across all sensors.

#### Use Case Example 3 - Grade Distribution by Class

```
FOR class_number IN 1 TO 10 DO
    LOAD "all_students.csv";
    FILTER class == class_number;
    MAP final_grade = midterm + final;
    SELECT student_name, final_grade;
    SAVE "class_grades.csv";
END;
```

**Justification:** A school needs to generate grade reports for 10 different classes. The FOR loop processes each class separately, calculating final grades and creating individual class reports.

#### Use Case Example 4 - Price Point Analysis

```
FOR discount IN 10 TO 50 DO
    LOAD "products.csv";
    MAP discounted_price = price - discount;
    FILTER discounted_price > 0;
    SAVE "pricing_analysis.csv";
END;
```

**Justification:** A retail company wants to analyze how different discount levels (10-50) affect their product pricing. The FOR loop tests each discount amount to determine optimal pricing strategies.

---

### 2.8 WHILE Statement

**Syntax:** `WHILE condition DO statement_list END;`

**Purpose:** Repeatedly executes statements as long as a condition is true. Useful for processing data until a specific criterion is met.

**Usage:**

- Condition format: `operand comparison_op operand`
- Use unquoted identifier for **variables**: `WHILE counter < 10`
- Use quoted string for **columns**: `WHILE "temperature" < 100`

#### Use Case Example 1 - Iterative Data Cleaning (Variable-Based)

```
VAR error_count = 10;
WHILE error_count > 0 DO
    LOAD "messy_data.csv";
    FILTER "quality_score" > 50;
    MAP error_count = error_count - 1;
    SAVE "cleaned_data.csv";
END;
```

**Justification:** A data cleaning pipeline uses a variable to track remaining errors. The WHILE loop checks the variable `error_count` (unquoted) and continues filtering the `"quality_score"` column (quoted) until all errors are processed.

#### Use Case Example 2 - Temperature Monitoring Loop (Variable-Based)

```
VAR current_temp = 0;
WHILE current_temp < 100 DO
    LOAD "sensor_readings.txt";
    FILTER "status" == "active";
    MAP current_temp = "temperature";
    SAVE "monitoring_log.txt";
END;
```

**Justification:** A safety system uses a variable to track current temperature. The WHILE loop checks `current_temp` variable (unquoted) and reads from the `"temperature"` column (quoted). The loop continues until the critical threshold is reached.

#### Use Case Example 3 - Stock Level Management (Variable-Based)

```
VAR stock_level = 500;
WHILE stock_level < 1000 DO
    LOAD "inventory.csv";
    FILTER "category" == "essential";
    MAP stock_level = stock_level + "reorder_quantity";
    SAVE "updated_inventory.csv";
END;
```

**Justification:** An inventory system uses the variable `stock_level` to track current inventory. The WHILE loop continues while `stock_level` (unquoted variable) is below minimum, adding values from the `"reorder_quantity"` column (quoted).

#### Use Case Example 4 - Column-Based Condition

```
LOAD "performance_metrics.csv";
WHILE "avg_response_time" > 500 DO
    FILTER "server_load" < 80;
    MAP "avg_response_time" = "avg_response_time" - 10;
    SAVE "optimized_metrics.csv";
END;
```

**Justification:** A performance system monitors a column value directly. The WHILE loop checks the `"avg_response_time"` column (quoted) and continues optimizing until the column value meets the performance target.

---

## 3. Complete Pipeline Examples

### 3.1 E-commerce Sales Analysis Pipeline

```
VAR discount_amount = 20;
LOAD "daily_sales.csv";
FILTER "amount" > 100;
MAP "discount_price" = "amount" - discount_amount;
SELECT customer_id, product_name, discount_price;
SAVE "premium_customer_discounts.csv";
```

**Business Context:** An e-commerce company wants to offer special discounts to customers who made high-value purchases. This pipeline:

1. Declares a discount variable
2. Loads daily sales data
3. Filters for purchases over $100 using the `"amount"` column
4. Calculates discount price using the variable and `"amount"` column
5. Selects only relevant columns
6. Saves the result for the marketing team

---

### 3.2 IoT Sensor Alert System

```
VAR threshold = 80;
LOAD "sensor_data.txt";
FILTER "temperature" > threshold;
MAP "alert_priority" = "temperature" - threshold;
SELECT sensor_id, timestamp, temperature, alert_priority;
SAVE "critical_alerts.txt";
```

**Technical Context:** A building management system monitors temperature sensors. This pipeline:

1. Declares a temperature threshold variable
2. Loads sensor readings
3. Filters for temperatures exceeding the threshold using `"temperature"` column
4. Calculates alert priority using column and variable
5. Selects relevant alert information
6. Saves alerts for immediate action

---

### 3.3 Academic Performance Monitoring

```
VAR bonus_points = 10;
LOAD "semester_grades.csv";
FILTER "attendance" > 75;
MAP "final_score" = "grade" + bonus_points;
SELECT student_name, final_score;
SAVE "bonus_eligible.csv";
```

**Educational Context:** A university awards bonus points to students with good attendance. This pipeline:

1. Declares a bonus points variable
2. Loads semester grades
3. Filters for students with >75% attendance using `"attendance"` column
4. Adds bonus points to the `"grade"` column
5. Selects student names and adjusted scores
6. Saves the list of eligible students

---

### 3.4 Multi-Region Sales Processing with Loops

```
VAR revenue_target = 50000;
FOR region_id IN 1 TO 5 DO
    LOAD "sales_data.csv";
    FILTER "region" == region_id;
    WHILE revenue_target < 100000 DO
        MAP revenue_target = revenue_target + 10000;
        FILTER "product_price" > 50;
        SELECT region, product_name, product_price;
    END;
    SAVE "region_analysis.csv";
END;
```

**Business Context:** A multinational company analyzes sales across 5 regions with iterative revenue target adjustments. This pipeline:

1. Declares a revenue target variable
2. Iterates through 5 different regions using FOR loop
3. Loads and filters data for each region using `"region"` column
4. Uses WHILE loop with variable to adjust targets until threshold is met
5. Filters high-value products using `"product_price"` column
6. Selects relevant metrics and saves regional reports

---

## 4. Language Advantages

1. **Simplicity:** Easy to learn and use, even for non-programmers
2. **Readability:** Code is self-documenting and clear in intent
3. **Pipeline Approach:** Natural flow from input to output
4. **Domain-Focused:** Optimized for data transformation tasks
5. **Minimal Syntax:** Few keywords and simple grammar reduce errors
6. **Loop Support:** FOR and WHILE loops enable batch processing and iterative operations
7. **Nested Control Flow:** Loops can be combined for complex multi-dimensional processing
8. **Clear Variable/Column Distinction:** Unquoted identifiers are variables, quoted strings are columns
9. **Flexible Data Access:** Variables enable configuration and state management alongside column operations
10. **Expression Power:** Parentheses allow complex mathematical expressions with controlled order of operations

---

## 5. Typical Workflows

**Data Cleaning Workflow:**

```
LOAD → FILTER (remove invalid) → SELECT (relevant columns) → SAVE
```

**Data Transformation Workflow:**

```
LOAD → MAP (calculate new values) → SELECT (choose columns) → SAVE
```

**Data Analysis Workflow:**

```
LOAD → FILTER (subset) → MAP (compute metrics) → SAVE
```

**Data Export Workflow:**

```
LOAD → SELECT (specific columns) → SAVE
```

**Batch Processing Workflow (with FOR loop):**

```
FOR range → LOAD → FILTER → MAP → SAVE → END
```

**Iterative Processing Workflow (with WHILE loop):**

```
WHILE condition → LOAD → FILTER/MAP → SAVE → END
```

**Complex Multi-Level Processing (nested loops):**

```
FOR range → WHILE condition → LOAD → FILTER → MAP → SELECT → END → SAVE → END
```

---

## 6. Conclusion

This mini language provides a focused, efficient way to perform common data transformation tasks. Each statement serves a specific purpose in the ETL pipeline, making it ideal for quick data processing without the overhead of general-purpose programming languages.

With the addition of FOR and WHILE loops, the language now supports:

- **Batch processing** across multiple datasets or time periods
- **Iterative refinement** of data until quality standards are met
- **Complex workflows** combining loops with data transformation operations
- **Automated repetitive tasks** without code duplication

The language is particularly well-suited for:

- Data analysts performing ad-hoc data transformations
- IoT systems processing sensor data and continuous monitoring
- Educational institutions managing student records across multiple classes
- Business users performing sales analysis across regions and time periods
- Batch processing scenarios requiring iterative data refinement
- Any scenario requiring simple, readable data pipelines with control flow
