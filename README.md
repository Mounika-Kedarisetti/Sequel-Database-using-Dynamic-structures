# Sequel-Database-using-Dynamic-structures
This is a replication of sequel database using C. Dynamic structures are used to allow the user to create any schema, insert data into schema from csv files, join 2 tables, flush given table's data into a CSV file and query data using select command to display required fields in a table
insert(import), select and flush is also done using working sets where data is processed in form of chunks(here, 50 records at a time) instead of processing the whole data at once 
