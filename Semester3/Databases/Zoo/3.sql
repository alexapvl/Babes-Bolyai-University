select V.zid, count(V.vvid) as visit_count from Visits V
group by V.zid
having count(V.vvid) 
= (select min(visit_count)
    from (  
        select zid, count(vvid) as visit_count 
        from Visits
        group by zid
        ) as AliasTable
);