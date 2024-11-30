
CREATE type my_complex AS (x int, y int);
create table test (id int, com my_complex);
insert into test values (1, (5,2));
insert into test values (generate_series(1,10000), (5,2));
analyze test;

select * from test where com=(4,2);


CREATE OR REPLACE function my_complex_abs_lt(c1 my_complex, c2 my_complex)
RETURNS BOOLEAN AS $$
DECLARE ret BOOLEAN;
BEGIN
	select ((c1).x*(c1).x+(c1).y*(c1).y) < ((c2).x*(c2).x+(c2).y*(c2).y) into ret;
	return ret;
END $$ LANGUAGE plpgsql;

CREATE OPERATOR < (
	   leftarg = my_complex, rightarg = my_complex, procedure = my_complex_abs_lt,
	   commutator = > , negator = >= ,
	   restrict = scalarltsel, join = scalarltjoinsel
);

CREATE OR REPLACE function complex_abs_cmp(c1 my_complex, c2 my_complex)
RETURNS integer AS $$
DECLARE ret integer;
BEGIN
	select ((c1).x*(c1).x+(c1).y*(c1).y) - ((c2).x*(c2).x+(c2).y*(c2).y) into ret;
	return ret;
END $$ LANGUAGE plpgsql;


CREATE OPERATOR FAMILY my_complex_ops USING btree;

CREATE OPERATOR CLASS my_complex_abs_ops
    DEFAULT FOR TYPE my_complex USING btree FAMILY my_complex_ops AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       >;
		FUNCTION        1       complex_abs_cmp(my_complex, my_complex);

CREATE OPERATOR CLASS my_complex_abs_ops
    DEFAULT FOR TYPE my_complex USING btree FAMILY my_complex_ops AS
        OPERATOR        1       < ,
		FUNCTION        1       complex_abs_cmp(my_complex, my_complex);

create index on test(com);

set enable_seqscan to false;
explain select * from test where com < (5,2)::my_complex;
explain select * from test where com = (5,2)::my_complex; -- seqscan

-- select catelog table

select oid,* from pg_opfamily order by oid desc limit 5;
-- oid == 24642

select
pg_am.amname, pg_opfamily.opfname,
pg_operator.oprname, pg_amop.amopstrategy,
pg_typeleft.typname, pg_typeright.typname
from
pg_amop, pg_am, pg_opfamily, pg_operator,
pg_type pg_typeleft, pg_type pg_typeright
where
pg_am.oid=403 and pg_amop.amopmethod=pg_am.oid and
pg_amop.amopfamily=24642 and pg_amop.amopfamily=pg_opfamily.oid and
pg_amop.amopopr=pg_operator.oid and
pg_amop.amoplefttype=pg_typeleft.oid and pg_amop.amoprighttype=pg_typeright.oid;


select
pg_opfamily.opfname, pg_proc.proname, pg_amproc.amprocnum,
pg_typeleft.typname, pg_typeright.typname
from
pg_amproc, pg_opfamily, pg_proc,
pg_type pg_typeleft, pg_type pg_typeright
where
pg_amproc.amprocfamily=24642 and pg_amproc.amprocfamily=pg_opfamily.oid
and pg_amproc.amproc=pg_proc.oid and pg_amproc.amproclefttype=pg_typeleft.oid and pg_amproc.amprocrighttype=pg_typeright.oid;

