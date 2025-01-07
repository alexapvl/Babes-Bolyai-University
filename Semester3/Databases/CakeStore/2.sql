create procedure AddCakeToOrder (@OrderID int, @CakeName varchar(50), @P int) as
declare 
    @CakeID int
begin
    select @CakeID = cid from Cakes where cname = @CakeName

    if exists (select 1 from Orders where oid = @OrderID and cid = @CakeID)
    begin
        update Orders set quantity = @P where oid = @OrderID and cid = @CakeID
    end

    else 
    begin
        insert into Orders (oid, cid, quantity, orderDate) values 
        (@OrderID, @CakeID, @P, getdate())
    end
end

drop procedure AddCakeToOrder;

exec AddCakeToOrder 106, 'Triple Chocolate Cake', 5;

select * from Orders;