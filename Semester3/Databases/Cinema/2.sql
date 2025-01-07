create procedure AddActorToTeam (@ActorID int, @ProductionID int, @EntryMoment int) as
begin
    insert into ActingTeam values (@ActorID, @ProductionID, @EntryMoment)
end
