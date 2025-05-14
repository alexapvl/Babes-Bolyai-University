-- Phantom Reads demonstration
USE f1;
GO

-- Connection 1 (REPEATABLE READ)
CREATE OR ALTER PROCEDURE PhantomReadDemo1 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'PhantomReadDemo1', 'Starting phantom read demo - first connection');
    
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- First count
        SELECT COUNT(*) AS DriverCount FROM driver WHERE teamId = 1;
        
        -- Wait for Connection 2 to insert and commit
        WAITFOR DELAY '00:00:10';
        
        -- Second count will see phantom rows
        SELECT COUNT(*) AS DriverCount FROM driver WHERE teamId = 1;
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'PhantomReadDemo1', 'Ending phantom read demo - first connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'PhantomReadDemo1', 'Error in phantom read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Connection 2
CREATE OR ALTER PROCEDURE PhantomReadDemo2 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'PhantomReadDemo2', 'Starting phantom read demo - second connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Insert a new driver for team 1
        INSERT INTO driver(driverName, teamId, nationality, dateOfBirth)
        VALUES('New Driver', 1, 'GBR', '2000-01-01');
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'PhantomReadDemo2', 'Ending phantom read demo - second connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'PhantomReadDemo2', 'Error in phantom read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Solution: Use SERIALIZABLE isolation level instead of REPEATABLE READ