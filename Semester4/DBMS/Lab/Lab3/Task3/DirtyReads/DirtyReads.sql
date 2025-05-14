-- Dirty Reads demonstration
USE f1;
GO

-- Connection 1 (READ UNCOMMITTED)
CREATE OR ALTER PROCEDURE DirtyReadDemo2 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'DirtyReadDemo2', 'Starting dirty read demo - second connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Update points but don't commit
        UPDATE driverChampionship
        SET points = points + 25
        WHERE driverId IN (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton');
        
        -- Wait before rolling back
        WAITFOR DELAY '00:00:05';
        
        -- Roll back the change
        ROLLBACK TRANSACTION;
        
        -- Log success AFTER the rollback is completed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'DirtyReadDemo2', 'Ending dirty read demo - second connection, rolled back');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'DirtyReadDemo2', 'Error in dirty read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Connection 2
CREATE OR ALTER PROCEDURE DirtyReadDemo1 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'DirtyReadDemo1', 'Starting dirty read demo - first connection');
    
    SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Read points before update
        SELECT d.driverName, dc.points 
        FROM driverChampionship dc
        JOIN driver d ON d.driverId = dc.driverId
        WHERE d.driverName = 'Lewis Hamilton';
        
        -- Wait for Connection 2 to update
        WAITFOR DELAY '00:00:10';
        
        -- Read again, will see uncommitted data
        SELECT d.driverName, dc.points 
        FROM driverChampionship dc
        JOIN driver d ON d.driverId = dc.driverId
        WHERE d.driverName = 'Lewis Hamilton';
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'DirtyReadDemo1', 'Ending dirty read demo - first connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'DirtyReadDemo1', 'Error in dirty read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Solution: Use READ COMMITTED isolation level instead of READ UNCOMMITTED 