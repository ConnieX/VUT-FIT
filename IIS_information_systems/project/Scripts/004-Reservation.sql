create table festival_schema.Reservation
(
	State varchar(45) not null,
	ExpirationDate date not null,
	ID int identity
		constraint ID_UNIQUE_RESERVATION
			unique,
	PersonID int not null
		constraint fk_Reservation_Person1
			references festival_schema.Person,
	primary key (ID, PersonID)
)
go

create index fk_Reservation_Person1_idx
	on festival_schema.Reservation (PersonID)
go

