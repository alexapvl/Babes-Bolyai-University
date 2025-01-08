create database Tax;

create table TaxCompany (
    tcid int primary key identity(1,1),
    tcname varchar(50),
    tcNoClients int,
    tcNoSlr int
);

insert into TaxCompany values 
('Broker', 1, 2)

create table Clients (
    cid int primary key identity(1,1),
    tcid int foreign key references TaxCompany(tcid),
    moneyClient float
);

alter table Clients add idNumber int
update Clients set idNumber = 123 where cid = 1

insert into Clients values 
(1, 100)

create table SLRs (
    sid int primary key identity(1,1),
    cid int foreign key references Clients(cid),
    sactivity varchar(50),
    slocation varchar(50)
);

insert into SLRs values 
(1, 'carspotting', 'Cluj-Napoca')

update SLRs set sname = 'AudiPro' where sid = 1;

insert into SLRs values 
(1, 'honey', 'Turda')


update SLRs set sname = 'HoneyMoney' where sid = 2;

alter table SLRs add sname varchar(50)

create table Assets (
    aid int primary key identity(1,1),
    aname varchar(50),
    cid int foreign key references Clients(cid),
    quantity int,
    sid int foreign key references SLRs(sid)
);

insert into Assets values 
('AMG GT', 1, 1, 1)