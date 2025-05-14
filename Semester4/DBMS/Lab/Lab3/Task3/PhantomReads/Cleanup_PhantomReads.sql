-- Clean up test data for PhantomReads demonstration
USE f1;
GO

-- Check the ActionLog table before deleting
SELECT * FROM ActionLog WHERE Description LIKE '%phantom read demo%' ORDER BY LogDateTime;
GO

DELETE FROM ActionLog WHERE Description LIKE '%phantom read demo%';
DELETE FROM driver WHERE driverName = 'New Driver';
GO

PRINT 'Cleanup for Phantom Reads completed.';