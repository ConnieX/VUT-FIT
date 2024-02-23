CREATE TABLE festival_schema.Person (
  [ID] INT NOT NULL IDENTITY,
  [Name] VARCHAR(45) NOT NULL,
  [Surname] VARCHAR(45) NOT NULL,
  [Email] VARCHAR(75) NOT NULL,
  [Birth] DATE NULL,
  PRIMARY KEY ([ID]),
  CONSTRAINT [id_UNIQUE] UNIQUE  ([ID] ASC));


-- -----------------------------------------------------
-- Table User
-- -----------------------------------------------------
CREATE TABLE festival_schema.[User] (
  [PersonID] INT NOT NULL,
  [Password] VARCHAR(256) NULL,
  [Role] INT NULL default 0,
  PRIMARY KEY ([PersonID]),
  CONSTRAINT [fk_User_Person]
    FOREIGN KEY ([PersonID])
    REFERENCES festival_schema.Person ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table Festival
-- -----------------------------------------------------
CREATE TABLE festival_schema.Festival (
  [ID] INT NOT NULL IDENTITY,
  [StartDate] DATE NOT NULL,
  [Name] VARCHAR(45) NOT NULL,
  [EndDate] DATE NOT NULL,
  [Place] VARCHAR(100) NOT NULL,
  [Genre] VARCHAR(45) NOT NULL,
  [Capacity] VARCHAR(45) NOT NULL,
  [Details] VARCHAR(max) NULL,
  PRIMARY KEY ([ID]),
  CONSTRAINT [ID_UNIQUE_FESTIVAL] UNIQUE  ([ID] ASC));


-- -----------------------------------------------------
-- Table Artist
-- -----------------------------------------------------
CREATE TABLE festival_schema.Artist (
  [ID] INT NOT NULL IDENTITY,
  [Name] VARCHAR(45) NOT NULL,
  [Surname] VARCHAR(45) NOT NULL,
  PRIMARY KEY ([ID]),
  CONSTRAINT [ID_UNIQUE_ARTIST] UNIQUE  ([ID] ASC));


-- -----------------------------------------------------
-- Table Performer
-- -----------------------------------------------------
CREATE TABLE festival_schema.Performer (
  [ID] INT NOT NULL IDENTITY,
  [Name] VARCHAR(45) NOT NULL,
  [Logo] VARCHAR(100) NULL,
  [Genre] VARCHAR(45) NULL,
  [Rating] FLOAT NULL,
  [Artist_ID] INT NOT NULL,
  PRIMARY KEY ([ID]),
  CONSTRAINT [ID_UNIQUE_Performer] UNIQUE  ([ID] ASC) ,
  INDEX fk_Performer_Artist1_idx ([Artist_ID] ASC) ,
  CONSTRAINT [fk_Performer_Artist1]
    FOREIGN KEY ([Artist_ID])
    REFERENCES festival_schema.Artist ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table Performer_has_Artist
-- -----------------------------------------------------
CREATE TABLE festival_schema.Performer_has_Artist (
  [Performer_ID] INT NOT NULL,
  [Artist_ID] INT NOT NULL,
  PRIMARY KEY ([Performer_ID], [Artist_ID]),
  INDEX [fk_Performer_has_Artist_Artist1_idx] ([Artist_ID] ASC),
  INDEX fk_Performer_has_Artist_Performer1_idx ([Performer_ID] ASC) ,
  CONSTRAINT [fk_Performer_has_Artist_Performer1]
    FOREIGN KEY ([Performer_ID])
    REFERENCES festival_schema.Performer ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT [fk_Performer_has_Artist_Artist1]
    FOREIGN KEY ([Artist_ID])
    REFERENCES festival_schema.Artist ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table Concert
-- -----------------------------------------------------
CREATE TABLE festival_schema.Concert (
  [FestivalID] INT NOT NULL,
  [PerformerID] INT NOT NULL,
  [Start] DATETIME2(0) NOT NULL,
  [End] DATETIME2(0) NOT NULL,
  [Number] INT NULL,
  PRIMARY KEY ([FestivalID], [PerformerID]),
  INDEX fk_Festival_has_Performer_Performer1_idx ([PerformerID] ASC),
  INDEX fk_Festival_has_Performer_Festival1_idx ([FestivalID] ASC) ,
  CONSTRAINT [fk_Festival_has_Performer_Festival1]
    FOREIGN KEY ([FestivalID])
    REFERENCES festival_schema.Festival ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT [fk_Festival_has_Performer_Performer1]
    FOREIGN KEY ([PerformerID])
    REFERENCES festival_schema.Performer ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table TicketType
-- -----------------------------------------------------
CREATE TABLE festival_schema.TicketType (
  [ID] INT NOT NULL,
  [FestivalID] INT NOT NULL,
  [Name] VARCHAR(45) NOT NULL,
  [ValidFrom] DATE NOT NULL,
  [ValidTo] DATE NOT NULL,
  [Count] INT NOT NULL,
  [Price] FLOAT NOT NULL,
  PRIMARY KEY ([ID], [FestivalID]),
  INDEX fk_TicketType_Festival1_idx ([FestivalID] ASC),
  INDEX ID_UNIQUE_TICKET_TYPE ([ID] ASC) ,
  CONSTRAINT [fk_TicketType_Festival1]
    FOREIGN KEY ([FestivalID])
    REFERENCES festival_schema.Festival ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table Reservation
-- -----------------------------------------------------
CREATE TABLE festival_schema.Reservation (
  [State] VARCHAR(45) NOT NULL,
  [ExpirationDate] DATE NOT NULL,
  [ID] INT NOT NULL IDENTITY,
  [PersonID] INT NOT NULL,
  PRIMARY KEY ([ID], [PersonID]),
  CONSTRAINT [ID_UNIQUE_RESERVATION] UNIQUE  ([ID] ASC) ,
  INDEX fk_Reservation_Person1_idx ([PersonID] ASC) ,
  CONSTRAINT [fk_Reservation_Person1]
    FOREIGN KEY ([PersonID])
    REFERENCES festival_schema.Person ([ID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table Ticket
-- -----------------------------------------------------
CREATE TABLE festival_schema.Ticket (
  [ID] INT NOT NULL IDENTITY,
  [TicketTypeID] INT NOT NULL,
  [FestivalID] INT NOT NULL,
  [ReservationID] INT NOT NULL,
  [PersonID] INT NOT NULL,
  PRIMARY KEY ([ID], [TicketTypeID], [FestivalID], [ReservationID], [PersonID]),
  CONSTRAINT [ID_UNIQUE_TICKET] UNIQUE  ([ID] ASC) ,
  INDEX fk_Ticket_TicketType1_idx ([TicketTypeID] ASC, [FestivalID] ASC) ,
  INDEX fk_Ticket_Reservation1_idx ([ReservationID] ASC, [PersonID] ASC) ,
  CONSTRAINT [fk_Ticket_TicketType1]
    FOREIGN KEY ([TicketTypeID] , [FestivalID])
    REFERENCES festival_schema.TicketType ([ID] , [FestivalID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT [fk_Ticket_Reservation1]
    FOREIGN KEY ([ReservationID] , [PersonID])
    REFERENCES festival_schema.Reservation ([ID] , [PersonID])
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);
