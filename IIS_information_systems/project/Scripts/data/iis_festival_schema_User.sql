create table festival_schema.[User]
(
    PersonID    int           not null
        primary key
        constraint fk_User_Person
            references festival_schema.Person,
    Password    varchar(256),
    Role        int default 0 not null,
    ProfilePic  varchar(100),
    PhoneNumber int,
    Address     varchar(150)
)
go

INSERT INTO iis.festival_schema.[User] (PersonID, Password, Role, ProfilePic, PhoneNumber, Address) VALUES (8, N'25f43b1486ad95a1398e3eeb3d83bc4010015fcc9bedb35b432e00298d5021f7', 1, null, null, null);
INSERT INTO iis.festival_schema.[User] (PersonID, Password, Role, ProfilePic, PhoneNumber, Address) VALUES (10, N'1b4f0e9851971998e732078544c96b36c3d01cedf7caa332359d6f1d83567014', 2, null, null, null);
INSERT INTO iis.festival_schema.[User] (PersonID, Password, Role, ProfilePic, PhoneNumber, Address) VALUES (12, N'678e82d907d3e6e71f81d5cf3ddacc3671dc618c38a1b7a9f9393a83d025b296', 3, null, null, null);
INSERT INTO iis.festival_schema.[User] (PersonID, Password, Role, ProfilePic, PhoneNumber, Address) VALUES (13, N'1b4f0e9851971998e732078544c96b36c3d01cedf7caa332359d6f1d83567014', 3, null, null, null);