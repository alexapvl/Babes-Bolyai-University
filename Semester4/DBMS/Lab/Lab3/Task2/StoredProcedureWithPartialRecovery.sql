-- Task 2: Stored procedure with partial recovery
USE f1;
GO

CREATE OR ALTER PROCEDURE dbo.InsertRaceResult
    @DriverName VARCHAR(255),
    @CircuitName VARCHAR(255),
    @RaceDate DATE,
    @Placement INT
AS
BEGIN
    SET NOCOUNT ON;
    
    DECLARE @DriverId INT;
    DECLARE @CircuitId INT;
    DECLARE @RaceId INT;
    
    -- Validate parameters
    IF @Placement < 1 OR @Placement > 20
    BEGIN
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Invalid placement: ' + CAST(@Placement AS VARCHAR));
        
        RAISERROR('Placement must be between 1 and 20', 16, 1);
        RETURN;
    END
    
    IF NOT (YEAR(@RaceDate) = 2025)
    BEGIN
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Invalid race date: ' + CAST(@RaceDate AS VARCHAR));
        
        RAISERROR('Race date must be in 2025', 16, 1);
        RETURN;
    END
    
    -- Get driver ID
    BEGIN TRY
        SELECT @DriverId = driverId 
        FROM driver 
        WHERE driverName = @DriverName;
        
        IF @DriverId IS NULL
        BEGIN
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('ERROR', 'InsertRaceResult', 'Driver not found: ' + @DriverName);
            
            RAISERROR('Driver not found', 16, 1);
            RETURN;
        END
    END TRY
    BEGIN CATCH
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Error finding driver: ' + ERROR_MESSAGE());
        
        THROW;
        RETURN;
    END CATCH
    
    -- Get circuit ID
    BEGIN TRY
        SELECT @CircuitId = circuitId 
        FROM circuit 
        WHERE circuitName = @CircuitName;
        
        IF @CircuitId IS NULL
        BEGIN
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('ERROR', 'InsertRaceResult', 'Circuit not found: ' + @CircuitName);
            
            RAISERROR('Circuit not found', 16, 1);
            RETURN;
        END
    END TRY
    BEGIN CATCH
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Error finding circuit: ' + ERROR_MESSAGE());
        
        THROW;
        RETURN;
    END CATCH
    
    -- Get or create race
    BEGIN TRY
        SELECT @RaceId = raceId 
        FROM race 
        WHERE circuitId = @CircuitId AND raceDate = @RaceDate;
        
        IF @RaceId IS NULL
        BEGIN
            -- Create new race
            BEGIN TRANSACTION;
            
            INSERT INTO race(circuitId, raceDate, numberOfLaps)
            VALUES(@CircuitId, @RaceDate, 50); -- Default 50 laps
            
            SET @RaceId = SCOPE_IDENTITY();
            
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('INSERT', 'race', 'Added race at circuit: ' + @CircuitName);
            
            COMMIT TRANSACTION;
        END
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Error creating race: ' + ERROR_MESSAGE());
        
        THROW;
        RETURN;
    END CATCH
    
    -- Add result
    BEGIN TRY
        BEGIN TRANSACTION;
        
        INSERT INTO result(raceId, driverId, placement)
        VALUES(@RaceId, @DriverId, @Placement);
        
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('INSERT', 'result', 'Added result for driver: ' + @DriverName + ' at ' + @CircuitName);
        
        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            ROLLBACK TRANSACTION;
            
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertRaceResult', 'Error creating result: ' + ERROR_MESSAGE());
        
        THROW;
    END CATCH
END;
GO