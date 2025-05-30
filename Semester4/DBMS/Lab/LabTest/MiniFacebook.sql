create database MiniFacebook;

use MiniFacebook;

create table Categories (
	Cid int primary key identity(1,1),
	DescriptionText varchar(255) not null,
);

create table Pages (
	Pid int primary key identity(1,1),
	PageName varchar(255),
	Cid int foreign key references Categories(Cid) not null,
);

create table Users (
	UserId int primary key identity(1,1),
	UserName varchar(50) not null,
	CurrentCity varchar(100) not null,
	DateOfBirth Date not null
);

create table Likes (
	Lid int primary key identity(1,1),
	UserId int foreign key references Users(UserId) not null,
	Pid int foreign key references Pages(Pid) not null,
	LikeDate DateTime not null,
);

create table Posts (
	PostId int primary key identity(1,1),
	UserId int foreign key references Users(UserId),
	PostDate DateTime not null,
	PostText varchar(255),
	NoShares int not null default 0,
);

create table Comments (
	CommId int primary key identity(1,1),
	PostId int foreign key references Posts(PostId),
	CommentText varchar(255) not null,
	CommentDate DateTime not null,
	TopComment bit not null default 0.
);

insert into Users(UserName, CurrentCity, DateOfBirth) values 
('Alex', 'Cluj-Napoca', cast(N'2004-10-07' as date)),
('Robert', 'Bistrita', cast(N'2004-06-14' as date)),
('Marius', 'Targu Jiu', cast(N'2004-02-26' as date));


insert into Posts(UserId, PostDate, PostText, NoShares) values
(1, GetDate(), 'aaaa',5),
(2, GetDate(), 'bbbbb',15),
(1, GetDate() - MONTH(1), 'cccccc',25);

select * from Posts;

-- WAITFOR DELAY '00:00:05' (5 SECONDS)

-- first transaction
set transaction isolation level READ COMMITTED; -- permits non-repeatable reads
begin transaction
	select * from Posts where PostId = 1;

	waitfor delay '00:00:05' -- wait for 5 seconds (time in which another transaction will occur)

	select * from Posts where PostId = 1;
commit transaction;

-- second transaction
set transaction isolation level READ COMMITTED;
begin transaction
	update Posts set NoShares = 10 where PostId = 1;
commit transaction

