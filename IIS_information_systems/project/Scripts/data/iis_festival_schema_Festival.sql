create table festival_schema.Festival
(
    ID        int identity
        primary key
        constraint ID_UNIQUE_FESTIVAL
            unique,
    StartDate date         not null,
    Name      varchar(45)  not null,
    EndDate   date         not null,
    Place     varchar(100) not null,
    Genre     varchar(45)  not null,
    Capacity  int          not null,
    Details   varchar(max),
    CreatorID int          not null
        constraint Festival_user__fk
            references festival_schema.[User]
)
go

INSERT INTO iis.festival_schema.Festival (ID, StartDate, Name, EndDate, Place, Genre, Capacity, Details, CreatorID) VALUES (1, N'2020-11-19', N'B4L', N'2020-11-19', N'Ostrava', N'Multi', 6, null, 10);
INSERT INTO iis.festival_schema.Festival (ID, StartDate, Name, EndDate, Place, Genre, Capacity, Details, CreatorID) VALUES (2, N'2020-11-29', N'Colors', N'2020-11-30', N'Ostrava', N'Multi', 16, null, 10);
INSERT INTO iis.festival_schema.Festival (ID, StartDate, Name, EndDate, Place, Genre, Capacity, Details, CreatorID) VALUES (47, N'2020-11-22', N'Porušení narízení ', N'2020-11-24', N'Brno', N'Rebel', 699, N'Jdeme rozširovat koronu', 10);