create table festival_schema.Person
(
    ID      int identity
        primary key
        constraint id_UNIQUE
            unique,
    Name    varchar(45) not null,
    Surname varchar(45) not null,
    Email   varchar(75) not null,
    Birth   date
)
go

INSERT INTO iis.festival_schema.Person (ID, Name, Surname, Email, Birth) VALUES (8, N'Petr', N'JeDebilek', N'prachybába@ftracker.cz', N'6969-01-06');
INSERT INTO iis.festival_schema.Person (ID, Name, Surname, Email, Birth) VALUES (10, N'Test', N'User', N'test@user.com', null);
INSERT INTO iis.festival_schema.Person (ID, Name, Surname, Email, Birth) VALUES (12, N'Pokus', N'Testovni', N'test.moni@IIS.com', N'9991-06-07');
INSERT INTO iis.festival_schema.Person (ID, Name, Surname, Email, Birth) VALUES (13, N'Admin', N'Acconut', N'admin@example.com', N'6969-06-06');