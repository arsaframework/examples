# Connecting PHP to MySQL with CRUD
In this article, we will look at how to use PHP to connect to MySQL and perform basic CRUD operations. 

## Prerequisites
Before we get started, you will need to install the following:
- MySQL Server
- PHP
 
## Connecting to MySQL
Once you have your MySQL Server setup and running, you can begin by writing the code to connect to it. 

Start by setting up some variables to contain the connection credentials:

```php 
<?php 
    $host = 'localhost';
    $user = 'root';
    $password = 'password';
    $dbname = 'dbname';
 
    // create connection
    $conn = mysqli_connect($host, $user, $password, $dbname); 
?>
```   

Once these are set, you can connect to your server using the mysqli_connect() function.

## Performing Basic CRUD Operations in PHP
Now that you've connected to the MySQL database, you can begin to perform some simple create, read, update, and delete (CRUD) operations. 

### Create
The first operation you might want to do is to write data to the database. This is known as an insert or create operation. 

To do this, you'll need to write an SQL query. The syntax for this is a bit different from the usual PHP syntax but it can be written in PHP like this: 

```php 
<?php 
    // insert data into database 
    $sql = "INSERT INTO yourTable (column1, column2) VALUES ('value1', 'value2')";
    mysqli_query($conn, $sql); 
?>
``` 

### Read
The next operation is to read data from the database. This is known as a select or read operation. 

Again, you'll need to write an SQL query to get the data. This time the syntax is as follows: 

```php 
<?php 
    // select data from database 
    $sql = "SELECT * FROM yourTable"; 
    $result = mysqli_query($conn, $sql); 
 
    // fetch data from results 
    if (mysqli_num_rows($result) > 0) { 
        while ($row = mysqli_fetch_assoc($result)) { 
            echo $row['column1']; 
        } 
    }
?>
``` 

### Update
The third operation is to update data already stored in the database. This is known as an update or modify operation. 

Once again, you'll need to write an SQL query. The syntax is as follows: 

```php 
<?php 
    // update data in database 
    $sql = "UPDATE yourTable SET column1 = 'value1' WHERE someColumn = someValue"; 
    mysqli_query($conn, $sql); 
?>
``` 

### Delete
The fourth and final operation is to delete data from the database. This is known as a delete operation. 

Once again, you'll need to write an SQL query. The syntax is as follows: 

```php 
<?php 
    // delete data from database 
    $sql = "DELETE FROM yourTable WHERE someColumn = someValue"; 
    mysqli_query($conn, $sql); 
?>
``` 


## Conclusion 
In this tutorial, we looked at how to connect PHP to MySQL and perform basic CRUD operations. We began by setting up the connection variables, then we wrote code to execute our queries. 

Now that you have the basic knowledge of how to connect PHP to MySQL, you can start writing your own code to read from and write to the database.