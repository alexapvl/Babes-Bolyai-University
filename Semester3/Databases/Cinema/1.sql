create database Cinema;

use Cinema;

create table Actors (
    aid int primary key identity(1,1),
    aname varchar(50),
    arank int
);

create table Companies (
    cid int primary key identity(1,1),
    cname varchar(50)
);

create table StageDirectors (
    sdid int primary key identity(1,1),
    sdname varchar(50),
    awards int
);

create table Movies (
    mid int primary key identity(1,1),
    mname varchar(50),
    releaseDate date,
    cid int foreign key references Companies(cid),
    sdid int foreign key references StageDirectors(sdid)
);

create table CinemaProductions (
    cpid int primary key identity(1,1),
    cptitle varchar(50),
    mid int foreign key references Movies(mid)
);

create table ActingTeam (
    cpid int foreign key references CinemaProductions(cpid),
    aid int foreign key references Actors(aid),
    entryMoment int
);

INSERT INTO Actors (aname, arank) VALUES
('Robert Downey Jr.', 5),
('Scarlett Johansson', 4),
('Chris Hemsworth', 4),
('Meryl Streep', 5),
('Leonardo DiCaprio', 5);

INSERT INTO Companies (cname) VALUES
('Warner Bros.'),
('20th Century Fox'),
('Marvel Studios'),
('Universal Pictures'),
('Paramount Pictures');

INSERT INTO StageDirectors (sdname, awards) VALUES
('Steven Spielberg', 3),
('Christopher Nolan', 2),
('Quentin Tarantino', 2),
('Martin Scorsese', 4),
('James Cameron', 5);

INSERT INTO Movies (mname, releaseDate, cid, sdid) VALUES
('Inception', '2010-07-16', 2, 2),
('Avengers: Endgame', '2019-04-26', 3, 5),
('The Godfather', '1972-03-24', 1, 4),
('Titanic', '1997-12-19', 5, 5),
('Jurassic Park', '1993-06-11', 1, 1);

INSERT INTO CinemaProductions (cptitle, mid) VALUES
('Inception Release', 1),
('Endgame Release', 2),
('The Godfather Release', 3),
('Titanic Release', 4),
('Jurassic Park Release', 5);

INSERT INTO ActingTeam (cpid, aid, entryMoment) VALUES
(1, 3, 10),
(2, 1, 15),
(2, 2, 30),
(3, 5, 10),
(4, 4, 25),
(5, 3, 40);

-- insert to have result at point 3
insert into ActingTeam (cpid, aid, entryMoment) values 
(2, 3, 1),
(3, 3, 1),
(4, 3, 1);
