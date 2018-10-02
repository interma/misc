

# remove ending |
sed -i -e "s/|$//g" *.tbl

# import data via copy command, e.g.
# COPY part FROM '/Users/hma/hawq/install/dbgen/part.tbl' WITH DELIMITER '|';

