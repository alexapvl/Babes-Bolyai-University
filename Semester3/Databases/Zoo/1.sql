create database Zoo;

create table Zoo (
    zid int primary key identity(1,1),
    admin int,
    zname varchar(50)
);

create table Animals (
    aid int primary key identity(1,1),
    aname varchar(50),
    adob date,
    zid int foreign key references Zoo(zid)
);

create table Food (
    fid int primary key,
    fname varchar(50)
);

create table FoodDailyQuota (
    aid int foreign key references Animals(aid),
    fid int foreign key references Food(fid),
    quantity int,
    fdqDate date
);

create table Visitors (
    vid int primary key identity(1,1),
    vname varchar(50),
    age int
);

create table Visits (
    vvid int primary key identity(1,1),
    vvdate date,
    price float,
    vid int foreign key references Visitors(vid),
    zid int foreign key references Zoo(zid)
);

INSERT INTO Zoo (admin, zname) VALUES
(1, 'Safari Park'),
(2, 'Rainforest Zoo'),
(3, 'Ocean World'),
(4, 'Mountain Zoo'),
(5, 'Desert Habitat');

INSERT INTO Animals (aname, adob, zid) VALUES
('Lion', '2010-05-12', 1),
('Elephant', '2008-08-30', 2),
('Penguin', '2015-01-26', 3),
('Panda', '2011-11-11', 4),
('Kangaroo', '2012-03-05', 2);

INSERT INTO Food (fid, fname) VALUES
(1, 'Meat'),
(2, 'Grass'),
(3, 'Fish'),
(4, 'Bamboo'),
(5, 'Leaves');

INSERT INTO FoodDailyQuota (aid, fid, quantity, fdqDate) VALUES
(1, 1, 10, '2025-01-01'),
(2, 2, 50, '2025-01-01'),
(3, 3, 20, '2025-01-01'),
(4, 4, 30, '2025-01-01'),
(5, 2, 40, '2025-01-01');

INSERT INTO Visitors (vname, age) VALUES
('John Doe', 25),
('Jane Smith', 32),
('Alice Johnson', 28),
('Robert Brown', 45),
('Mary Davis', 22);

INSERT INTO Visits (vvdate, price, vid, zid) VALUES
('2025-01-05', 20.00, 1, 1),
('2025-01-06', 15.50, 2, 2),
('2025-01-07', 10.00, 3, 3),
('2025-01-05', 18.75, 4, 4),
('2025-01-06', 12.00, 5, 5);
