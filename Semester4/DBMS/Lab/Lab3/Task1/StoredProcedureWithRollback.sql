-- Task 1: Stored procedure with complete rollback
USE f1;
GO

-- Procedure for task 1
CREATE OR ALTER PROCEDURE dbo.InsertTeamSponsorshipAll
    @TeamName VARCHAR(255),
    @TeamCountry VARCHAR(5),
    @ConstructorName VARCHAR(255),
    @SponsorName VARCHAR(255),
    @SponsorCountry VARCHAR(5)
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRY
        BEGIN TRANSACTION;
            
        DECLARE @ConstructorId INT;
        DECLARE @TeamId INT;
        DECLARE @SponsorId INT;
        
        -- Check if constructor exists, if not create error (must exist)
        SELECT @ConstructorId = constructorId 
        FROM constructor 
        WHERE constructorName = @ConstructorName;
        
        IF @ConstructorId IS NULL
        BEGIN
            RAISERROR('Constructor does not exist', 16, 1);
            RETURN;
        END
        
        -- Insert team if it doesn't exist
        IF NOT EXISTS(SELECT 1 FROM team WHERE teamName = @TeamName)
        BEGIN
            INSERT INTO team(constructorId, teamName, teamCountry)
            VALUES(@ConstructorId, @TeamName, @TeamCountry);
            
            SET @TeamId = SCOPE_IDENTITY();
            
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('INSERT', 'team', 'Added team: ' + @TeamName);
        END
        ELSE
        BEGIN
            SELECT @TeamId = teamId FROM team WHERE teamName = @TeamName;
        END
        
        -- Insert sponsor if it doesn't exist
        IF NOT EXISTS(SELECT 1 FROM sponsor WHERE sponsorName = @SponsorName)
        BEGIN
            INSERT INTO sponsor(sponsorName, sponsorCountry)
            VALUES(@SponsorName, @SponsorCountry);
            
            SET @SponsorId = SCOPE_IDENTITY();
            
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('INSERT', 'sponsor', 'Added sponsor: ' + @SponsorName);
        END
        ELSE
        BEGIN
            SELECT @SponsorId = sponsorId FROM sponsor WHERE sponsorName = @SponsorName;
        END
        
        -- Create the sponsorship relationship
        IF NOT EXISTS(SELECT 1 FROM sponsorship WHERE teamId = @TeamId AND sponsorId = @SponsorId)
        BEGIN
            INSERT INTO sponsorship(teamId, sponsorId)
            VALUES(@TeamId, @SponsorId);
            
            INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
            VALUES('INSERT', 'sponsorship', 'Added sponsorship between: ' + @TeamName + ' and ' + @SponsorName);
        END
        ELSE
        BEGIN
            RAISERROR('Sponsorship already exists', 16, 1);
        END
        
        COMMIT TRANSACTION;
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;
        
        INSERT INTO dbo.ActionLog(ActionType, TableName, Description)
        VALUES('ERROR', 'InsertTeamSponsorshipAll', 'Error: ' + ERROR_MESSAGE());
        
        THROW;
    END CATCH
END;
GO