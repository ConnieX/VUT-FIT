create table festival_schema.Performer_has_Artist
(
    Performer_ID int not null
        constraint fk_Performer_has_Artist_Performer1
            references festival_schema.Performer,
    Artist_ID    int not null
        constraint fk_Performer_has_Artist_Artist1
            references festival_schema.Artist,
    primary key (Performer_ID, Artist_ID)
)
go

create index fk_Performer_has_Artist_Performer1_idx
    on festival_schema.Performer_has_Artist (Performer_ID)
go

create index fk_Performer_has_Artist_Artist1_idx
    on festival_schema.Performer_has_Artist (Artist_ID)
go

INSERT INTO iis.festival_schema.Performer_has_Artist (Performer_ID, Artist_ID) VALUES (2, 2);
INSERT INTO iis.festival_schema.Performer_has_Artist (Performer_ID, Artist_ID) VALUES (2, 3);