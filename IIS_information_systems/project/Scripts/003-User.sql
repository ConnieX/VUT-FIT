create table festival_schema.[User]
(
	PersonID int not null
		primary key
		constraint fk_User_Person
			references festival_schema.Person,
	Password varchar(256),
	Role int default 0 not null,
	ProfilePic varchar(100),
	PhoneNumber int,
	Address varchar(150)
)
go

