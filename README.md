Cloud Paillier Cryptosystem

Server Setup:

SSH into AWS instance
Edit config file: $ sudo vim /etc/mysql/my.cnf
Change bind-address to server's private IP
Restart MySQL service: $ sudo service mysql restart
If it's taking too long to restart, make sure you have the correct IP
Compile UDF: $ make udf
Login to MySQL terminal: $ mysql -u root -p
Initialize database: mysql> source initialize.sql
Use correct database: mysql> use project;
Create function: CREATE AGGREGATE FUNCTION SUM_HE RETURNS STRING SONAME 'sum_he.so';
Give client access: GRANT ALL ON project.\* TO root@<client public IP> IDENTIFIED BY 'cs174$';
Client Setup:

Run on local machine (or SSH into a separate AWS instance from server).
Install GMP and paillier library if not done beforehand.
Paillier: acsc.cs.utexas.edu/libpaillier/
GMP: https://gmplib.org/
Change connection IP to server public IP in queries.c.
Compile and run client:
make
./client
Running the code

Client code:
make to compile
./client to run client
UDF:
make udf to compile and setup UDF code
Go through server setup to ensure updated function created and loaded into MySQL database