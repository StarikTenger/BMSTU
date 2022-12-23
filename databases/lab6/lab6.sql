--Лабораторная работа №6. Ключи, ограничения, значения по умолчанию


use master;
GO
IF DB_ID (N'LAB6') IS NOT NULL
DROP DATABASE LAB6;
GO 
CREATE DATABASE LAB6
ON ( NAME = LAB6_dat, FILENAME = 'C:\db\lab6.mdf', SIZE = 10, MAXSIZE = UNLIMITED, FILEGROWTH = 5% )
LOG ON ( NAME = LAB6_log, FILENAME = 'C:\db\lab6.ldf', SIZE = 5MB, MAXSIZE = 25MB, FILEGROWTH = 5MB );
GO

--1.Создать таблицу с автоинкрементным первичным ключом.
--2.Добавить поля, для которых используются ограничения (CHECK), значения по умолчанию(DEFAULT), также использовать функции для вычисления.

USE LAB6;
DROP TABLE IF EXISTS Student
CREATE TABLE Student(
	StudentID INT IDENTITY(1,1) PRIMARY KEY,
	FirstName NVARCHAR(60) NOT NULL CHECK (LEN(FirstName) > 1),
	SecondName NVARCHAR(60) NOT NULL CHECK (LEN(SecondName) > 1),
	Patronymic NVARCHAR(60) NOT NULL CHECK (LEN(Patronymic) > 1),
	PhoneNumber NVARCHAR(60) NOT NULL CHECK (LEN(PhoneNumber) > 10),
	DateOfBirth Date CHECK (DateOfBirth < DATEADD(year, -12, GETDATE())) DEFAULT DATEADD(year, -12, GETDATE()),
	RoomID INT NOT NULL
)


INSERT INTO Student (SecondName, FirstName, Patronymic, PhoneNumber, DateOfBirth, RoomID) VALUES
(N'Makaronov',' Dagar','Olegovich','89116620590', '1999-12-06', '717'),
(N'Kanila',' Dryazikhin','Olegovich','88005553535', '2000-02-06', '323'),
(N'Asyutkyin',' Igorg','Olegovich','99875643728', '2000-02-06', '323'),
(N'Asyutkyin',' Igorg','Olegovich','99875643728', '2000-02-06', '323');

-- TEST -----------------------
drop table if exists TestTable;
create table TestTable(a int identity(1,1) primary key, b int);
insert into TestTable(b) values (1), (3), (2);

--SELECT * FROM Student;

select @@IDENTITY as id;
select SCOPE_IDENTITY() as scope_id;
select IDENT_CURRENT('Student') as id_current;


--3.Создать таблицу с первичным ключом на основе глобального уникального идентификатора.

DROP TABLE IF EXISTS Revision;
CREATE TABLE Revision(
	RevisionID UNIQUEIDENTIFIER PRIMARY KEY DEFAULT(NEWID()),
	RevisionDate DATE,
	Comment VARCHAR(500),
	RoomId INT,
);

INSERT INTO Revision (RoomId, RevisionDate, Comment) VALUES
('717', '2022-11-11', 'хорошо'),
('717', '2022-11-12', 'хорошо, но не очень');

--SELECT * FROM Revision;

--4.Создать таблицу с первичным ключом на основе последовательности.


DROP SEQUENCE IF EXISTS FloorSeq;
CREATE SEQUENCE FloorSeq 
    START WITH 1
    INCREMENT BY 1
    MAXVALUE 10;

DROP TABLE IF EXISTS DormFloor;
CREATE TABLE DormFloor(
	DormFloorID INT PRIMARY KEY,
	Area FLOAT,
	EmergencyExits INT
);

INSERT INTO DormFloor(DormFloorID, Area, EmergencyExits) VALUES
(NEXT VALUE FOR FloorSeq, '987.44', '4'),
(NEXT VALUE FOR FloorSeq, '983.93', '1'),
(NEXT VALUE FOR FloorSeq, '979.71', '1'),
(NEXT VALUE FOR FloorSeq, '977.37', '1'),
(NEXT VALUE FOR FloorSeq, '977.13', '2');

--SELECT * FROM DormFloor;

--5.Создать две связанные таблицы, и протестировать на них различные варианты действий для ограничений ссылочной целостности (NO ACTION| CASCADE | SET NULL | SET DEFAULT).

DROP TABLE IF EXISTS Room
CREATE TABLE Room(
	RoomID INT PRIMARY KEY,
	Capacity INT
);

DROP TABLE IF EXISTS Student
CREATE TABLE Student(
	StudentID INT IDENTITY(1,1) PRIMARY KEY,
	FirstName NVARCHAR(60) NOT NULL CHECK (LEN(FirstName) > 1),
	SecondName NVARCHAR(60) NOT NULL CHECK (LEN(SecondName) > 1),
	Patronymic NVARCHAR(60) NOT NULL CHECK (LEN(Patronymic) > 1),
	PhoneNumber NVARCHAR(60) NOT NULL CHECK (LEN(PhoneNumber) > 10),
	DateOfBirth Date CHECK (DateOfBirth < DATEADD(year, -12, GETDATE())) DEFAULT DATEADD(year, -12, GETDATE()),
	RoomID INT
	CONSTRAINT FK_RoomID FOREIGN KEY (RoomID) REFERENCES Room (RoomID)
		ON UPDATE CASCADE --каскадное изменение ссылающихся таблиц;
		--ON UPDATE NO ACTION --выдаст ошибку при удалении/изменении
		--ON UPDATE SET NULL --установка NULL для ссылающихся внешних ключей;
		--ON UPDATE SET DEFAULT --установка значений по умолчанию для ссылающихся внешних ключей;
		ON DELETE SET NULL
		--ON DELETE NO ACTION
		--ON DELETE SET DEFAULT
		--ON DELETE CASCADE
)


INSERT INTO Room (RoomID, Capacity) VALUES
(717, 3),
(323, 3);

INSERT INTO Student (SecondName, FirstName, Patronymic, PhoneNumber, DateOfBirth, RoomID) VALUES
(N'Makaronov',' Dagar','Olegovich','89116620590', '1999-12-06', '717'),
(N'Kanila',' Dryazikhin','Olegovich','88005553535', '2000-02-06', '323'),
(N'Asyutkyin',' Igorg','Olegovich','99875643728', '2000-02-06', '323');

SELECT * FROM Room;
SELECT * FROM Student;


DELETE FROM Room WHERE RoomID=717

SELECT * FROM Room;
SELECT * FROM Student;