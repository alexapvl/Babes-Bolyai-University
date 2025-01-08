create database Trains;

create table TrainTypes (
    ttid int primary key identity(1,1),
    ttdesc varchar(100)
);

insert into TrainTypes values 
('tren lung')

create table Trains (
    tid int primary key identity(1,1),
    tname varchar(50),
    ttid int foreign key references TrainTypes(ttid)
);

insert into Trains values 
('thomas', 1)

create table Stations (
    sid int primary key identity(1,1),
    sname varchar(50) unique
);

insert into Stations values 
('fsega')

create table Routes (
    rid int primary key identity(1,1),
    rname varchar(50) unique,
    tid int foreign key references Trains(tid)
);

insert into Routes values 
('fsega-floresti', 1)

create table Schedule (
    rid int foreign key references Routes(rid),
    sid int foreign key references Stations(sid),
    primary key (rid, sid),
    atime time,
    dtime time
);