USE f1;
GO

-- Clear existing data (optional, use with caution in a non-dev environment)
DELETE FROM dhlFastestLap;
DELETE FROM result;
DELETE FROM race;
DELETE FROM circuit;
DELETE FROM driverChampionship;
DELETE FROM driver;
DELETE FROM sponsorship;
DELETE FROM sponsor;
DELETE FROM team;
DELETE FROM constructor;
GO

-- Reset identity seeds (optional)
DBCC CHECKIDENT ('team', RESEED, 0);
DBCC CHECKIDENT ('sponsor', RESEED, 0);
DBCC CHECKIDENT ('sponsorship', RESEED, 0);
DBCC CHECKIDENT ('driver', RESEED, 0);
DBCC CHECKIDENT ('driverChampionship', RESEED, 0);
DBCC CHECKIDENT ('circuit', RESEED, 0);
DBCC CHECKIDENT ('race', RESEED, 0);
DBCC CHECKIDENT ('result', RESEED, 0);
DBCC CHECKIDENT ('dhlFastestLap', RESEED, 0);
GO

-- Populate constructor table (Based on 2025 F1 Teams)
PRINT 'Populating Constructors (2025)...';
INSERT INTO constructor (constructorId, constructorName, constructorCountry) VALUES
(1, 'Red Bull Racing RBPT', 'AUT'),         -- Oracle Red Bull Racing (Engine: RBPT - Red Bull Powertrains)
(2, 'Mercedes', 'GER'),                    -- Mercedes-AMG Petronas F1 Team
(3, 'Ferrari', 'ITA'),                     -- Scuderia Ferrari
(4, 'McLaren Mercedes', 'GBR'),            -- McLaren F1 Team (Engine: Mercedes)
(5, 'Aston Martin Aramco Mercedes', 'GBR'), -- Aston Martin Aramco F1 Team (Engine: Mercedes)
(6, 'Alpine Renault', 'FRA'),               -- BWT Alpine F1 Team (Engine: Renault)
(7, 'Williams Mercedes', 'GBR'),            -- Williams Racing (Engine: Mercedes)
(8, 'RB Honda RBPT', 'ITA'),                -- Visa Cash App RB Formula One Team (Engine: Honda RBPT)
(9, 'Kick Sauber Ferrari', 'SUI'),          -- Stake F1 Team Kick Sauber (Engine: Ferrari)
(10, 'Haas Ferrari', 'USA');                -- MoneyGram Haas F1 Team (Engine: Ferrari)
GO

-- Populate team table (Based on 2025 F1 Teams)
PRINT 'Populating Teams (2025)...';
INSERT INTO team (constructorId, teamName, teamCountry) VALUES
((SELECT constructorId FROM constructor WHERE constructorName = 'Red Bull Racing RBPT'), 'Oracle Red Bull Racing', 'AUT'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Mercedes'), 'Mercedes-AMG Petronas F1 Team', 'GBR'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Ferrari'), 'Scuderia Ferrari HP', 'ITA'), -- Updated with HP sponsor
((SELECT constructorId FROM constructor WHERE constructorName = 'McLaren Mercedes'), 'McLaren Formula 1 Team', 'GBR'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Aston Martin Aramco Mercedes'), 'Aston Martin Aramco Formula One Team', 'GBR'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Alpine Renault'), 'BWT Alpine F1 Team', 'FRA'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Williams Mercedes'), 'Williams Racing', 'GBR'),
((SELECT constructorId FROM constructor WHERE constructorName = 'RB Honda RBPT'), 'Visa Cash App RB Formula One Team', 'ITA'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Kick Sauber Ferrari'), 'Stake F1 Team Kick Sauber', 'SUI'),
((SELECT constructorId FROM constructor WHERE constructorName = 'Haas Ferrari'), 'MoneyGram Haas F1 Team', 'USA');
GO

-- Populate sponsor table (Generic and major sponsors)
PRINT 'Populating Sponsors...';
INSERT INTO sponsor (sponsorName, sponsorCountry) VALUES
('Oracle', 'USA'), ('Red Bull', 'AUT'),
('Petronas', 'MYS'), ('INEOS', 'GBR'),
('HP', 'USA'), ('Shell', 'NLD'), ('Puma', 'GER'),
('Google Chrome', 'USA'), ('British American Tobacco (Vuse)', 'GBR'),
('Aramco', 'SAU'), ('Cognizant', 'USA'),
('BWT', 'AUT'),
('Komatsu', 'JPN'), ('Duracell', 'USA'),
('Visa', 'USA'), ('Cash App', 'USA'),
('Stake', 'AUS'), ('Kick', 'AUS'),
('MoneyGram', 'USA');
GO

-- Populate sponsorship table (Illustrative based on team names and major partners)
PRINT 'Populating Sponsorships (2025)...';
INSERT INTO sponsorship (teamId, sponsorId) VALUES
((SELECT teamId FROM team WHERE teamName = 'Oracle Red Bull Racing'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Oracle')),
((SELECT teamId FROM team WHERE teamName = 'Oracle Red Bull Racing'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Red Bull')),
((SELECT teamId FROM team WHERE teamName = 'Mercedes-AMG Petronas F1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Petronas')),
((SELECT teamId FROM team WHERE teamName = 'Mercedes-AMG Petronas F1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'INEOS')),
((SELECT teamId FROM team WHERE teamName = 'Scuderia Ferrari HP'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'HP')),
((SELECT teamId FROM team WHERE teamName = 'Scuderia Ferrari HP'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Shell')),
((SELECT teamId FROM team WHERE teamName = 'McLaren Formula 1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Google Chrome')),
((SELECT teamId FROM team WHERE teamName = 'McLaren Formula 1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'British American Tobacco (Vuse)')),
((SELECT teamId FROM team WHERE teamName = 'Aston Martin Aramco Formula One Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Aramco')),
((SELECT teamId FROM team WHERE teamName = 'Aston Martin Aramco Formula One Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Cognizant')),
((SELECT teamId FROM team WHERE teamName = 'BWT Alpine F1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'BWT')),
((SELECT teamId FROM team WHERE teamName = 'Williams Racing'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Komatsu')),
((SELECT teamId FROM team WHERE teamName = 'Visa Cash App RB Formula One Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Visa')),
((SELECT teamId FROM team WHERE teamName = 'Visa Cash App RB Formula One Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Cash App')),
((SELECT teamId FROM team WHERE teamName = 'Stake F1 Team Kick Sauber'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Stake')),
((SELECT teamId FROM team WHERE teamName = 'Stake F1 Team Kick Sauber'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'Kick')),
((SELECT teamId FROM team WHERE teamName = 'MoneyGram Haas F1 Team'), (SELECT sponsorId FROM sponsor WHERE sponsorName = 'MoneyGram'));
GO

-- Populate driver table (Based on 2025 F1 Drivers, hypothetical full lineup)
PRINT 'Populating Drivers (2025)...';
INSERT INTO driver (driverName, teamId, nationality, dateOfBirth) VALUES
-- Oracle Red Bull Racing
('Max Verstappen', (SELECT teamId FROM team WHERE teamName = 'Oracle Red Bull Racing'), 'NLD', '1997-09-30'),
('Sergio Pérez', (SELECT teamId FROM team WHERE teamName = 'Oracle Red Bull Racing'), 'MEX', '1990-01-26'), 
-- Mercedes-AMG Petronas F1 Team
('George Russell', (SELECT teamId FROM team WHERE teamName = 'Mercedes-AMG Petronas F1 Team'), 'GBR', '1998-02-15'),
('Kimi Antonelli', (SELECT teamId FROM team WHERE teamName = 'Mercedes-AMG Petronas F1 Team'), 'ITA', '2006-08-25'), -- Assuming Antonelli, actual may vary
-- Scuderia Ferrari HP
('Charles Leclerc', (SELECT teamId FROM team WHERE teamName = 'Scuderia Ferrari HP'), 'MON', '1997-10-16'),
('Lewis Hamilton', (SELECT teamId FROM team WHERE teamName = 'Scuderia Ferrari HP'), 'GBR', '1985-01-07'),
-- McLaren Formula 1 Team
('Lando Norris', (SELECT teamId FROM team WHERE teamName = 'McLaren Formula 1 Team'), 'GBR', '1999-11-13'),
('Oscar Piastri', (SELECT teamId FROM team WHERE teamName = 'McLaren Formula 1 Team'), 'AUS', '2001-04-06'),
-- Aston Martin Aramco Formula One Team
('Fernando Alonso', (SELECT teamId FROM team WHERE teamName = 'Aston Martin Aramco Formula One Team'), 'ESP', '1981-07-29'),
('Lance Stroll', (SELECT teamId FROM team WHERE teamName = 'Aston Martin Aramco Formula One Team'), 'CAN', '1998-10-29'),
-- BWT Alpine F1 Team
('Pierre Gasly', (SELECT teamId FROM team WHERE teamName = 'BWT Alpine F1 Team'), 'FRA', '1996-02-07'),
('Victor Martins', (SELECT teamId FROM team WHERE teamName = 'BWT Alpine F1 Team'), 'FRA', '2001-06-16'), -- Assuming Martins, actual may vary
-- Williams Racing
('Alexander Albon', (SELECT teamId FROM team WHERE teamName = 'Williams Racing'), 'THA', '1996-03-23'),
('Carlos Sainz Jr.', (SELECT teamId FROM team WHERE teamName = 'Williams Racing'), 'ESP', '1994-09-01'), -- Assuming Sainz to Williams
-- Visa Cash App RB Formula One Team
('Yuki Tsunoda', (SELECT teamId FROM team WHERE teamName = 'Visa Cash App RB Formula One Team'), 'JPN', '2000-05-11'),
('Liam Lawson', (SELECT teamId FROM team WHERE teamName = 'Visa Cash App RB Formula One Team'), 'NZL', '2002-02-11'), -- Assuming Lawson, actual may vary
-- Stake F1 Team Kick Sauber
('Nico Hülkenberg', (SELECT teamId FROM team WHERE teamName = 'Stake F1 Team Kick Sauber'), 'GER', '1987-08-19'),
('Valtteri Bottas', (SELECT teamId FROM team WHERE teamName = 'Stake F1 Team Kick Sauber'), 'FIN', '1989-08-28'), -- Assuming Bottas stays, actual may vary
-- MoneyGram Haas F1 Team
('Kevin Magnussen', (SELECT teamId FROM team WHERE teamName = 'MoneyGram Haas F1 Team'), 'DEN', '1992-10-05'), -- Assuming Magnussen stays, actual may vary
('Oliver Bearman', (SELECT teamId FROM team WHERE teamName = 'MoneyGram Haas F1 Team'), 'GBR', '2005-05-08'); -- Assuming Bearman
GO

-- Populate driverChampionship table (Initial points for all drivers)
PRINT 'Populating Driver Championships (2025 - Initial Points)...';
INSERT INTO driverChampionship (driverId, points) 
SELECT driverId, 0.0 FROM driver;
GO

-- Populate circuit table (Known circuits for early 2025 season)
PRINT 'Populating Circuits (2025)...';
INSERT INTO circuit (circuitName, circuitCountry, lapLength) VALUES
('Albert Park Circuit', 'AUS', 5.3),              -- Australia (Season Opener)
('Shanghai International Circuit', 'CHN', 5.5),
('Suzuka International Racing Course', 'JPN', 5.8),
('Bahrain International Circuit', 'BHR', 5.4),
('Jeddah Street Circuit', 'SAU', 6.2),
('Miami International Autodrome', 'USA', 5.4);
-- Add more circuits as the 2025 calendar is confirmed
GO

-- Populate race table (Actual 2025 Races up to Miami)
PRINT 'Populating Races (2025 up to Miami)...';
INSERT INTO race (circuitId, raceDate, numberOfLaps) VALUES
((SELECT circuitId FROM circuit WHERE circuitName = 'Albert Park Circuit'), '2025-03-16', 58),
((SELECT circuitId FROM circuit WHERE circuitName = 'Shanghai International Circuit'), '2025-03-23', 56),
((SELECT circuitId FROM circuit WHERE circuitName = 'Suzuka International Racing Course'), '2025-04-06', 53),
((SELECT circuitId FROM circuit WHERE circuitName = 'Bahrain International Circuit'), '2025-04-13', 57),
((SELECT circuitId FROM circuit WHERE circuitName = 'Jeddah Street Circuit'), '2025-04-20', 50),
((SELECT circuitId FROM circuit WHERE circuitName = 'Miami International Autodrome'), '2025-05-04', 57);
GO

-- Populate result table (Illustrative results based on available data up to Miami 2025)
PRINT 'Populating Results (2025 - Up to Miami - Illustrative)...';

-- Race 1: Australian GP (Example Results)
DECLARE @RaceIdAus INT = (SELECT raceId FROM race WHERE raceDate = '2025-03-16');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdAus, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 1),
(@RaceIdAus, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 2),
(@RaceIdAus, (SELECT driverId FROM driver WHERE driverName = 'Lando Norris'), 3),
(@RaceIdAus, (SELECT driverId FROM driver WHERE driverName = 'George Russell'), 4),
(@RaceIdAus, (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), 5);
-- ... Add other drivers for Australian GP

-- Race 2: Chinese GP (Example Results)
DECLARE @RaceIdChn INT = (SELECT raceId FROM race WHERE raceDate = '2025-03-23');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdChn, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 1),
(@RaceIdChn, (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), 2),
(@RaceIdChn, (SELECT driverId FROM driver WHERE driverName = 'Oscar Piastri'), 3),
(@RaceIdChn, (SELECT driverId FROM driver WHERE driverName = 'Sergio Pérez'), 4),
(@RaceIdChn, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 5);
-- ... Add other drivers for Chinese GP

-- Race 3: Japanese GP (Example Results)
DECLARE @RaceIdJpn INT = (SELECT raceId FROM race WHERE raceDate = '2025-04-06');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdJpn, (SELECT driverId FROM driver WHERE driverName = 'Lando Norris'), 1),
(@RaceIdJpn, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 2),
(@RaceIdJpn, (SELECT driverId FROM driver WHERE driverName = 'George Russell'), 3),
(@RaceIdJpn, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 4),
(@RaceIdJpn, (SELECT driverId FROM driver WHERE driverName = 'Fernando Alonso'), 5);
-- ... Add other drivers for Japanese GP

-- Race 4: Bahrain GP (Example Results)
DECLARE @RaceIdBhr INT = (SELECT raceId FROM race WHERE raceDate = '2025-04-13');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdBhr, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 1),
(@RaceIdBhr, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 2),
(@RaceIdBhr, (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), 3),
(@RaceIdBhr, (SELECT driverId FROM driver WHERE driverName = 'Lando Norris'), 4),
(@RaceIdBhr, (SELECT driverId FROM driver WHERE driverName = 'George Russell'), 5);
-- ... Add other drivers for Bahrain GP

-- Race 5: Saudi Arabian GP (Example Results)
DECLARE @RaceIdSau INT = (SELECT raceId FROM race WHERE raceDate = '2025-04-20');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdSau, (SELECT driverId FROM driver WHERE driverName = 'Sergio Pérez'), 1),
(@RaceIdSau, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 2),
(@RaceIdSau, (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), 3),
(@RaceIdSau, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 4),
(@RaceIdSau, (SELECT driverId FROM driver WHERE driverName = 'Oscar Piastri'), 5);
-- ... Add other drivers for Saudi Arabian GP

-- Race 6: Miami GP (Example Results)
DECLARE @RaceIdMia INT = (SELECT raceId FROM race WHERE raceDate = '2025-05-04');
INSERT INTO result (raceId, driverId, placement) VALUES
(@RaceIdMia, (SELECT driverId FROM driver WHERE driverName = 'Lando Norris'), 1),
(@RaceIdMia, (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), 2),
(@RaceIdMia, (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), 3),
(@RaceIdMia, (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), 4),
(@RaceIdMia, (SELECT driverId FROM driver WHERE driverName = 'George Russell'), 5);
-- ... Add other drivers for Miami GP. Fill all 20 placements for each race for complete data.
GO

-- Update driverChampionship points based on illustrative results
-- This is a simplified points update. Real F1 scoring is more complex (e.g., points for fastest lap if in top 10).
PRINT 'Updating Driver Championship Points (Illustrative)...';

DECLARE @Points TABLE (Placement INT PRIMARY KEY, PointsAwarded FLOAT);
INSERT INTO @Points (Placement, PointsAwarded) VALUES
(1, 25), (2, 18), (3, 15), (4, 12), (5, 10), (6, 8), (7, 6), (8, 4), (9, 2), (10, 1);

UPDATE dc
SET dc.points = dc.points + ISNULL(p.PointsAwarded, 0)
FROM driverChampionship dc
JOIN driver d ON dc.driverId = d.driverId
JOIN result r ON d.driverId = r.driverId
JOIN @Points p ON r.placement = p.Placement
WHERE r.raceId IN (
    (SELECT raceId FROM race WHERE raceDate = '2025-03-16'),
    (SELECT raceId FROM race WHERE raceDate = '2025-03-23'),
    (SELECT raceId FROM race WHERE raceDate = '2025-04-06'),
    (SELECT raceId FROM race WHERE raceDate = '2025-04-13'),
    (SELECT raceId FROM race WHERE raceDate = '2025-04-20'),
    (SELECT raceId FROM race WHERE raceDate = '2025-05-04')
);
GO

-- Populate dhlFastestLap table (Based on F1 2025 fastest laps data up to Miami)
PRINT 'Populating DHL Fastest Lap (2025 - Up to Miami)...';
INSERT INTO dhlFastestLap (raceId, driverId, fastestLapTime) VALUES
-- Australian GP (Example: Leclerc, Ferrari)
((SELECT raceId FROM race WHERE raceDate = '2025-03-16'), (SELECT driverId FROM driver WHERE driverName = 'Charles Leclerc'), '1:20.260'),
-- Chinese GP (Example: Verstappen, Red Bull)
((SELECT raceId FROM race WHERE raceDate = '2025-03-23'), (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), '1:35.860'),
-- Japanese GP (Example: Norris, McLaren)
((SELECT raceId FROM race WHERE raceDate = '2025-04-06'), (SELECT driverId FROM driver WHERE driverName = 'Lando Norris'), '1:33.123'),
-- Bahrain GP (Example: Hamilton, Ferrari)
((SELECT raceId FROM race WHERE raceDate = '2025-04-13'), (SELECT driverId FROM driver WHERE driverName = 'Lewis Hamilton'), '1:31.447'),
-- Saudi Arabian GP (Example: Verstappen, Red Bull)
((SELECT raceId FROM race WHERE raceDate = '2025-04-20'), (SELECT driverId FROM driver WHERE driverName = 'Max Verstappen'), '1:30.734'),
-- Miami GP (Example: Piastri, McLaren)
((SELECT raceId FROM race WHERE raceDate = '2025-05-04'), (SELECT driverId FROM driver WHERE driverName = 'Oscar Piastri'), '1:29.708');
-- Add 1 point to driver championship for fastest lap if they are in top 10 - simplified here
-- Note: The F1 rules give a point for fastest lap only if the driver finishes in the top 10.
-- This script does not currently implement that specific logic for points updates, but the fastest lap itself is recorded.
GO

PRINT 'Seed data population script for 2025 F1 season (up to Miami) finished.';
PRINT 'Please review and expand with more complete/accurate 2025 data as it becomes available. This current version is up until the Miami Grand Prix(included)';
GO 