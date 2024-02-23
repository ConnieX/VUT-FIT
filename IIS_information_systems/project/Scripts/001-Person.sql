create table festival_schema.Person
(
	ID int identity
		primary key
		constraint id_UNIQUE
			unique,
	Name varchar(45) not null,
	Surname varchar(45) not null,
	Email varchar(75) not null,
	Birth date
)
go

