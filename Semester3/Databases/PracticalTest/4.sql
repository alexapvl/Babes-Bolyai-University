create function Ex4 ()
returns table
as 
    return (
    select C.idNumber, S.slocation, count(A.aid) as 'No. assets SRL' from Clients C
    join SLRs S on C.cid = S.cid
    full join Assets A on C.cid = A.cid and A.sid = S.sid -- to show also with 0 assets
    group by C.idNumber, S.slocation
    )

select * from Ex4()
