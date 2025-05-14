use f1;
GO

SELECT * FROM ActionLog WHERE Description LIKE '%deadlock demo%'
GO

DELETE FROM ActionLog WHERE Description LIKE '%deadlock demo%'
DELETE FROM driver WHERE driverName LIKE 'TestDriver%'
DELETE FROM constructor WHERE constructorName LIKE 'TestConstructor%'
GO

PRINT 'Cleanup for Deadlock demo completed.';