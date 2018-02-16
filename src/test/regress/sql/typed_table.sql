CREATE TABLE ttable1 OF nothing;

CREATE TYPE person_type AS (id int, name text);
CREATE TABLE persons OF person_type;
SELECT * FROM persons;
\d persons

CREATE FUNCTION get_all_persons() RETURNS SETOF person_type
LANGUAGE SQL
AS $$
    SELECT * FROM persons;
$$;

SELECT * FROM get_all_persons();

ALTER TABLE persons ADD COLUMN comment text;
ALTER TABLE persons DROP COLUMN name;
ALTER TABLE persons RENAME COLUMN id TO num;

CREATE TABLE personsx OF person_type (myname WITH OPTIONS NOT NULL); -- error

-- This test comes from postgres, and we expect it to fail on Greenplum
-- because Greenplum does not support both primary keys and unique constraints
-- at the same time.
CREATE TABLE persons2 OF person_type (
    id WITH OPTIONS PRIMARY KEY,
    UNIQUE (name)
);

\d persons2

-- These are added for Greenplum, as the previous table creation statement
-- should have failed.
CREATE TABLE gppersons1 OF person_type (
    id WITH OPTIONS PRIMARY KEY
);

\d gppersons1

CREATE TABLE gppersons2 OF person_type (
    UNIQUE (name)
);

\d gppersons2

CREATE TABLE persons3 OF person_type (
    PRIMARY KEY (id),
    name WITH OPTIONS DEFAULT ''
);

\d persons3

CREATE TABLE persons4 OF person_type (
    name WITH OPTIONS NOT NULL,
    name WITH OPTIONS DEFAULT ''  -- error, specified more than once
);

DROP TYPE person_type RESTRICT;
DROP TYPE person_type CASCADE;
