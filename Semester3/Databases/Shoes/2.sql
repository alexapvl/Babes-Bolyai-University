create procedure AddShoeToStock (@ShoeID int, @PresShopID int, @Quantity int)
as 
begin 
    insert into AvailableStock values (@PresShopID, @ShoeID, @Quantity)
end 

exec AddShoeToStock 1, 1, 10

select * from AvailableStock;