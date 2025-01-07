select M.mname from Movies M where datediff(day, releaseDate, '2018-01-01') < 0

select count(distinct CP.cpid), CP.mid from CinemaProductions CP
group by CP.mid
having count(distinct CP.cpid) > 0

create function AtLeastPProductions (@p int)
returns table
as 
    return (
        select M.mname from Movies M 
        join CinemaProductions CP 
        on M.mid = CP.mid
        where datediff(day, M.releaseDate, '2018-01-01') < 0
        group by CP.mid, M.mname
        having count(distinct CP.cpid) >= @p
    )

select * from AtLeastPProductions(1)
