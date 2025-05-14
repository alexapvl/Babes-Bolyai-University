-- Testing script for Deadlock demo procedures
-- This script will setup required test data and create procedures to demonstrate deadlocks

USE f1;
GO

-- Clear existing test data (if any)
DELETE FROM ActionLog WHERE Description LIKE '%deadlock demo%';
DELETE FROM driver WHERE driverName LIKE 'TestDriver%';
DELETE FROM constructor WHERE constructorName LIKE 'TestConstructor%';
GO

-- First, create test prerequisites - constructor and driver
PRINT '=== Setting up test prerequisites ===';

-- Insert test constructor
IF NOT EXISTS (SELECT 1 FROM constructor WHERE constructorId = 1001)
BEGIN
    INSERT INTO constructor (constructorId, constructorName, constructorCountry)
    VALUES (1001, 'TestConstructor1', 'USA');
END

-- Insert test team
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

-- Insert test driver
IF NOT EXISTS (SELECT 1 FROM driver WHERE driverName = 'TestDriver1')
BEGIN
    INSERT INTO driver (driverName, teamId, nationality, dateOfBirth)
    VALUES ('TestDriver1', @TeamId, 'USA', '1990-01-01');
END

PRINT 'Test prerequisites created successfully';
GO


PRINT '=== Deadlock demo procedures created successfully ===';

-- Instructions for running the deadlock demo
PRINT '===================================================';
PRINT 'DEADLOCK DEMONSTRATION INSTRUCTIONS:';
PRINT '1. Open two separate query windows in SQL Server Management Studio';
PRINT '2. In the first window, execute: EXEC DeadlockDemo1';
PRINT '3. Immediately after, in the second window, execute: EXEC DeadlockDemo2';
PRINT '4. Observe that one of the connections will be chosen as a deadlock victim';
PRINT '   and will receive a deadlock error (error 1205)';
PRINT '===================================================';
PRINT '';

-- Query to check logs after running the procedures
PRINT 'After running the demo, execute the following query to check the logs:';
PRINT 'SELECT * FROM ActionLog WHERE Description LIKE ''%deadlock demo%'' ORDER BY LogDateTime;';
PRINT '';

-- Deadlock solution demonstration
PRINT '=== SOLUTION: Using consistent order of resource access ===';
PRINT 'To avoid deadlocks, resources should be accessed in a consistent order.';
PRINT 'For example, always update constructor before driver:';
PRINT '
BEGIN TRANSACTION;
    -- Always lock constructor table first
    UPDATE constructor
    SET constructorName = constructorName
    WHERE constructorId = 1001;
    
    -- Then lock driver table 
    UPDATE driver
    SET driverName = driverName
    WHERE driverId = (SELECT TOP 1 driverId FROM driver WHERE driverName = ''TestDriver1'');
COMMIT TRANSACTION;
';

PRINT 'Alternatively, use SNAPSHOT isolation level to avoid locking:';
PRINT '
SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRANSACTION;
    -- Operations here won''t take locks
COMMIT TRANSACTION;
';
PRINT '';

PRINT 'Testing setup completed!'; 