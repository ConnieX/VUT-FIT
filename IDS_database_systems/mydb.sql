drop table  ALERGENY cascade constraints;
drop table  CELA cascade constraints;
drop table  DOZORCE cascade constraints;
drop table  DRUH cascade constraints;
drop table  JE_DOMLUVEN cascade constraints;
drop table  JE_NA cascade constraints;
drop table  OBJEDNAVKA cascade constraints;
drop table  PASERAK cascade constraints;
drop table  PECE cascade constraints;
drop table  PECIVO cascade constraints;
drop table  PECIVO_JE_Z_SUROVIN cascade constraints;
drop table  PEKAR cascade constraints;
drop table  POLOZKA_OBJEDNAVKY cascade constraints;
drop table  PRACUJE_V cascade constraints;
drop table  PREDMET cascade constraints;
drop table  SMENA cascade constraints;
drop table  STAV_OBJEDNAVKY cascade constraints;
drop table  SUROVINA_MA_ALERGENY cascade constraints;
drop table  SUROVINY cascade constraints;
drop table  VEZEN cascade constraints;
drop table  VEZNICE cascade constraints;


CREATE TABLE Druh (
  id NUMBER(10) NOT NULL,
  nazev VARCHAR2(45) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Pecivo (
  id NUMBER(10) NOT NULL,
  nazev VARCHAR2(45) NULL,
  bezlepkove NUMBER(3) NULL,
  hmotnost NUMBER(10) NULL,
  cena NUMBER(10) NULL,
  Druh_id NUMBER(10) NOT NULL,
  PRIMARY KEY (id),
  CONSTRAINT fk_pecivo_druh_id
    FOREIGN KEY (Druh_id)
    REFERENCES Druh (id)
   )
;

CREATE TABLE Suroviny (
  id NUMBER(10) NOT NULL,
  nazev VARCHAR2(45) NULL,
  mnozstvi_skladem NUMBER(10) NULL,
  nakupni_cena NUMBER(10) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE pecivo_je_z_surovin (
  Suroviny_id NUMBER(10) NULL,
  Pecivo_id NUMBER(10) NOT NULL,
  mnozstvi NUMBER(10) NULL,
  PRIMARY KEY (Suroviny_id, Pecivo_id),
  CONSTRAINT fk_pecvivo_je_suroviny_id
    FOREIGN KEY (Suroviny_id)
    REFERENCES Suroviny (id)
   ,
  CONSTRAINT fk_pecivo_je_pecivo_id
    FOREIGN KEY (Pecivo_id)
    REFERENCES Pecivo (id)
   )
;

CREATE TABLE Alergeny (
  nazev VARCHAR2(45) NOT NULL,
  PRIMARY KEY (nazev))
;

CREATE TABLE Surovina_ma_alergeny (
  Suroviny_id NUMBER(10) NULL,
  Alergeny_nazev VARCHAR2(45) NULL,
  PRIMARY KEY (Suroviny_id, Alergeny_nazev),
  CONSTRAINT fk_surovina_ma_suroviny_id
    FOREIGN KEY (Suroviny_id)
    REFERENCES Suroviny (id)
   ,
  CONSTRAINT fk_surovina_ma_alergeny_nazev
    FOREIGN KEY (Alergeny_nazev)
    REFERENCES Alergeny (nazev)
   )
;

CREATE TABLE stav_objednavky (
  id NUMBER(10) NOT NULL,
  stav VARCHAR2(45) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Veznice (
  id NUMBER(10) NOT NULL,
  nazev VARCHAR2(45) NULL,
  adresa VARCHAR2(100) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Cela (
  cislo NUMBER(10) NULL,
  typ VARCHAR2(45) NULL,
  Veznice_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Veznice_id),
  CONSTRAINT fk_cela_veznice_id
    FOREIGN KEY (Veznice_id)
    REFERENCES Veznice (id)
   )
;

CREATE TABLE Vezen (
  id NUMBER(10) NOT NULL,
  jmeno VARCHAR2(45) NULL,
  prijmeni VARCHAR2(45) NULL,
  Cela_Veznice_id NUMBER(10) NOT NULL,
  PRIMARY KEY (id),
  CONSTRAINT fk_vezen_cela_veznice_id
    FOREIGN KEY (Cela_Veznice_id)
    REFERENCES Cela (Veznice_id)
   )
;

CREATE TABLE Paserak (
  id NUMBER(10) NOT NULL,
  jmeno VARCHAR2(45) NULL,
  prijmeni VARCHAR2(45) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Objednavka (
  termin_dodani DATE NULL,
  celkova_cena VARCHAR2(45) NULL,
  zpusob_dodani VARCHAR2(45) NULL,
  stav_objednavky_id NUMBER(10) NOT NULL,
  Vezen_id NUMBER(10) NOT NULL,
  Paserak_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Vezen_id),
  CONSTRAINT fk_stav_objednavky_id
    FOREIGN KEY (stav_objednavky_id)
    REFERENCES stav_objednavky (id)
   ,
  CONSTRAINT fk_vezen_id
    FOREIGN KEY (Vezen_id)
    REFERENCES Vezen (id)
   ,
  CONSTRAINT fk_objednavka_paserak_id
    FOREIGN KEY (Paserak_id)
    REFERENCES Paserak (id)
   )
;

CREATE TABLE Predmet (
  id NUMBER(10) NOT NULL,
  nazev VARCHAR2(45) NULL,
  skladem_kusu NUMBER(10) NULL,
  cena NUMBER(10) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Polozka_objednavky (
  pocet_kusu NUMBER(10) NULL,
  celkova_cena NUMBER(10) NULL,
  Objednavka_Vezen_id NUMBER(10) NOT NULL,
  Predmet_id NUMBER(10) NULL,
  Pecivo_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Objednavka_Vezen_id),
  CONSTRAINT fk_polozka_vezen_id
    FOREIGN KEY (Objednavka_Vezen_id)
    REFERENCES Objednavka (Vezen_id)
   ,
  CONSTRAINT fk_premdet_id
    FOREIGN KEY (Predmet_id)
    REFERENCES Predmet (id)
   ,
  CONSTRAINT fk_objednavka_pecivo_id
    FOREIGN KEY (Pecivo_id)
    REFERENCES Pecivo (id)
   )
;

CREATE TABLE Pekar (
  id NUMBER(10) NOT NULL,
  jmeno VARCHAR2(45) NULL,
  prijmeni VARCHAR2(45) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE Dozorce (
  id NUMBER(10) NOT NULL,
  jmeno VARCHAR2(45) NULL,
  prijmeni VARCHAR2(45) NULL,
  PRIMARY KEY (id))
;

CREATE TABLE je_domluven (
  paserak_id NUMBER(10) NOT NULL,
  dozorce_id NUMBER(10) NULL,
  PRIMARY KEY (paserak_id, dozorce_id),
  CONSTRAINT fk_paserak_id
    FOREIGN KEY (paserak_id)
    REFERENCES Paserak (id)
   ,
  CONSTRAINT fk_dozorce_id
    FOREIGN KEY (dozorce_id)
    REFERENCES Dozorce (id)
   )
;

CREATE TABLE Pece (
  Objednavka_Vezen_id NUMBER(10) NOT NULL,
  Pekar_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Objednavka_Vezen_id, Pekar_id),
  CONSTRAINT fk_objednavka_vezen_id
    FOREIGN KEY (Objednavka_Vezen_id)
    REFERENCES Objednavka (Vezen_id)
   ,
  CONSTRAINT fk_pekar_id
    FOREIGN KEY (Pekar_id)
    REFERENCES Pekar (id)
   )
;


CREATE TABLE Smena (
  cas_nastupu TIMESTAMP(0) NULL,
  cas_ukonceni TIMESTAMP(0) NULL,
  Veznice_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Veznice_id),
  CONSTRAINT fk_smena_veznice_id
    FOREIGN KEY (Veznice_id)
    REFERENCES Veznice (id)
   )
;


CREATE TABLE pracuje_v (
  Dozorce_id NUMBER(10) NULL,
  Veznice_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Dozorce_id, Veznice_id),
  CONSTRAINT fk_pracuje_v_dozorce_id
    FOREIGN KEY (Dozorce_id)
    REFERENCES Dozorce (id)
   ,
  CONSTRAINT fk_pracuje_v_veznice_id
    FOREIGN KEY (Veznice_id)
    REFERENCES Veznice (id)
   )
;

CREATE TABLE je_na (
  Dozorce_id NUMBER(10) NULL,
  Smena_Veznice_id NUMBER(10) NOT NULL,
  PRIMARY KEY (Dozorce_id, Smena_Veznice_id),
  CONSTRAINT fk_je_na_dozorce_id
    FOREIGN KEY (Dozorce_id)
    REFERENCES Dozorce (id)
   ,
  CONSTRAINT fk_je_na_veznice_id
    FOREIGN KEY (Smena_Veznice_id)
    REFERENCES Smena (Veznice_id)
   )
;
