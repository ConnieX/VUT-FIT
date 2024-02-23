create table festival_schema.Ticket
(
	ID int identity
		constraint ID_UNIQUE_TICKET
			unique,
	TicketTypeID int not null,
	FestivalID int not null,
	ReservationID int not null,
	PersonID int not null,
	primary key (ID, TicketTypeID, FestivalID, ReservationID, PersonID),
	constraint fk_Ticket_Reservation1
		foreign key (ReservationID, PersonID) references festival_schema.Reservation,
	constraint fk_Ticket_TicketType1
		foreign key (TicketTypeID, FestivalID) references festival_schema.TicketType
)
go

create index fk_Ticket_Reservation1_idx
	on festival_schema.Ticket (ReservationID, PersonID)
go

create index fk_Ticket_TicketType1_idx
	on festival_schema.Ticket (TicketTypeID, FestivalID)
go

