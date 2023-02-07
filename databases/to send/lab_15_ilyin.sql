--Задание 15. Создание распределенных баз данных со связанными таблицами средствами СУБД SQL Server 2012


--1.Создать в базах данных пункта 1 задания 13 связанные таблицы.
--2.Создать необходимые элементы базы данных (представления, триггеры), 
	--обеспечивающие работу с данными связанных таблиц (выборку, вставку, изменение, удаление).


use master;
go
drop database if exists lab15_1;
go
create database lab15_1
go


use master;
go
drop database if exists lab15_2;
go
create database lab15_2
go


--1.Создать в базах данных пункта 1 задания 13 связанные таблицы.
use lab15_1;
go
-- parent --
drop table if exists office
create table office(
	office_id int identity(1,1) primary key,
	city nvarchar(60) not null,
	street nvarchar(60) not null,
	building nvarchar(60) not null
)

use lab15_2;
go
-- child --
drop table if exists company
CREATE TABLE company(
	company_id INT IDENTITY(1,1) PRIMARY KEY,
	abbreviation NVARCHAR(60) NOT NULL CHECK (LEN(abbreviation) > 1),
	full_name NVARCHAR(60) NOT NULL CHECK (LEN(full_name) > 1),
	office_id int NOT NULL unique
)


--2.Создать необходимые элементы базы данных (представления, триггеры), 
	--обеспечивающие работу с данными связанных таблиц (выборку, вставку, изменение, удаление).


-- insert parent --

--normal insert on parent (office). trigger is not necessary

insert into company (abbreviation, full_name, office_id) values
	('BMSTU', 'Bauman Moscow State Technical University', 1),
	('MIPT', 'Moscow Institute of Physics and Technology', 2),
	('HNGU', 'Lobachevsky State University of Nizhny Novgorod', 3);


use lab15_1;
go
insert into office values
	('Moscow', '2nd Baumanskaia', '5c1'),
	('Dolgoprudny', 'Institutskiy Pereulok', '9'),
	('Nizhny Novgorod', 'Ashkhabadskaya Ulitsa', '4');

select * from office

-- delete parent -- 

use lab15_1;
go
drop trigger if exists office_del
go
create trigger office_del
	on lab15_1.dbo.office
	instead of delete
	as
	begin
		
		delete from lab15_1.dbo.office
			where office_id in (select office_id from deleted)

		delete from lab15_2.dbo.company
			where office_id in (select office_id from deleted)
	end
go

delete from office
	where office.city = 'Moscow';

select * from office
select * from lab15_2.dbo.company

-- update parent --

use lab15_1;
go
drop trigger if exists office_upd
go
create trigger office_upd
	on lab15_1.dbo.office
	instead of update
	as
	begin

		if UPDATE(office_id)
			RAISERROR('[PARENT-UPD TRIGGER]: "id" cant be modified', 16, 1);

		if UPDATE(city)
		begin
			update office
				set city = (select city from inserted where inserted.office_id = office.office_id)
				where office.office_id = (select office_id from inserted where inserted.office_id = office.office_id)
		end

	end
go

update lab15_1.dbo.office
	set city = city --'Mytishchi'
	--where city = 'Dolgoprudny'

select * from lab15_1.dbo.office



-- insert child --

use lab15_2;
go
drop trigger if exists company_ins
go
create trigger company_ins
	on lab15_2.dbo.company
	instead of insert
	as
	begin
	
		if exists (select office_id from inserted where office_id not in (select office_id from lab15_1.dbo.office))
		begin
			RAISERROR('[CHILD-INS TRIGGER]: reference to parental entry was not found!', 16, 1);
		end

		else
		begin
			insert into company (abbreviation, full_name, office_id)
				select abbreviation, full_name, office_id from inserted
		end

	end
go

insert into lab15_1.dbo.office values
	('Moscow', 'Myasnitskaya Ulitsa', '20');

insert into company values
	('HSE', 'Higher School of Economics', 4);

select * from company



-- update child --

use lab15_2;
go
if OBJECT_ID(N'company_upd', N'TR') is not null
    drop trigger company_upd
go
create trigger company_upd
	on lab15_2.dbo.company
	instead of update
	as
	begin

		if UPDATE(office_id)
			begin
				RAISERROR('[CHILD-UPD TRIGGER]: "office_id" is unupdatable', 16, 1);
			end

		if UPDATE(abbreviation) or UPDATE(full_name)
			begin
				update company
					set
						abbreviation = (select abbreviation from inserted where inserted.company_id = company.company_id),
						full_name = (select full_name from inserted where inserted.company_id = company.company_id)
					where company_id = (select company_id from inserted where inserted.company_id = company.company_id)
			end
	end
go

update company
	set abbreviation = 'МФТИ'
	where abbreviation = 'MIPT'


select * from company