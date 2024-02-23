create table festival_schema.Festival
(
	ID int identity
		primary key
		constraint ID_UNIQUE_FESTIVAL
			unique,
	StartDate date not null,
	Name varchar(45) not null,
	EndDate date not null,
	Place varchar(100) not null,
	Genre varchar(45) not null,
	Capacity int not null,
	Details varchar(max),
	CreatorID int not null
		constraint Festival_user__fk
			references festival_schema.[User]
)
go

