create function GetVisitorsByAniamalCount (@N int) 
returns table
as
    return (
        select distinct V.vid from Visits V
        where V.zid in (
        select A.zid from Animals A
        group by A.zid
        having count(A.aid) >= @N
        )
    )

select * from GetVisitorsByAniamalCount(1)