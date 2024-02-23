create table festival_schema.Artist
(
	ID int identity
		primary key
		constraint ID_UNIQUE_ARTIST
			unique,
	Name varchar(45) not null,
	Surname varchar(45) not null
)
go

