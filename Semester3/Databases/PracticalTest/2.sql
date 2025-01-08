create procedure Ex2 (@ClientID int)
as
    select count(distinct A.aid) as 'No. Assets', count(S.sid) as 'No. SLRs' from Clients C
    join Assets A on C.cid = A.cid
    join SLRs S on C.cid = S.cid
    group by C.cid
    having C.cid = @ClientID

drop procedure Ex2

exec Ex2 1
