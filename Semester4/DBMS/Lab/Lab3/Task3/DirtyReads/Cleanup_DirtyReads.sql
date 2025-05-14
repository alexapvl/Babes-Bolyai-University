-- Clean up test data for DirtyReads demonstration
USE f1;
GO

-- Check the ActionLog table before deleting
SELECT * FROM ActionLog WHERE Description LIKE '%dirty read demo%' ORDER BY LogDateTime;
GO

DELETE FROM ActionLog WHERE Description LIKE '%dirty read demo%';
DELETE FROM driverChampionship WHERE driverId IN (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton');
DELETE FROM driver WHERE driverName = 'Lewis Hamilton';
GO

PRINT 'Cleanup for Dirty Reads completed.';