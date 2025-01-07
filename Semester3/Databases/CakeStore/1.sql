create database CakeStore;

use CakeStore;

create table CakeTypes (
    ctid int primary key identity(1,1),
    ctname varchar(50),
    ctdesc varchar(100)
);

create table Cakes (
    cid int primary key identity(1,1),
    cname varchar(50),
    cshape varchar(50),
    cweight float,
    cprice float,
    ctid int foreign key references CakeTypes(ctid)
);

create table ConfectioneryChefs (
    ccid int primary key identity(1,1),
    ccname varchar(50),
    ccgender char, -- M or F
    ccdob date
);

create table Specialisation (
    ccid int foreign key references ConfectioneryChefs(ccid),
    cid int foreign key references Cakes(cid),
    primary key (ccid, cid)
);

create table Orders (
    oid int,
    cid int foreign key references Cakes(cid),
    primary key (oid, cid),
    quantity int,
    orderDate date
);

INSERT INTO CakeTypes (ctname, ctdesc) VALUES
('Chocolate Cake', 'Rich and moist chocolate flavor'),
('Vanilla Cake', 'Classic vanilla sponge cake'),
('Red Velvet Cake', 'Velvety soft with a hint of cocoa'),
('Cheesecake', 'Creamy and smooth cream cheese delight'),
('Carrot Cake', 'Spiced cake with grated carrots');

INSERT INTO Cakes (cname, cshape, cweight, cprice, ctid) VALUES
('Triple Chocolate Cake', 'Round', 1.5, 45.00, 1),
('Classic Vanilla', 'Square', 1.2, 30.00, 2),
('Red Velvet Dream', 'Heart', 1.4, 50.00, 3),
('New York Cheesecake', 'Round', 1.0, 40.00, 4),
('Spicy Carrot Delight', 'Rectangle', 1.6, 55.00, 5);

INSERT INTO ConfectioneryChefs (ccname, ccgender, ccdob) VALUES
('Julia Child', 'F', '1912-08-15'),
('Gordon Ramsay', 'M', '1966-11-08'),
('Martha Stewart', 'F', '1941-08-03'),
('Nigella Lawson', 'F', '1960-01-06'),
('Paul Hollywood', 'M', '1966-03-01');

INSERT INTO Specialisation (ccid, cid) VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5),
(1, 5),
(3, 4);

INSERT INTO Orders (oid, cid, quantity, orderDate) VALUES
(101, 1, 2, '2025-01-01'),
(102, 3, 1, '2025-01-02'),
(103, 2, 5, '2025-01-05'),
(104, 4, 3, '2025-01-06'),
(105, 5, 1, '2025-01-07');

insert into Specialisation (ccid, cid) values 
(2,1),
(2,3),
(2,4),
(2,5)
