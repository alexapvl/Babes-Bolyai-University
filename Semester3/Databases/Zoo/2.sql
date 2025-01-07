create procedure DeleteFoodQuotaFromAnimal (@AnimalID int) as 
begin 
    delete from FoodDailyQuota where aid = @AnimalID
end 

exec DeleteFoodQuotaFromAnimal 1;

select * from FoodDailyQuota;