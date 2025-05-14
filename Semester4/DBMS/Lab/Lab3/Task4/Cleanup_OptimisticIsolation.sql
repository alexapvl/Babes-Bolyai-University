-- Clean up test data for OptimisticIsolation demonstration
USE f1;
GO

-- Check the ActionLog table before deleting
SELECT * FROM ActionLog WHERE Description LIKE '%optimistic conflict demo%' ORDER BY LogDateTime;
GO

-- Check current points
SELECT d.driverName, dc.points 
FROM driverChampionship dc 
JOIN driver d ON dc.driverId = d.driverId 
WHERE d.driverId = 1;
GO

-- Clean up logs and reset points
DELETE FROM ActionLog WHERE Description LIKE '%optimistic conflict%';
UPDATE driverChampionship SET points = 100.0 WHERE driverId = 1;
GO

PRINT 'Cleanup for Optimistic Isolation completed.'; 