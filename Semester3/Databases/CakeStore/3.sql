create function ChefsSpecialisedInEveryCakeType ()
returns table 
as
    return(
        select CC.ccname from ConfectioneryChefs CC
        join Specialisation S on CC.ccid = S.ccid
        group by CC.ccname
        having count(*) 
        = (select count(*) from CakeTypes)
    )