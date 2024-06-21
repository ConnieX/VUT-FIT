INSERT INTO Categories (ID, Name) VALUES (1, 'Potraviny');
INSERT INTO Categories (ID, Name) VALUES (2, 'Elektronika');
INSERT INTO Categories (ID, Name) VALUES (3, 'Nábytek');

INSERT INTO Manufacturers (ID, Name, Description, Logo, Country) VALUES (1, 'Emco', 'Výrobce cereálií', '', 'Czech Republic');
INSERT INTO Manufacturers (ID, Name, Description, Logo, Country) VALUES (2, 'Oneplus', 'Jeden z výrobců chytrých telefonů', '', 'China');
INSERT INTO Manufacturers (ID, Name, Description, Logo, Country) VALUES (3, 'Ikea', 'Nábytek který si musíš složit', '', 'Sweden');

INSERT INTO Goods (ID, Name, Photo, Description, Price, Weight, Count, CategoryID, ManufacturerID) VALUES (1, 'Cereálie', null, 'Čokoládové. Asi i chutné', 6, 15.2, 20, 1,1);
INSERT INTO Goods (ID, Name, Photo, Description, Price, Weight, Count, CategoryID, ManufacturerID) VALUES (2, '7 Pro', null, 'A tak dá sa.', 699, 0.9, 5, 2, 2);
INSERT INTO Goods (ID, Name, Photo, Description, Price, Weight, Count, CategoryID, ManufacturerID) VALUES (3, 'Skříň', null, 'Možná se i nerozpadne', 39, 4, 10, 3, 3);

INSERT INTO Ratings (ID, StarRating, TextRating, GoodsID) VALUES (1, 5.3, 'Eh', 1);
INSERT INTO Ratings (ID, StarRating, TextRating, GoodsID) VALUES (2, 4.6, 'Ale tak', 2);
INSERT INTO Ratings (ID, StarRating, TextRating, GoodsID) VALUES (3, 9.5, 'Good', 2);
INSERT INTO Ratings (ID, StarRating, TextRating, GoodsID) VALUES (4, 5, 'Great', 3);
