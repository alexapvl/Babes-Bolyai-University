create database f1;

use f1;

create table constructor(
	constructorId int primary key,
    constructorName varchar(255) not null,
    constructorCountry varchar(5) not null);

create table team(
	teamId int primary key identity(1,1),
    constructorId int not null,
    teamName varchar(255) not null,
    teamCountry varchar(5) not null,
    constraint FK1_team_constructorId foreign key(constructorId) references constructor(constructorId)
		on delete cascade -- upon deletion of the reference, all rows with this reference will be deleted(will likely not be used)
);

create table sponsor(
	sponsorId int primary key identity(1,1),
    sponsorName varchar(255) not null,
    sponsorCountry varchar(5) not null
);

create table sponsorship(
	sponsorshipId int primary key identity(1,1),
    teamId int not null,
    sponsorId int not null,
    constraint FK1_sponsorship_teamId foreign key(teamId) references team(teamId) on delete cascade,
    constraint FK2_sponsorship_sponsorId foreign key(sponsorId) references sponsor(sponsorId) on delete cascade,
    unique(teamId, sponsorId) -- makes it so that there is no 'double' sponsorship for one team from the same sponsor
);

create table driver(
	driverId int primary key identity(1,1),
    driverName varchar(255) not null,
    teamId int not null,
    nationality varchar(5) not null,
    dateOfBirth date not null,
    constraint FK1_driver_teamId foreign key(teamId) references team(teamId) on delete cascade
);

create table driverChampionship(
	dchampionshipId int primary key identity(1,1),
    driverId int not null,
    points float not null default 0.0, -- sets the default points of a driver to 0
    constraint FK1_driverChampionship_driverId foreign key(driverId) references driver(driverId) on delete cascade
);

create table circuit(
	circuitId int primary key identity(1,1),
    circuitName varchar(255) not null,
    circuitCountry varchar(5) not null,
    lapLength decimal(2,1) not null default 0, -- stores lengths up to 9.9 (km), default value is 0
    constraint LapRange check (lapLength >= 0) -- the distance should be a positive number
);

create table race(
	raceId int primary key identity(1,1),
    circuitId int not null,
    raceDate date not null,
    numberOfLaps int not null default 0,
    constraint FK1_race_circuitId foreign key(circuitId) references circuit(circuitId) on delete cascade,
    constraint DateRange check (YEAR(raceDate) = 2025) -- date should be for the 2025 season
);

create table result(
	resultId int primary key identity(1,1),
    raceId int not null,
    driverId int not null,
    placement int not null,
    unique (raceId, placement),
    constraint PlacementRange check (placement >= 1 and placement <= 20), -- there are 20 drivers in a race, so placements range from 1..20
    constraint FK1_result_raceId foreign key(raceId) references race(raceId) on delete cascade,
    constraint FK2_result_driverId foreign key(driverId) references driver(driverId) on delete cascade,
    unique (raceId, driverId) -- for a specific race, the driver should have only one result
);

create table dhlFastestLap(
	dhlId int primary key identity(1,1),
    raceId int not null,
    driverId int not null,
    fastestLapTime varchar(15) not null default '0:00.000', -- example '1:20.123'
    constraint FK1_dhlFastestLap_raceId foreign key(raceId) references race(raceId) on delete cascade,
    constraint FK2_dhlFastestLap_driverId foreign key(driverId) references driver(driverId) on delete cascade
);