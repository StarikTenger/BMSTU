USE [master]
GO
IF DB_ID (N'Dormitory') IS NOT NULL
DROP DATABASE Dormitory;
GO

--1.������� ���� ������ (CREATE DATABASE�, ����������� �������� �������� ������).

CREATE DATABASE Dormitory
ON PRIMARY
(
	NAME = Dormitory_PrimaryData,
	FILENAME = 'C:\db\lab5.mdf', 
	SIZE = 10,
	MAXSIZE = UNLIMITED, 
	FILEGROWTH = 5%
),
(
	NAME = Controller_SecondaryData,
	FILENAME = 'C:\db\lab5.ndf' 
)
LOG ON
(
	NAME = Controller_Log,
	FILENAME = 'C:\db\lab5.ldf',
	SIZE = 5MB,
	MAXSIZE = 25MB,
	FILEGROWTH = 5%
)
GO

--2.������� ������������ �������(CREATE TABLE�).

USE Dormitory
GO

DROP TABLE IF EXISTS Student

CREATE TABLE Student(
	StudentID INT NOT NULL,
	FirstName NVARCHAR(60) NOT NULL,
	SecondName NVARCHAR(60) NOT NULL,
	Patronymic NVARCHAR(60) NOT NULL,
	PhoneNumber NVARCHAR(60) NOT NULL,
	Schedule NVARCHAR(60) NOT NULL,
	DateOfBirth Date,
	RoomID INT
)

GO

SELECT
  *
FROM
  Student;
GO

--3.�������� �������� ������ � ���� ������(ALTER DATABASE�).

ALTER DATABASE Dormitory
ADD FILEGROUP LargeFileGroup;  
GO

ALTER DATABASE Dormitory
ADD FILE(
    NAME = Dormitory_LargeData,  
    FILENAME = 'C:\db\Dormitory_db2.ndf',  
    SIZE = 5MB,  
    MAXSIZE = 25MB,  
    FILEGROWTH = 5% 
)
TO FILEGROUP LargeFileGroup
GO

--4.������� ��������� �������� ������ �������� ������� �� ���������.

ALTER DATABASE Dormitory
	MODIFY FILEGROUP LargeFileGroup DEFAULT;
GO

IF OBJECT_ID(N'Room', N'U') is not null
	DROP TABLE Room;
GO
CREATE TABLE Room (
	RoomID INT
	);
GO


ALTER DATABASE Dormitory
	MODIFY FILEGROUP [primary] DEFAULT;
GO

--5.(*) ������� ��� ���� ������������ �������.

-- backup
select * into room_backup from room;

drop table room;

select * from room_backup;


--6.(*) ������� ��������� ������� �������� ������.

ALTER DATABASE Dormitory
	REMOVE FILE Dormitory_LargeData;
GO
ALTER DATABASE Dormitory
	REMOVE FILEGROUP LargeFileGroup;
GO

--7.������� �����, ����������� � ��� ���� �� ������, ������� �����.
IF SCHEMA_ID(N'DormitorySchema') is not null
	DROP schema DormitorySchema;
GO
create schema DormitorySchema;
GO

--modify schema
ALTER SCHEMA DormitorySchema 
	TRANSFER Student;
GO
if OBJECT_ID(N'DormitorySchema.Student', N'U') is not null
	DROP TABLE DormitorySchema.Student;
GO

--drop schema
DROP SCHEMA DormitorySchema;
GO