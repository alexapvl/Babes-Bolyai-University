use f1;

INSERT INTO constructor (constructorId, constructorName, constructorCountry) VALUES
(1, 'Mercedes', 'DEU'),
(2, 'Honda Red Bull Powertrains', 'JPN'),
(3, 'Ferrari', 'ITA'),
(4, 'Renult', 'FRA');

INSERT INTO team (teamId, constructorId, teamName, teamCountry) VALUES
(1, 1, 'McLaren Mercedes', 'GBR'),
(2, 2, 'Red Bull Racing Honda RBPT', 'AUT'),
(3, 3, 'Ferrari', 'ITA'),
(4, 1, 'Mercedes', 'DEU'),
(5, 1, 'Aston Martin Aramco Mercedes', 'GBR'),
(6, 3, 'Haas Ferrari', 'USA'),
(7, 2, 'RB Honda RBPT', 'JPN'),
(8, 1, 'Williams Mercedes', 'GBR'),
(9, 4, 'Alpine Renault', 'FRA'),
(10, 3, 'Kick Sauber Ferrari', 'CHE');

INSERT INTO driver (driverId, driverName, teamId, nationality, dateOfBirth) VALUES
-- McLaren Mercedes (team_id: 1)
(1, 'Lando Norris', 1, 'GBR', '1999-11-13'),
(2, 'Oscar Piastri', 1, 'AUS', '2001-04-06'),

-- Red Bull Racing Honda RBPT (team_id: 2)
(3, 'Max Verstappen', 2, 'NED', '1997-09-30'),
(4, 'Sergio Perez', 2, 'MEX', '1990-01-26'),

-- Ferrari (team_id: 3)
(5, 'Charles Leclerc', 3, 'MON', '1997-10-16'),
(6, 'Carlos Sainz', 3, 'ESP', '1994-09-01'),

-- Mercedes (team_id: 4)
(7, 'Lewis Hamilton', 4, 'GBR', '1985-01-07'),
(8, 'George Russell', 4, 'GBR', '1998-02-15'),

-- Aston Martin Aramco Mercedes (team_id: 5)
(9, 'Fernando Alonso', 5, 'ESP', '1981-07-29'),
(10, 'Lance Stroll', 5, 'CAN', '1998-10-29'),

-- Haas Ferrari (team_id: 6)
(11, 'Nico Hulkenberg', 6, 'GER', '1987-08-19'),
(12, 'Kevin Magnussen', 6, 'DEN', '1992-10-05'),
(13, 'Oliver Bearman', 6, 'GBR', '2005-05-08'),

-- RB Honda RBPT (team_id: 7)
(14, 'Yuki Tsunoda', 7, 'JPN', '2000-05-11'),
(15, 'Daniel Ricciardo', 7, 'AUS', '1989-07-01'),
(16, 'Liam Lawson', 7, 'NZL', '2002-02-11'),

-- Williams Mercedes (team_id: 8)
(17, 'Alex Albon', 8, 'THA', '1996-03-23'),
(18, 'Franco Colapinto', 8, 'ARG', '2003-05-27'),

-- Alpine Renault (team_id: 9)
(19, 'Pierre Gasly', 9, 'FRA', '1996-02-07'),
(20, 'Esteban Ocon', 9, 'FRA', '1996-09-17'),

-- Kick Sauber Ferrari (team_id: 10)
(21, 'Zhou Guanyu', 10, 'CHN', '1999-05-30'),
(22, 'Logan Sargeant', 10, 'USA', '2000-12-31'),
(23, 'Valtteri Bottas', 10, 'FIN', '1989-08-28');


INSERT INTO driverChampionship (dchampionshipId, driverId, points) values 
(1, 1, 297),
(2, 2, 247),

(3, 3, 354),
(4, 4, 150),

(5, 5, 275),
(6, 6, 215),

(7, 7, 177),
(8, 8, 167),

(9, 9, 62),
(10, 10, 24),

(11, 11, 29),
(12, 12, 8),
(13, 13, 7),

(14, 14, 22),
(15, 15, 12),
(16, 16, 2),

(17, 17, 12),
(18, 18, 5),

(19, 19, 8),
(20, 20, 5),

(21, 21,  0),
(22, 22, 0),
(23, 23, 0);

INSERT INTO circuit (circuitId, circuitName, circuitCountry, lapLength) VALUES
(1, 'Melbourne Grand Prix Circuit', 'AUS', 5.278),
(2, 'Shanghai International Circuit', 'CHN', 5.451),
(3, 'Suzuka International Racing Course', 'JPN', 5.807),
(4, 'Bahrain International Circuit', 'BHR', 5.412),
(5, 'Jeddah Street Circuit', 'SAU', 6.174),
(6, 'Miami International Autodrome', 'USA', 5.412),
(7, 'Imola Circuit', 'ITA', 4.909),
(8, 'Circuit de Monaco', 'MCO', 3.337),
(9, 'Circuit de Barcelona-Catalunya', 'ESP', 4.657),
(10, 'Circuit Gilles Villeneuve', 'CAN', 4.361),
(11, 'Red Bull Ring', 'AUT', 4.318),
(12, 'Silverstone Circuit', 'GBR', 5.891),
(13, 'Circuit de Spa-Francorchamps', 'BEL', 7.004),
(14, 'Hungaroring', 'HUN', 4.381),
(15, 'Circuit Zandvoort', 'NED', 4.259),
(16, 'Autodromo Nazionale Monza', 'ITA', 5.793),
(17, 'Baku City Circuit', 'AZE', 6.003),
(18, 'Marina Bay Street Circuit', 'SGP', 4.940),
(19, 'Circuit of the Americas', 'USA', 5.513),
(20, 'Autódromo Hermanos Rodríguez', 'MEX', 4.304),
(21, 'Interlagos Circuit', 'BRA', 4.309),
(22, 'Las Vegas Street Circuit', 'USA', 6.201),
(23, 'Lusail International Circuit', 'QAT', 5.419),
(24, 'Yas Marina Circuit', 'UAE', 5.281);

INSERT INTO race (raceId, circuitId, raceDate, numberOfLaps) VALUES
(1, 1, '2024-03-24', 58),  -- Melbourne
(2, 2, '2024-04-21', 56),  -- Shanghai
(3, 3, '2024-04-07', 53),  -- Suzuka
(4, 4, '2024-03-02', 57),  -- Bahrain
(5, 5, '2024-03-09', 50),  -- Jeddah
(6, 6, '2024-05-05', 57),  -- Miami
(7, 7, '2024-05-19', 63),  -- Imola
(8, 8, '2024-05-26', 78),  -- Monaco
(9, 9, '2024-06-23', 66),  -- Barcelona
(10, 10, '2024-06-09', 70), -- Montreal
(11, 11, '2024-06-30', 71), -- Spielberg
(12, 12, '2024-07-07', 52), -- Silverstone
(13, 13, '2024-07-28', 44), -- Spa
(14, 14, '2024-07-21', 70), -- Budapest
(15, 15, '2024-08-25', 72), -- Zandvoort
(16, 16, '2024-09-01', 53), -- Monza
(17, 17, '2024-09-15', 51), -- Baku
(18, 18, '2024-09-22', 62), -- Singapore
(19, 19, '2024-10-20', 56), -- Austin
(20, 20, '2024-10-27', 71), -- Mexico City
(21, 21, '2024-11-03', 71), -- Sao Paulo
(22, 22, '2024-11-24', 50), -- Las Vegas
(23, 23, '2024-12-01', 57), -- Lusail
(24, 24, '2024-12-08', 58); -- Yas Marina

INSERT INTO sponsor (sponsorId, sponsorName, sponsorCountry) VALUES
(1, 'Petronas', 'MYS'),
(2, 'Shell', 'NLD'),
(3, 'Red Bull', 'AUT'),
(4, 'Hilton', 'USA'),
(5, 'Cognizant', 'USA');

INSERT INTO sponsorship (sponsorshipId, teamId, sponsorId) VALUES
(1, 1, 1), -- Mercedes AMG Petronas and Petronas
(2, 2, 2), -- Scuderia Ferrari and Shell
(3, 3, 3), -- Red Bull Racing and Red Bull
(4, 4, 4), -- McLaren Racing and Hilton
(5, 5, 5); -- Aston Martin Cognizant and Cognizant

-- Inserts for the Sao Paulo Grand Prix, raceId = 21
INSERT INTO result (resultId, raceId, driverId, placement) VALUES
(1, 21, 3, 1),  -- Max Verstappen, 1st place
(2, 21, 20, 2), -- Esteban Ocon, 2nd place
(3, 21, 19, 3), -- Pierre Gasly, 3rd place
(4, 21, 8, 4),  -- George Russell, 4th place
(5, 21, 5, 5),  -- Charles Leclerc, 5th place
(6, 21, 1, 6),  -- Lando Norris, 6th place
(7, 21, 14, 7), -- Yuki Tsunoda, 7th place
(8, 21, 2, 8),  -- Oscar Piastri, 8th place
(9, 21, 16, 9), -- Liam Lawson, 9th place
(10, 21, 7, 10), -- Lewis Hamilton, 10th place
(11, 21, 4, 11), -- Sergio Perez, 11th place
(12, 21, 13, 12),-- Oliver Bearman, 12th place
(13, 21, 23, 13),-- Valtteri Bottas, 13th place
(14, 21, 9, 14), -- Fernando Alonso, 14th place
(15, 21, 21, 15);-- Zhou Guanyu, 15th place

