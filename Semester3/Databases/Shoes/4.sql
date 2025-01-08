create function Ex4 (@T int) 
returns table 
as
    return (
        select S.sid from Shoes S
        join AvailableStock AVS on S.sid = AVS.sid
        group by S.sid
        having count(distinct AVS.psid) >= @T
    )

drop function Ex4;

select * from Ex4(3)