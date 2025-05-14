-- Clean up test data for NonRepeatableReads demonstration
USE f1;
GO

-- Check the ActionLog table before deleting
SELECT * FROM ActionLog WHERE Description LIKE '%non-repeatable read demo%' ORDER BY LogDateTime;
GO

DELETE FROM ActionLog WHERE Description LIKE '%non-repeatable read demo%';
DELETE FROM sponsor WHERE sponsorName = 'Red Bull';
GO

PRINT 'Cleanup for Non-Repeatable Reads completed.'; 