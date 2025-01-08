create table ShoeModels (
    smid int primary key identity(1,1),
    smname varchar(50),
    smseason varchar(20)
);

insert into ShoeModels values 
('Nike Air Force 1', 'Summer')

create table Shoes (
    sid int primary key identity(1,1),
    sprice float,
    smid int foreign key references ShoeModels(smid)
);

insert into Shoes values 
(100, 1)

insert into Shoes values 
(1000, 1)

create table PresentationShops (
    psid int primary key identity(1,1),
    psname varchar(50),
    city varchar(20)
);

insert into PresentationShops values 
('Nike Shop', 'Cluj-Napoca')

insert into PresentationShops values 
('Zara', 'Cluj-Napoca'),
('Buzz', 'Cluj-Napoca')

create table AvailableStock (
    psid int foreign key references PresentationShops(psid),
    sid int foreign key references Shoes(sid),
    primary key (psid, sid),
    asQuantity int
);

insert into AvailableStock values 
(2, 1, 1),
(3, 1, 1),
(1, 2, 1),
(3, 2, 1)

create table Women (
    wid int primary key identity(1,1),
    wname varchar(50),
    buget float
);

insert into Women values 
('Alexandra', 3000),
('Iulia', 5000)

create table Orders (
    wid int foreign key references Women(wid),
    sid int foreign key references Shoes(sid),
    primary key (wid, sid),
    oQuantity int,
    oPrice float
);

insert into Orders values 
(1, 1, 1, 100),
(2, 1, 3, 300)
