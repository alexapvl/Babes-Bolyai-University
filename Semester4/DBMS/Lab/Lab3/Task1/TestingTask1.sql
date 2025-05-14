-- Testing script for InsertTeamSponsorshipAll stored procedure
-- This script will test both successful and failure scenarios

USE f1;
GO

-- Clear existing test data (if any)

DELETE FROM sponsorship WHERE teamId IN (SELECT teamId FROM team WHERE teamName LIKE 'Test%');
DELETE FROM sponsor WHERE sponsorName LIKE 'Test%';
DELETE FROM team WHERE teamName LIKE 'Test%';
DELETE FROM constructor WHERE constructorName LIKE 'Test%';
DELETE FROM ActionLog WHERE Description LIKE '%Test%';

GO

-- Insert test constructors
INSERT INTO constructor (constructorId, constructorName, constructorCountry)
VALUES (1001, 'TestConstructor1', 'USA'),
       (1002, 'TestConstructor2', 'GER');
GO

PRINT '=== Testing successful case ===';
PRINT 'Creating new team, sponsor and sponsorship';

-- Test Case 1: Successfully add new team, sponsor and create sponsorship
BEGIN TRY
    EXEC dbo.InsertTeamSponsorshipAll
        @TeamName = 'TestTeam1',
        @TeamCountry = 'USA',
        @ConstructorName = 'TestConstructor1',
        @SponsorName = 'TestSponsor1', 
        @SponsorCountry = 'JPN';
        
    PRINT 'Successfully added team, sponsor and created sponsorship';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the results
PRINT 'Verifying results:';
SELECT t.teamName, s.sponsorName
FROM sponsorship sp
JOIN team t ON sp.teamId = t.teamId
JOIN sponsor s ON sp.sponsorId = s.sponsorId
WHERE t.teamName = 'TestTeam1' AND s.sponsorName = 'TestSponsor1';

-- Check logs
PRINT 'Checking logs:';
SELECT * FROM ActionLog WHERE Description LIKE '%TestTeam1%' OR Description LIKE '%TestSponsor1%';
GO

PRINT '=== Testing idempotency ===';
PRINT 'Running the same procedure again (should not create duplicates)';

-- Test Case 2: Test idempotency - running the same command should not create duplicates
BEGIN TRY
    EXEC dbo.InsertTeamSponsorshipAll
        @TeamName = 'TestTeam1',
        @TeamCountry = 'USA',
        @ConstructorName = 'TestConstructor1',
        @SponsorName = 'TestSponsor1', 
        @SponsorCountry = 'JPN';
        
    PRINT 'Procedure completed successfully';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify no duplicates
PRINT 'Verifying no duplicates:';
SELECT COUNT(*) AS SponsorshipCount 
FROM sponsorship sp
JOIN team t ON sp.teamId = t.teamId
JOIN sponsor s ON sp.sponsorId = s.sponsorId
WHERE t.teamName = 'TestTeam1' AND s.sponsorName = 'TestSponsor1';
GO

PRINT '=== Testing new sponsor for existing team ===';

-- Test Case 3: Add new sponsor to existing team
BEGIN TRY
    EXEC dbo.InsertTeamSponsorshipAll
        @TeamName = 'TestTeam1',
        @TeamCountry = 'USA',
        @ConstructorName = 'TestConstructor1',
        @SponsorName = 'TestSponsor2', 
        @SponsorCountry = 'ITA';
        
    PRINT 'Successfully added new sponsor to existing team';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the new sponsorship
PRINT 'Verifying new sponsorship:';
SELECT t.teamName, s.sponsorName
FROM sponsorship sp
JOIN team t ON sp.teamId = t.teamId
JOIN sponsor s ON sp.sponsorId = s.sponsorId
WHERE t.teamName = 'TestTeam1' AND s.sponsorName = 'TestSponsor2';
GO

PRINT '=== Testing failure case - non-existent constructor ===';

-- Test Case 4: Test rollback on error (invalid constructor)
BEGIN TRY
    EXEC dbo.InsertTeamSponsorshipAll
        @TeamName = 'TestTeam3',
        @TeamCountry = 'FRA',
        @ConstructorName = 'NonExistentConstructor',
        @SponsorName = 'TestSponsor3', 
        @SponsorCountry = 'ESP';
        
    PRINT 'Procedure completed successfully';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the team and sponsor weren't created (should be rolled back)
PRINT 'Verifying no data was inserted:';
SELECT COUNT(*) AS TeamCount FROM team WHERE teamName = 'TestTeam3';
SELECT COUNT(*) AS SponsorCount FROM sponsor WHERE sponsorName = 'TestSponsor3';
GO

PRINT '=== Testing final verification ===';

-- Show all test sponsorships
PRINT 'All test sponsorships:';
SELECT t.teamName, s.sponsorName
FROM sponsorship sp
JOIN team t ON sp.teamId = t.teamId
JOIN sponsor s ON sp.sponsorId = s.sponsorId
WHERE t.teamName LIKE 'Test%' OR s.sponsorName LIKE 'Test%';

-- Show all logs from the test
PRINT 'All test logs:';
SELECT * FROM ActionLog 
WHERE Description LIKE '%Test%' 
ORDER BY LogDateTime DESC;
GO

-- Clean up test data - commented out to preserve test data (will be deleted in the next test run)
/*
DELETE FROM sponsorship WHERE teamId IN (SELECT teamId FROM team WHERE teamName LIKE 'Test%');
DELETE FROM sponsor WHERE sponsorName LIKE 'Test%';
DELETE FROM team WHERE teamName LIKE 'Test%';
DELETE FROM constructor WHERE constructorName LIKE 'Test%';
DELETE FROM ActionLog WHERE Description LIKE '%Test%';
GO
*/

PRINT 'Testing completed!'; 