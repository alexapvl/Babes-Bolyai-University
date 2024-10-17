create database f1_season_2024;

-- single line comment 
/*
This is 
a multi-line
comment
*/

use f1_season_2024; 

create table constructor(
	constructorId int primary key,
    constructorName varchar(255) not null,
    constructorCountry varchar(5) not null);	
    
create table team(
	teamId int primary key,
    constructorId int not null,
    teamName varchar(255) not null,
    teamCountry varchar(5) not null,
    constraint FK_Team foreign key(constructorId) references constructor(constructorId)
		on delete cascade -- upon deletion of the reference, all rows with this reference will be deleted(will likely not be used)
);

create table sponsor(
	sponsorId int primary key,
    sponsorName varchar(255) not null,
    sponsorCountry varchar(5) not null
);

create table sponsorship(
	sponsorshipId int primary key,
    teamId int not null,
    sponsorId int not null,
    constraint FK1_Sponsorship foreign key(teamId) references team(teamId),
    constraint FK2_Sponsorship foreign key(sponsorId) references sponsor(sponsorId),
    unique(teamId, sponsorId) -- makes it so that there is no 'double' sponsorship for one team from the same sponsor
);

create table driver(
	driverId int primary key,
    driverName varchar(255) not null,
    teamId int not null,
    nationality varchar(5) not null,
    dateOfBirth date not null,
    constraint FK_Driver foreign key(teamId) references team(teamId)
);

create table driverChampionship(
	dchampionshipId int primary key,
    driverId int not null,
    points float not null default 0.0, -- sets the default points of a driver to 0
    constraint FK_DriverChampionship foreign key(driverId) references driver(driverId)
);

create table circuit(
	circuitId int primary key,
    circuitName varchar(255) not null,
    circuitCountry varchar(5) not null,
    lapLength decimal(2,1) not null default 0, -- stores lengths up to 9.9 (km), default value is 0
    constraint LapRange check (lapLength >= 0) -- the distance should be a positive number
);

create table race(
	raceId int primary key,
    circuitId int not null,
    raceDate date not null,
    numberOfLaps int not null default 0,
    constraint FK_Race foreign key(circuitId) references circuit(circuitId),
    constraint DateRange check (YEAR(raceDate) = 2024) -- date should be this year(2024) since the database is for the 2024 season
);

create table result(
	resultId int primary key,
    raceId int not null,
    driverId int not null,
    placement int not null,
    unique (raceId, placement),
    constraint PlacementRange check (placement >= 1 and placement <= 20), -- there are 20 drivers in a race, so placements range from 1..20
    constraint FK1_Result foreign key(raceId) references race(raceId),
    constraint FK2_Result foreign key(driverId) references driver(driverId),
    unique (raceId, driverId) -- for a specific race, the driver should have only one result
);

create table dhlFastestLap(
	dhlId int primary key,
    raceId int not null,
    driverId int not null,
    fastestLapTime varchar(15) not null default '0:00.000' -- example '1:20.123'
);