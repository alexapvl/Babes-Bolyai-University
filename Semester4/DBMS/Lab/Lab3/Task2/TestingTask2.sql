-- Testing script for InsertRaceResult stored procedure
-- This script will test both successful and partial recovery scenarios

USE f1;
GO

-- Clear existing test data (if any)

DELETE FROM result WHERE raceId IN (SELECT raceId FROM race WHERE circuitId IN (SELECT circuitId FROM circuit WHERE circuitName LIKE 'Test%'));
DELETE FROM race WHERE circuitId IN (SELECT circuitId FROM circuit WHERE circuitName LIKE 'Test%');
DELETE FROM driverChampionship WHERE driverId IN (SELECT driverId FROM driver WHERE driverName LIKE 'Test%');
DELETE FROM driver WHERE driverName LIKE 'Test%';
DELETE FROM circuit WHERE circuitName LIKE 'Test%';
DELETE FROM ActionLog WHERE Description LIKE '%Test%';

GO

-- First, create test prerequisites - team, drivers, circuits
PRINT '=== Setting up test prerequisites ===';

-- Insert test constructor if not exists
IF NOT EXISTS (SELECT 1 FROM constructor WHERE constructorName = 'TestConstructor')
BEGIN
    INSERT INTO constructor (constructorId, constructorName, constructorCountry)
    VALUES (1001, 'TestConstructor', 'USA');
END

-- Insert test team if not exists
DECLARE @TeamId INT;
IF NOT EXISTS (SELECT 1 FROM team WHERE teamName = 'TestTeam')
BEGIN
    INSERT INTO team (constructorId, teamName, teamCountry)
    VALUES (1001, 'TestTeam', 'USA');
    
    SET @TeamId = SCOPE_IDENTITY();
END
ELSE
BEGIN
    SELECT @TeamId = teamId FROM team WHERE teamName = 'TestTeam';
END

-- Insert test drivers if not exist
IF NOT EXISTS (SELECT 1 FROM driver WHERE driverName = 'TestDriver1')
BEGIN
    INSERT INTO driver (driverName, teamId, nationality, dateOfBirth)
    VALUES ('TestDriver1', @TeamId, 'USA', '1990-01-01');
    
    -- Add driver to championship
    INSERT INTO driverChampionship (driverId, points)
    VALUES (SCOPE_IDENTITY(), 0);
END

IF NOT EXISTS (SELECT 1 FROM driver WHERE driverName = 'TestDriver2')
BEGIN
    INSERT INTO driver (driverName, teamId, nationality, dateOfBirth)
    VALUES ('TestDriver2', @TeamId, 'CAN', '1992-05-15');
    
    -- Add driver to championship
    INSERT INTO driverChampionship (driverId, points)
    VALUES (SCOPE_IDENTITY(), 0);
END

-- Insert test circuits if not exist
IF NOT EXISTS (SELECT 1 FROM circuit WHERE circuitName = 'TestCircuit1')
BEGIN
    INSERT INTO circuit (circuitName, circuitCountry, lapLength)
    VALUES ('TestCircuit1', 'USA', 5.5);
END

IF NOT EXISTS (SELECT 1 FROM circuit WHERE circuitName = 'TestCircuit2')
BEGIN
    INSERT INTO circuit (circuitName, circuitCountry, lapLength)
    VALUES ('TestCircuit2', 'CAN', 4.8);
END

PRINT 'Test prerequisites created successfully';
GO

PRINT '=== Testing successful case ===';
PRINT 'Creating new race result (should create both race and result)';

-- Test Case 1: Successfully add a race and result
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver1',
        @CircuitName = 'TestCircuit1',
        @RaceDate = '2025-03-15',
        @Placement = 1;
        
    PRINT 'Successfully added race and result';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the results
PRINT 'Verifying results:';
SELECT r.raceDate, c.circuitName, d.driverName, res.placement
FROM result res
JOIN race r ON res.raceId = r.raceId
JOIN circuit c ON r.circuitId = c.circuitId
JOIN driver d ON res.driverId = d.driverId
WHERE d.driverName = 'TestDriver1' AND c.circuitName = 'TestCircuit1' AND r.raceDate = '2025-03-15';

-- Check logs
PRINT 'Checking logs:';
SELECT * FROM ActionLog WHERE Description LIKE '%TestDriver1%' OR Description LIKE '%TestCircuit1%';
GO

PRINT '=== Testing race reuse ===';
PRINT 'Adding another driver to same race (should reuse existing race)';

-- Test Case 2: Add another driver to the same race
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver2',
        @CircuitName = 'TestCircuit1',
        @RaceDate = '2025-03-15',
        @Placement = 2;
        
    PRINT 'Successfully added result to existing race';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the results - should show both drivers in same race
PRINT 'Verifying results (both drivers):';
SELECT r.raceDate, c.circuitName, d.driverName, res.placement
FROM result res
JOIN race r ON res.raceId = r.raceId
JOIN circuit c ON r.circuitId = c.circuitId
JOIN driver d ON res.driverId = d.driverId
WHERE c.circuitName = 'TestCircuit1' AND r.raceDate = '2025-03-15'
ORDER BY res.placement;
GO

PRINT '=== Testing validation errors ===';

-- Test Case 3: Test invalid placement
PRINT 'Testing invalid placement (should fail validation):';
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver1',
        @CircuitName = 'TestCircuit2',
        @RaceDate = '2025-07-20',
        @Placement = 25; -- Invalid placement (>20)
        
    PRINT 'Procedure completed successfully (should not happen)';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

-- Test Case 4: Test invalid date
PRINT 'Testing invalid race date (should fail validation):';
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver1',
        @CircuitName = 'TestCircuit2',
        @RaceDate = '2024-07-20', -- Invalid year (not 2025)
        @Placement = 1;
        
    PRINT 'Procedure completed successfully (should not happen)';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

PRINT '=== Testing non-existent entities ===';

-- Test Case 5: Test non-existent driver
PRINT 'Testing non-existent driver:';
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'NonExistentDriver',
        @CircuitName = 'TestCircuit2',
        @RaceDate = '2025-07-20',
        @Placement = 1;
        
    PRINT 'Procedure completed successfully (should not happen)';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

-- Test Case 6: Test non-existent circuit
PRINT 'Testing non-existent circuit:';
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver1',
        @CircuitName = 'NonExistentCircuit',
        @RaceDate = '2025-07-20',
        @Placement = 1;
        
    PRINT 'Procedure completed successfully (should not happen)';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

PRINT '=== Testing partial recovery scenario ===';
PRINT 'Testing race creation with duplicate placement (race should persist even if result fails)';

-- First create a race with placement = 1
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver1',
        @CircuitName = 'TestCircuit2',
        @RaceDate = '2025-09-10',
        @Placement = 1;
        
    PRINT 'Successfully created race with first driver';
END TRY
BEGIN CATCH
    PRINT 'ERROR: ' + ERROR_MESSAGE();
END CATCH
GO

-- Now try to add another driver with same placement (should fail)
BEGIN TRY
    EXEC dbo.InsertRaceResult
        @DriverName = 'TestDriver2',
        @CircuitName = 'TestCircuit2',
        @RaceDate = '2025-09-10',
        @Placement = 1; -- Duplicate placement should cause error
        
    PRINT 'Procedure completed successfully (should not happen)';
END TRY
BEGIN CATCH
    PRINT 'Expected error occurred: ' + ERROR_MESSAGE();
END CATCH
GO

-- Verify the race still exists despite the error
PRINT 'Verifying race was preserved despite result insertion error:';
SELECT r.raceId, r.raceDate, c.circuitName, COUNT(res.resultId) AS Results
FROM race r
JOIN circuit c ON r.circuitId = c.circuitId
LEFT JOIN result res ON r.raceId = res.raceId
WHERE c.circuitName = 'TestCircuit2' AND r.raceDate = '2025-09-10'
GROUP BY r.raceId, r.raceDate, c.circuitName;
GO

PRINT '=== Final verification ===';

-- Show all test races
PRINT 'All test races:';
SELECT r.raceId, r.raceDate, c.circuitName, d.driverName, res.placement
FROM race r
JOIN circuit c ON r.circuitId = c.circuitId
LEFT JOIN result res ON r.raceId = res.raceId
LEFT JOIN driver d ON res.driverId = d.driverId
WHERE c.circuitName LIKE 'Test%'
ORDER BY r.raceDate, res.placement;

-- Show all logs from the test
PRINT 'Test logs:';
SELECT * FROM ActionLog 
WHERE Description LIKE '%Test%' 
ORDER BY LogDateTime DESC;
GO

-- Clean up test data - commented out to preserve test data (will be deleted in the next test run)
/*
DELETE FROM result WHERE raceId IN (SELECT raceId FROM race WHERE circuitId IN (SELECT circuitId FROM circuit WHERE circuitName LIKE 'Test%'));
DELETE FROM race WHERE circuitId IN (SELECT circuitId FROM circuit WHERE circuitName LIKE 'Test%');
DELETE FROM driverChampionship WHERE driverId IN (SELECT driverId FROM driver WHERE driverName LIKE 'Test%');
DELETE FROM driver WHERE driverName LIKE 'Test%';
DELETE FROM circuit WHERE circuitName LIKE 'Test%';
DELETE FROM team WHERE teamName = 'TestTeam';
DELETE FROM constructor WHERE constructorName = 'TestConstructor';
DELETE FROM ActionLog WHERE Description LIKE '%Test%';
GO
*/

PRINT 'Testing completed!'; 