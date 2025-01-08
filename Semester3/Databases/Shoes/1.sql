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

create table PresentationShops (
    psid int primary key identity(1,1),
    psname varchar(50),
    city varchar(20)
);

insert into PresentationShops values 
('Nike Shop', 'Cluj-Napoca')

create table AvailableStock (
    psid int foreign key references PresentationShops(psid),
    sid int foreign key references Shoes(sid),
    primary key (psid, sid),
    asQuantity int
);

create table Women (
    wid int primary key identity(1,1),
    wname varchar(50),
    buget float
);

create table Orders (
    wid int foreign key references Women(wid),
    sid int foreign key references Shoes(sid),
    primary key (wid, sid),
    oQuantity int,
    oPrice float
);

