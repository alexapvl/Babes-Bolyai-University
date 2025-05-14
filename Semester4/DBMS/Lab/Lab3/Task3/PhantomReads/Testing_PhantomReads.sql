-- Testing script for PhantomReads demo procedures
-- This script will setup required test data and create procedures to demonstrate phantom reads

USE f1;
GO

-- Clear existing test data (if any)
DELETE FROM ActionLog WHERE Description LIKE '%phantom read demo%';
DELETE FROM driver WHERE driverName = 'New Driver';
GO

-- First, ensure test team exists
PRINT '=== Setting up test prerequisites ===';

-- Verify team with ID 1 exists
IF NOT EXISTS (SELECT 1 FROM team WHERE teamId = 1)
BEGIN
    PRINT 'ERROR: Team with ID 1 does not exist. Please run the database creation script first.';
    RETURN;
END
ELSE
BEGIN
    PRINT 'Team with ID 1 found. Ready for phantom read demo.';
END

PRINT 'Test prerequisites verified successfully';
GO

PRINT '=== Phantom read demo procedures created successfully ===';

-- Instructions for running the phantom read demo
PRINT '===================================================';
PRINT 'PHANTOM READ DEMONSTRATION INSTRUCTIONS:';
PRINT '1. Open two separate query windows in SQL Server Management Studio';
PRINT '2. In the first window, execute: EXEC PhantomReadDemo1';
PRINT '3. Immediately after, in the second window, execute: EXEC PhantomReadDemo2';
PRINT '4. Observe that the first connection will see different row counts in the same transaction';
PRINT '===================================================';
PRINT '';

-- Query to check logs after running the procedures
PRINT 'After running the demo, execute the following query to check the logs:';
PRINT 'SELECT * FROM ActionLog WHERE Description LIKE ''%phantom read demo%'' ORDER BY LogDateTime;';
PRINT '';

-- Phantom read solution demonstration
PRINT '=== SOLUTION: Using SERIALIZABLE isolation level ===';
PRINT 'To avoid phantom reads, use SERIALIZABLE isolation level instead of REPEATABLE READ:';
PRINT '
-- Modified Connection 1 procedure that avoids phantom reads
CREATE PROCEDURE SafeReadDemo AS
BEGIN
    SET TRANSACTION ISOLATION LEVEL SERIALIZABLE; -- Change from REPEATABLE READ
    BEGIN TRANSACTION;
    
    -- First count
    SELECT COUNT(*) FROM driver WHERE teamId = 1;
    
    -- No matter what happens in other transactions, this will return the same count
    SELECT COUNT(*) FROM driver WHERE teamId = 1;
    
    COMMIT TRANSACTION;
END;
';
PRINT '';

PRINT 'Testing setup completed!'; 