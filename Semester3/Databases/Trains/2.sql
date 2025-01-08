create procedure AddTimeToSchedule (@RouteID int, @StationID int, @Arrival time, @Departure time) 
as
begin 
    if exists (select 1 from Schedule where rid = @RouteID and sid = @StationID)
    begin 
        update Schedule set atime = @Arrival, dtime = @Departure where rid = @RouteID and sid = @StationID
    end 
    else 
    begin 
        insert into Schedule values 
        (@RouteID, @StationID, @Arrival, @Departure)
    end
end 