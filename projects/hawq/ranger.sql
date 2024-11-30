-- RUN AS SUPERUSER 

-- role
drop role if exists user1;
create role user1 with login;

-- database
drop database IF EXISTS da;
create database da;
\c da

-- language
create table a(i int);
create language plpythonu;
CREATE OR REPLACE FUNCTION f4() 
	RETURNS TEXT AS $$ plpy.execute("select * from a") $$ 
	LANGUAGE plpythonu VOLATILE;
select * from f4();
drop function f4();
drop language plpythonu;


-- operator and operator class
CREATE OR REPLACE FUNCTION normalize_si(text)
	RETURNS text AS $$
	BEGIN
	RETURN substring($1, 9, 2) ||
			substring($1, 7, 2) || substring($1, 5, 2) || substring($1, 1, 4);
	END; 
	$$LANGUAGE 'plpgsql' IMMUTABLE;
CREATE OR REPLACE FUNCTION si_lt(text, text)
	RETURNS boolean AS $$
	BEGIN
			RETURN normalize_si($1) < normalize_si($2);
	END;
	$$ LANGUAGE 'plpgsql' IMMUTABLE;
CREATE OPERATOR <# ( PROCEDURE=si_lt,LEFTARG=text, RIGHTARG=text);
CREATE OR REPLACE FUNCTION si_same(text, text)
	RETURNS int AS $$
	BEGIN
		IF      normalize_si($1) < normalize_si($2)
			THEN
			RETURN -1;
		END IF;
	END;
	$$ LANGUAGE 'plpgsql' IMMUTABLE;
CREATE OPERATOR CLASS sva_special_ops
	FOR TYPE text USING btree AS
	OPERATOR        1       <#  ,
	FUNCTION        1       si_same(text, text);

drop OPERATOR CLASS sva_special_ops USING btree;
drop OPERATOR <# (text,text) CASCADE;


-- resource queue
CREATE RESOURCE QUEUE myqueue WITH (PARENT='pg_root', ACTIVE_STATEMENTS=20,
MEMORY_LIMIT_CLUSTER=50%, CORE_LIMIT_CLUSTER=50%);   
DROP RESOURCE QUEUE myqueue;

-- tablespace (create a filespace first: hawq filespace -h 127.0.0.1 --config filespace_example_config)
CREATE TABLESPACE mytblspace FILESPACE myfilespace;    
CREATE TABLE foo(i int) TABLESPACE mytblspace;
insert into foo(i) values(1234);
drop table foo;
drop tablespace mytblspace;

-- copy (generate a.txt first)
COPY a FROM '/tmp/a.txt';
COPY a TO STDOUT WITH DELIMITER '|';
drop table a;

-- external table (run gpfdist first: gpfdist -p 8081 -d .)
CREATE EXTERNAL TABLE ext_t 
   (i int) 
   LOCATION ( 'gpfdist://localhost:8081/*.csv' ) 
   FORMAT 'CSV' ( DELIMITER ',' ); 
select * from ext_t;   
CREATE WRITABLE EXTERNAL TABLE ext_t2 (LIKE ext_t) 
   LOCATION ('gpfdist://localhost:8081/t.out')
   FORMAT 'TEXT' ( DELIMITER '|' NULL ' ');
insert into ext_t2(i) values(234);

drop EXTERNAL TABLE ext_t;
drop EXTERNAL TABLE ext_t2;




-- RUN AS NORMAL USER (user1)

-- switch user
set role user1;

-- create 
create schema sa;
drop table if exists a;
create table a(i int);
create temp table ta(i int);
create view av as select * from a;
create table aa as select * from a;
create table sa.t(a int, b int);
CREATE SEQUENCE myseq START 101;

-- insert
insert into a values(1);
insert into a values(1);
insert into a VALUES (nextval('myseq'));

-- select
select * from a;
select generate_series(1,3);
select * from av;
SELECT setval('myseq', 201);
SELECT * INTO aaa FROM a WHERE i > 2;

-- prepare
PREPARE fooplan (int) AS INSERT INTO a VALUES($1);
EXECUTE fooplan(300);
DEALLOCATE fooplan;

-- explain
explain select * from a;

-- aggregate
CREATE FUNCTION scube_accum(numeric, numeric) RETURNS numeric 
    AS 'select $1 + $2 * $2 * $2' 
    LANGUAGE SQL 
    IMMUTABLE 
    RETURNS NULL ON NULL INPUT;
CREATE AGGREGATE scube(numeric) ( 
    SFUNC = scube_accum, 
    STYPE = numeric, 
    INITCOND = 0 );
ALTER AGGREGATE scube(numeric) RENAME TO scube2;   
DROP AGGREGATE scube2(numeric);

-- type
CREATE TYPE mytype AS (f1 int, f2 int);
CREATE FUNCTION getfoo() RETURNS SETOF mytype AS $$
    SELECT i,i FROM a
$$ LANGUAGE SQL;
select getfoo();
drop type mytype cascade;

-- declare
begin;
DECLARE mycursor CURSOR FOR SELECT * FROM a;
FETCH FORWARD 2 FROM mycursor;
commit;

-- savepoint
BEGIN;
    INSERT INTO a VALUES (3);
    SAVEPOINT my_savepoint;
    INSERT INTO a VALUES (4);
    RELEASE SAVEPOINT my_savepoint;
COMMIT;

-- reset
RESET hawq_rm_stmt_vseg_memory; 

-- set
SET search_path TO public, sa;

-- owned
--REASSIGN OWNED BY sally, bob TO admin;

-- \d
\d

-- misc
analyze a;
analyze;
vacuum aa;
vacuum analyze;
truncate aa;

-- alter
alter table a rename column i to j;

-- drop
drop table ta;
drop view av;
drop table aa;
drop table a;
drop schema sa CASCADE;
DROP OWNED BY user1;
    

   
    


  
