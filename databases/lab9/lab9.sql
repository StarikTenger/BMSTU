--Лабораторная работа №9. Триггеры DML.

-- Prepare database --

use master;
GO
IF DB_ID (N'lab9') IS NOT NULL
DROP DATABASE lab9;
GO 
CREATE DATABASE LAB9
ON ( NAME = LAB9_dat, FILENAME = 'C:\db\lab9.mdf', SIZE = 10, MAXSIZE = UNLIMITED, FILEGROWTH = 5% )
LOG ON ( NAME = LAB9_log, FILENAME = 'C:\db\lab9.ldf', SIZE = 5MB, MAXSIZE = 25MB, FILEGROWTH = 5MB );
GO

use lab9

DROP TABLE IF EXISTS Student
CREATE TABLE Student(
	StudentID INT IDENTITY(1,1) PRIMARY KEY,
	FirstName NVARCHAR(60) NOT NULL CHECK (LEN(FirstName) > 1),
	SecondName NVARCHAR(60) NOT NULL CHECK (LEN(SecondName) > 1),
	Patronymic NVARCHAR(60) NOT NULL CHECK (LEN(Patronymic) > 1),
	PhoneNumber NVARCHAR(60),
	DateOfBirth Date CHECK (DateOfBirth < DATEADD(year, -12, GETDATE())) DEFAULT DATEADD(year, -12, GETDATE()),
	RoomID INT NOT NULL
)

INSERT INTO Student (SecondName, FirstName, Patronymic, PhoneNumber, DateOfBirth, RoomID) VALUES
(N'Makaronov','Dagar','Olegovich','89116620590', '1999-12-06', '717'),
(N'Kanila',' Dryazikhin','Olegovich','88005553535', '2000-02-06', '323'),
(N'Asyutkyin',' Igorg','Olegovich','99875643728', '2000-02-06', '323'),
(N'Asyutkyin',' Igorg','Olegovich','99875643728', '2000-02-06', '323');


DROP TABLE IF EXISTS Room
CREATE TABLE Room(
	RoomID INT PRIMARY KEY,
	Capacity INT
);

INSERT INTO Room (RoomID, Capacity) VALUES
(717, 3),
(323, 3);

-- +==================================================================================================================+
-- | 1. Для одной из таблиц пункта 2 задания 7 создать триггеры на вставку, удаление и добавление,                    |
-- |   при выполнении заданных условий один из триггеров должен инициировать возникновение ошибки (RAISERROR / THROW).|
-- +==================================================================================================================+

-- trigger on INSERT with RAISERROR --

drop trigger if exists dbo.trigger_insert
go

create trigger dbo.trigger_insert on dbo.room after insert as -- for/after are the same; instead of possible
begin
	declare @min_val int = 100;
	declare @max_val int = 999;

	if exists (select *
		from inserted
		where not (inserted.RoomID >= @min_val and inserted.RoomID <= @max_val))
		begin
			delete from room  where not (RoomID >= @min_val and RoomID <= @max_val)
			raiserror ('[RoomId should have 3 digits', 2, 3); -- msg, level, state
		end
end
go

-- example

insert into room (RoomID, Capacity) values (111, 2)
insert into room (RoomID, Capacity) values (11, 2)
insert into room (RoomID, Capacity) values (1000, 2)

-- select * from Room;

-- trigger on UPDATE --
drop trigger if exists dbo.trigger_update
go

create trigger dbo.trigger_update on dbo.room after update as
		print 'table dbo.room has been updated'
go


-- trigger on DELETE --
drop trigger if exists dbo.trigger_delete
go

create trigger dbo.trigger_delete on dbo.room after delete as
begin
	delete from student where not exists (select * from room where room.RoomID = student.RoomID)-- !
	print 'Room is succesfully deleted, all students from the room are kicked out into the harsh outer world'
end
go

-- example

--select * from Student;
--delete from room where RoomID = 323;
--select * from Student;

-- +==================================================================================================+
-- | 2. Для представления пункта 2 задания 7 создать триггеры на вставку, удаление и добавление,      |
-- |    обеспечивающие возможность выполнения операций с данными непосредственно через представление. |
-- +==================================================================================================+

-- create tables
drop table if exists office
create table office(
	office_id int identity(1,1) primary key,
	city nvarchar(60) not null,
	street nvarchar(60) not null,
	building nvarchar(60) not null
)

drop table if exists company
CREATE TABLE company(
	company_id INT IDENTITY(1,1) PRIMARY KEY,
	abbreviation NVARCHAR(60) NOT NULL CHECK (LEN(abbreviation) > 1),
	full_name NVARCHAR(60) NOT NULL CHECK (LEN(full_name) > 1),
	office_id int NOT NULL unique foreign key references dbo.office (office_id)
)

-- joined view
drop view if exists joined_view;
go

create view joined_view as
	select 
		o.office_id,
		c.abbreviation, 
		c.full_name,
		o.city,
		o.street,
		o.building
	from company c 
	inner join office o
		on c.office_id = o.office_id
go

-- first of all, disable previous triggers
disable trigger dbo.trigger_delete on dbo.room;
go
disable trigger dbo.trigger_update on dbo.room;
go
disable trigger dbo.trigger_insert on dbo.room;
go

-- insert trigger --

drop trigger if exists dbo.joined_trigger_insert
go

create trigger dbo.joined_trigger_insert 
	on dbo.joined_view 
	instead of insert 
	as
	begin

		--if not (select distinct city, street, building from inserted) in (select city, street, building from office)
		--insert into dbo.office (city, street, building)
		--	select 
		--	city, street, building
		--		from inserted

		--insert into dbo.company (abbreviation, full_name, office_id)
		--		select 
		--		i.abbreviation, 
		--		i.full_name,
		--		SCOPE_IDENTITY() as office_id
		--			from inserted as i;

		declare @abbreviation varchar(60)
		declare @full_name varchar(60)
		declare @city varchar(60)
		declare @street varchar(60)
		declare @building varchar(60)

		declare cursor1 cursor for
			select abbreviation, full_name, city, street, building from inserted;

		open cursor1;
		fetch next from cursor1 into @abbreviation, @full_name, @city, @street, @building;
		while @@fetch_status = 0
		begin
			insert into dbo.office (city, street, building) values ( @city, @street, @building);
			insert into dbo.company (abbreviation, full_name, office_id) values (@abbreviation, @full_name, SCOPE_IDENTITY());
			fetch next from cursor1 into @abbreviation, @full_name, @city, @street, @building;
		end
		close cursor1;

	end
go


-- example

insert into dbo.joined_view (abbreviation, full_name, city, street, building) values
	('BMSTU', 'Bauman Moscow State Technical University', 'Moscow', '2nd Baumanskaia', '5c1'),
	('MIPT', 'Moscow Institute of Physics and Technology', 'Dolgoprudny', 'Institutskiy Pereulok', '9'),
	('HNGU', 'Lobachevsky State University of Nizhny Novgorod', 'Nizhny Novgorod', 'Ashkhabadskaya Ulitsa', '4');
	 
select * from company;
select * from office;
select * from joined_view;

-- delete trigger --

drop trigger if exists joined_trigger_delete
go

create trigger dbo.joined_trigger_delete
	on dbo.joined_view
	instead of delete
	as
	begin
		delete from dbo.office
			where office.office_id in (select d.office_id from deleted as d);
		delete from dbo.company
			where company.office_id in (select d.office_id from deleted as d);
	end
go

---- example

delete from dbo.joined_view
	where joined_view.city = 'Moscow'

select * from company;
select * from office;
select * from joined_view;

---- update trigger --

drop trigger if exists joined_trigger_update
go

create trigger dbo.joined_trigger_update
	on dbo.joined_view
	instead of update
	as 
	begin
		if UPDATE(office_id)
			RAISERROR('[UPD TRIGGER]: office_id cant be modified', 16, 1)
		
		else
			update dbo.office
				set 
					office.building = (select building from inserted where inserted.office_id = office.office_id),
					office.street = (select street from inserted where inserted.office_id = office.office_id),
					office.city = (select city from inserted where inserted.office_id = office.office_id)
				where office.office_id in (select office_id from inserted)

			update dbo.company
				set 
					company.abbreviation = (select abbreviation from inserted where inserted.office_id = company.office_id),
					company.full_name = (select full_name from inserted where inserted.office_id = company.office_id)
				where company.office_id in (select office_id from inserted)

	end
go

update dbo.joined_view 
	set city = 'Moscow'
	where joined_view.city = 'Dolgoprudny';

update dbo.joined_view 
	set office_id = 1
	where joined_view.office_id = 5;

select * from company;
select * from office;
select * from joined_view;

--select * from joined_view
--select * from Student

--update dbo.joined_view 
--	set first_name = 'Bobert'
--	where joined_view.first_name = 'aa';
