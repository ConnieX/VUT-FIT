create table festival_schema.Concert
(
    FestivalID  int          not null
        constraint fk_Festival_has_Performer_Festival1
            references festival_schema.Festival,
    PerformerID int          not null
        constraint fk_Festival_has_Performer_Performer1
            references festival_schema.Performer,
    Start       datetime2(0) not null,
    [End]       datetime2(0) not null,
    Number      int,
    primary key (FestivalID, PerformerID)
)
go

create index fk_Festival_has_Performer_Festival1_idx
    on festival_schema.Concert (FestivalID)
go

create index fk_Festival_has_Performer_Performer1_idx
    on festival_schema.Concert (PerformerID)
go

