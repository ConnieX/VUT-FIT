create table festival_schema.Performer
(
	ID int identity
		primary key
		constraint ID_UNIQUE_Performer
			unique,
	Name varchar(45) not null,
	Logo varchar(100),
	Genre varchar(45),
	Rating float,
	Artist_ID int not null
		constraint fk_Performer_Artist1
			references festival_schema.Artist
)
go

create index fk_Performer_Artist1_idx
	on festival_schema.Performer (Artist_ID)
go

