-- Testing script for DirtyReads demo procedures
-- This script will setup required test data and create procedures to demonstrate dirty reads

USE f1;
GO

-- Clear existing test data (if any)
DELETE FROM ActionLog WHERE Description LIKE '%dirty read demo%';
DELETE FROM driverChampionship WHERE driverId IN (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton');
DELETE FROM driver WHERE driverName = 'Lewis Hamilton';
GO

-- First, create test prerequisites - team, driver and championship
PRINT '=== Setting up test prerequisites ===';

-- Insert test constructor if not exists
IF NOT EXISTS (SELECT 1 FROM constructor WHERE constructorId = 1001)
BEGIN
    INSERT INTO constructor (constructorId, constructorName, constructorCountry)
    VALUES (1001, 'TestConstructor1', 'GBR');
END

-- Insert test team if not exists
DECLARE @TeamId INT;
IF NOT EXISTS (SELECT 1 FROM team WHERE teamName = 'Mercedes AMG')
BEGIN
    INSERT INTO team (constructorId, teamName, teamCountry)
    VALUES (1001, 'Mercedes AMG', 'GBR');
    
    SET @TeamId = SCOPE_IDENTITY();
END
ELSE
BEGIN
    SELECT @TeamId = teamId FROM team WHERE teamName = 'Mercedes AMG';
END

-- Insert test driver
IF NOT EXISTS (SELECT 1 FROM driver WHERE driverName = 'Lewis Hamilton')
BEGIN
    INSERT INTO driver (driverName, teamId, nationality, dateOfBirth)
    VALUES ('Lewis Hamilton', @TeamId, 'GBR', '1985-01-07');
    
    -- Add driver to championship with initial points
    INSERT INTO driverChampionship (driverId, points)
    VALUES (SCOPE_IDENTITY(), 100.0);
END

PRINT 'Test prerequisites created successfully';
GO

-- Instructions for running the dirty read demo
PRINT '===================================================';
PRINT 'DIRTY READ DEMONSTRATION INSTRUCTIONS:';
PRINT '1. Open two separate query windows in SQL Server Management Studio';
PRINT '2. In the first window, execute: EXEC DirtyReadDemo1';
PRINT '3. Immediately after, in the second window, execute: EXEC DirtyReadDemo2';
PRINT '4. Observe that the first connection will read uncommitted data that is later rolled back';
PRINT '===================================================';
PRINT '';

-- Query to check logs after running the procedures
PRINT 'After running the demo, execute the following query to check the logs:';
PRINT 'SELECT * FROM ActionLog WHERE Description LIKE ''%dirty read demo%'' ORDER BY LogDateTime;';
PRINT '';

-- Dirty read solution demonstration
PRINT '=== SOLUTION: Using READ COMMITTED isolation level ===';
PRINT 'To avoid dirty reads, use READ COMMITTED isolation level to COMMITED instead of UNCOMMITTED';
PRINT '';

PRINT 'Testing setup completed!'; 