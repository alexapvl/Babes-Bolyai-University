create view PopularActors 
as
    select A.aname from Actors A
    join ActingTeam ACT on A.aid = ACT.aid
    join CinemaProductions CP on CP.cpid = ACT.cpid
    group by A.aname
    having (select count(A.aname)) = (select count(*) from CinemaProductions)

select * from PopularActors;