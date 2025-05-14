-- Execute in a fourth connection window to create a conflict with the solution
USE f1;
GO

-- Update that will conflict with the solution procedure
UPDATE driverChampionship SET points = points + 10 WHERE driverId = 1;
PRINT 'Executed conflicting update: points + 10';
GO