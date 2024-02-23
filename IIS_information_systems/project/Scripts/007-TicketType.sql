create table festival_schema.TicketType
(
	ID int not null,
	FestivalID int not null
		constraint fk_TicketType_Festival1
			references festival_schema.Festival,
	Name varchar(45) not null,
	ValidFrom date not null,
	ValidTo date not null,
	Count int not null,
	Price float not null,
	primary key (ID, FestivalID)
)
go

create index ID_UNIQUE_TICKET_TYPE
	on festival_schema.TicketType (ID)
go

create index fk_TicketType_Festival1_idx
	on festival_schema.TicketType (FestivalID)
go

