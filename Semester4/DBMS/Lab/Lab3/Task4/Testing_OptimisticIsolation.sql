-- Testing script for OptimisticIsolation demo procedures
-- This script will setup required test data and create procedures to demonstrate update conflicts in optimistic isolation

USE f1;
GO

-- Enable SNAPSHOT isolation for the database if not already enabled
IF NOT EXISTS (SELECT 1 FROM sys.databases WHERE name = 'f1' AND snapshot_isolation_state = 1)
BEGIN
    ALTER DATABASE f1 SET ALLOW_SNAPSHOT_ISOLATION ON;
    PRINT 'SNAPSHOT isolation has been enabled for the f1 database.';
END
ELSE
BEGIN
    PRINT 'SNAPSHOT isolation is already enabled for the f1 database.';
END
GO

-- Clear existing test data (if any)
DELETE FROM ActionLog WHERE Description LIKE '%optimistic conflict demo%';
GO

-- First, ensure test driver championship exists
PRINT '=== Setting up test prerequisites ===';

-- Insert test constructor if not exists
IF NOT EXISTS (SELECT 1 FROM constructor WHERE constructorId = 1001)
BEGIN
    INSERT INTO constructor (constructorId, constructorName, constructorCountry)
    VALUES (1001, 'TestConstructor1', 'USA');
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

-- Insert test driver if not exists
IF NOT EXISTS (SELECT 1 FROM driver WHERE driverId = 1)
BEGIN
    INSERT INTO driver (driverId, driverName, teamId, nationality, dateOfBirth)
    VALUES (1, 'TestDriver1', @TeamId, 'USA', '1990-01-01');
END

-- Insert or reset driver championship with initial points
IF NOT EXISTS (SELECT 1 FROM driverChampionship WHERE driverId = 1)
BEGIN
    INSERT INTO driverChampionship (driverId, points)
    VALUES (1, 100.0);
END
ELSE
BEGIN
    UPDATE driverChampionship SET points = 100.0 WHERE driverId = 1;
END

PRINT 'Test prerequisites created successfully';
GO

-- Instructions for running the optimistic conflict demo
PRINT '===================================================';
PRINT 'OPTIMISTIC ISOLATION UPDATE CONFLICT DEMONSTRATION INSTRUCTIONS:';
PRINT '1. Open two separate query windows in SQL Server Management Studio';
PRINT '2. In the first window, execute: EXEC OptimisticConflictDemo1';
PRINT '3. Immediately after, in the second window, execute: EXEC OptimisticConflictDemo2';
PRINT '4. Observe that the first connection will receive an update conflict error';
PRINT '   because the data was modified by another transaction since it was read';
PRINT '===================================================';
PRINT '';

-- Query to check logs after running the procedures
PRINT 'After running the demo, execute the following query to check the logs:';
PRINT 'SELECT * FROM ActionLog WHERE Description LIKE ''%optimistic conflict demo%'' ORDER BY LogDateTime;';
PRINT '';

-- Query to check current points
PRINT 'To check the current points:';
PRINT 'SELECT d.driverName, dc.points FROM driverChampionship dc JOIN driver d ON dc.driverId = d.driverId WHERE d.driverId = 1;';
PRINT '';

-- Solution demonstration
PRINT '=== SOLUTION: Using retry logic with TRY-CATCH ===';
PRINT 'To handle optimistic concurrency conflicts, use retry logic:';
PRINT '1. Open a new query window and execute: EXEC OptimisticConflictSolution';
PRINT '2. In another window, quickly execute an update that will conflict:';
PRINT '   UPDATE driverChampionship SET points = points + 10 WHERE driverId = 1;';
PRINT '3. Observe how the solution procedure retries and eventually succeeds';
PRINT '';

-- Clean up test data (commented out to preserve test data)
/*
DELETE FROM ActionLog WHERE Description LIKE '%optimistic conflict demo%';
UPDATE driverChampionship SET points = 100.0 WHERE driverId = 1;
GO
*/

PRINT 'Testing setup completed!'; 