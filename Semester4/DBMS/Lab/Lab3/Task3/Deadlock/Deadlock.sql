-- Deadlock demonstration
USE f1;
GO

-- Connection 1
CREATE OR ALTER PROCEDURE DeadlockDemo1 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'DeadlockDemo1', 'Starting deadlock demo - first connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Lock driver table first
        UPDATE driver
        SET driverName = driverName
        WHERE driverId = 1;
        
        -- Wait for Connection 2 to lock constructor table
        WAITFOR DELAY '00:00:05';
        
        -- Now try to access constructor table
        UPDATE constructor
        SET constructorName = constructorName
        WHERE constructorId = 1;
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'DeadlockDemo1', 'Ending deadlock demo - first connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'DeadlockDemo1', 'Error in deadlock demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Connection 2
CREATE OR ALTER PROCEDURE DeadlockDemo2 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'DeadlockDemo2', 'Starting deadlock demo - second connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Lock constructor table first
        UPDATE constructor
        SET constructorName = constructorName
        WHERE constructorId = 1;
        
        -- Wait for Connection 1 to lock driver table
        WAITFOR DELAY '00:00:05';
        
        -- Now try to access driver table
        UPDATE driver
        SET driverName = driverName
        WHERE driverId = 1;
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'DeadlockDemo2', 'Ending deadlock demo - second connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'DeadlockDemo2', 'Error in deadlock demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Solution: Use consistent order of locking resources or use SNAPSHOT isolation