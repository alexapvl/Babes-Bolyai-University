-- Optimistic Isolation (SNAPSHOT) demonstration
USE f1;
GO

-- Connection 1
CREATE OR ALTER PROCEDURE OptimisticConflictDemo1 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'OptimisticConflictDemo1', 'Starting optimistic conflict demo - first connection');
    
    SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Read current value
        DECLARE @currentPoints FLOAT;
        SELECT @currentPoints = points 
        FROM driverChampionship 
        WHERE driverId = 1;
        
        -- Wait for Connection 2 to update and commit
        WAITFOR DELAY '00:00:10';
        
        -- Try to update with our calculated value (will cause conflict)
        UPDATE driverChampionship
        SET points = @currentPoints + 18
        WHERE driverId = 1;
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'OptimisticConflictDemo1', 'Successfully completed optimistic conflict demo - first connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'OptimisticConflictDemo1', 'Error in optimistic conflict demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Connection 2
CREATE OR ALTER PROCEDURE OptimisticConflictDemo2 AS
BEGIN
    -- Log start before beginning transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'OptimisticConflictDemo2', 'Starting optimistic conflict demo - second connection');
    
    SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
    
    BEGIN TRY
        BEGIN TRANSACTION;
        
        -- Update points
        UPDATE driverChampionship
        SET points = points + 25
        WHERE driverId = 1;
        
        COMMIT TRANSACTION;
        
        -- Log success AFTER the transaction is committed
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('SUCCESS', 'OptimisticConflictDemo2', 'Successfully completed optimistic conflict demo - second connection');
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        -- Log error AFTER the transaction is rolled back
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'OptimisticConflictDemo2', 'Error in optimistic conflict demo: ' + ERROR_MESSAGE());
        
        -- Re-throw the error
        THROW;
    END CATCH
END;
GO

-- Solution: Use TRY-CATCH to handle the update conflict and retry with fresh data
CREATE OR ALTER PROCEDURE OptimisticConflictSolution AS
BEGIN
    -- Log start before any transaction
    INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
    VALUES('START', 'OptimisticConflictSolution', 'Starting optimistic solution with retry logic');
    
    SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
    
    DECLARE @retry INT = 3;
    DECLARE @success BIT = 0;
    DECLARE @errorMsg NVARCHAR(4000);
    DECLARE @errorSeverity INT;
    DECLARE @errorState INT;
    DECLARE @attemptCount INT = 0;
    
    WHILE @retry > 0 AND @success = 0
    BEGIN
        SET @attemptCount = @attemptCount + 1;
        PRINT 'Try number: ' + CAST(@attemptCount AS VARCHAR);
        
        BEGIN TRY
            BEGIN TRANSACTION;
            
            -- Read current value
            DECLARE @currentPoints FLOAT;
            SELECT @currentPoints = points 
            FROM driverChampionship 
            WHERE driverId = 1;
            
            -- Give other transactions a chance to make changes
            WAITFOR DELAY '00:00:05';
            
            -- Try to update with our calculated value
            UPDATE driverChampionship
            SET points = @currentPoints + 18
            WHERE driverId = 1;
            
            COMMIT TRANSACTION;
            SET @success = 1;
        END TRY
        BEGIN CATCH
            PRINT 'Error occurred: ' + ERROR_MESSAGE();
            -- Save error details before any other operations
            SELECT 
                @errorMsg = ERROR_MESSAGE(),
                @errorSeverity = ERROR_SEVERITY(),
                @errorState = ERROR_STATE();
                
            IF @@TRANCOUNT > 0
                ROLLBACK TRANSACTION;
                
            SET @retry = @retry - 1;
            
            -- Log retry AFTER the transaction is rolled back
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('RETRY', 'OptimisticConflictSolution', 'Update conflict detected: ' + @errorMsg + '. Retrying. Attempts left: ' + CAST(@retry AS VARCHAR));
            
            -- Wait a bit before retrying
            WAITFOR DELAY '00:00:02';
            
            -- Don't re-throw here, we want to retry
            PRINT 'Retrying...';
        END CATCH
    END
    
    -- Ensure final logging works by adding a separate TRY-CATCH block
    BEGIN TRY
        IF @success = 0
        BEGIN
            PRINT 'Failed to update after multiple retries';
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('FAILED', 'OptimisticConflictSolution', 'Failed to update after multiple retries');
        END
        ELSE
        BEGIN
            PRINT 'Update successful after ' + CAST(@attemptCount AS VARCHAR) + ' attempts';
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('SUCCESS', 'OptimisticConflictSolution', 'Update successful after ' + CAST(@attemptCount AS VARCHAR) + ' attempts');
        END
    END TRY
    BEGIN CATCH
        PRINT 'Error logging final status: ' + ERROR_MESSAGE();
        -- Try one more time with simplified message
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'OptimisticConflictSolution', 'Error logging final status: ' + ERROR_MESSAGE());
    END CATCH
END;
GO 