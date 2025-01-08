create view Ex3
as 
    select C.idNumber, C.moneyClient, S.sname, S.sactivity from Clients C
    join SLRs S on C.cid = S.cid;


select * from Ex3;
