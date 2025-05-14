-- Execute in a third connection window to demonstrate the solution
USE f1;
GO

-- Reset the points to a known value first
UPDATE driverChampionship SET points = 100.0 WHERE driverId = 1;
PRINT 'Points reset to 100.0 for testing';
GO

-- Execute the solution with retry logic
EXEC OptimisticConflictSolution;