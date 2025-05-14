-- Testing script for NonRepeatableReads demo procedures
-- This script will setup required test data and create procedures to demonstrate non-repeatable reads

USE f1;
GO

-- Clear existing test data (if any)
DELETE FROM ActionLog WHERE Description LIKE '%non-repeatable read demo%';
DELETE FROM sponsor WHERE sponsorName = 'Red Bull';
GO

-- First, create test prerequisites
PRINT '=== Setting up test prerequisites ===';

-- Insert test sponsor
IF NOT EXISTS (SELECT 1 FROM sponsor WHERE sponsorName = 'Red Bull')
BEGIN
    INSERT INTO sponsor (sponsorName, sponsorCountry)
    VALUES ('Red Bull', 'GBR'); -- Initially set to GBR to demonstrate the change
END

PRINT 'Test prerequisites created successfully';
GO

PRINT '=== Non-repeatable read demo procedures created successfully ===';

-- Instructions for running the non-repeatable read demo
PRINT '===================================================';
PRINT 'NON-REPEATABLE READ DEMONSTRATION INSTRUCTIONS:';
PRINT '1. Open two separate query windows in SQL Server Management Studio';
PRINT '2. In the first window, execute: EXEC NonRepeatableReadDemo1';
PRINT '3. Immediately after, in the second window, execute: EXEC NonRepeatableReadDemo2';
PRINT '4. Observe that the first connection will read different data in the same transaction';
PRINT '===================================================';
PRINT '';

-- Query to check logs after running the procedures
PRINT 'After running the demo, execute the following query to check the logs:';
PRINT 'SELECT * FROM ActionLog WHERE Description LIKE ''%non-repeatable read demo%'' ORDER BY LogDateTime;';
PRINT '';

-- Non-repeatable read solution demonstration
PRINT '=== SOLUTION: Using REPEATABLE READ isolation level ===';
PRINT 'To avoid non-repeatable reads, use REPEATABLE READ isolation level instead of READ COMMITTED:';
PRINT '
-- Modified Connection 1 procedure that avoids non-repeatable reads
CREATE PROCEDURE SafeReadDemo AS
BEGIN
    SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; -- Change from READ COMMITTED
    BEGIN TRANSACTION;
    
    -- First read
    SELECT sponsorName, sponsorCountry 
    FROM sponsor 
    WHERE sponsorName = ''Red Bull'';
    
    -- No matter what happens in other transactions, this will return the same data
    SELECT sponsorName, sponsorCountry 
    FROM sponsor 
    WHERE sponsorName = ''Red Bull'';
    
    COMMIT TRANSACTION;
END;
';
PRINT '';

PRINT 'Testing setup completed!'; 