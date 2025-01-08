create view GetWomenThatBoughtShoeModel1 as
    select W.wid, S.smid from Women W 
    join Orders O on W.wid = O.wid
    join Shoes S on O.sid = S.sid
    group by W.wid, S.smid
    having sum(O.oQuantity) >= 2 and S.smid = 1;

select * from GetWomenThatBoughtShoeModel1;

-- given shoe model is 1 since we cannot have parameters in a view