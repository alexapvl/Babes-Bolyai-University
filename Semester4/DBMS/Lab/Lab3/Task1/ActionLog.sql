-- Create logging table
CREATE TABLE dbo.ActionLog (
    LogID INT PRIMARY KEY IDENTITY(1,1),
    LogDateTime DATETIME DEFAULT GETDATE(),
    ActionType VARCHAR(100),
    TableName VARCHAR(100),
    Description VARCHAR(1000)
);