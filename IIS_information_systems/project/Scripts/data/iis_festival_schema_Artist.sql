create table festival_schema.Artist
(
    ID      int identity
        primary key
        constraint ID_UNIQUE_ARTIST
            unique,
    Name    varchar(45) not null,
    Surname varchar(45) not null
)
go

INSERT INTO iis.festival_schema.Artist (ID, Name, Surname) VALUES (2, N'Vaclav', N'Jebavy');
INSERT INTO iis.festival_schema.Artist (ID, Name, Surname) VALUES (3, N'Karel', N'Svach');
INSERT INTO iis.festival_schema.Artist (ID, Name, Surname) VALUES (4, N'Fred', N'Larcher');