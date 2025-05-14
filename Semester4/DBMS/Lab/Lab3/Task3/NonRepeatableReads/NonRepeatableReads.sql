-- Non-Repeatable Reads demonstration
USE f1;
GO

-- Connection 1 (READ COMMITTED)
CREATE OR ALTER PROCEDURE NonRepeatableReadDemo1 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'NonRepeatableReadDemo1', 'Starting non-repeatable read demo - first connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- First read
        SELECT sponsorName, sponsorCountry 
        FROM sponsor 
        WHERE sponsorName = 'Red Bull';
        
        -- Wait for Connection 2 to update and commit
        WAITFOR DELAY '00:00:10';
        
        -- Second read will see different data
        SELECT sponsorName, sponsorCountry 
        FROM sponsor 
        WHERE sponsorName = 'Red Bull';
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'NonRepeatableReadDemo1', 'Ending non-repeatable read demo - first connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'NonRepeatableReadDemo1', 'Error in non-repeatable read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Connection 2
CREATE OR ALTER PROCEDURE NonRepeatableReadDemo2 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'NonRepeatableReadDemo2', 'Starting non-repeatable read demo - second connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Update sponsor
        UPDATE sponsor
        SET sponsorCountry = 'AUT'
        WHERE sponsorName = 'Red Bull';
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'NonRepeatableReadDemo2', 'Ending non-repeatable read demo - second connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'NonRepeatableReadDemo2', 'Error in non-repeatable read demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Solution: Use REPEATABLE READ isolation level instead of READ COMMITTED