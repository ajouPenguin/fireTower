var express = require('express');         // Use express framework.
var app = express();
var mysql = require('mysql');             // Use mysql
var bodyParser = require('body-parser');  // Need to parsing post's body
var conn = mysql.createConnection({       // Connect to the database that matches the properties listed below.
  host      : 'localhost',                // Localhost database
  user      : 'root',                     // Database user
  password  : '1234',                     // User's password
  database  : 'fire'                      // Database's name
});

app.use(bodyParser.urlencoded({extended: false}));

conn.connect();                           // Connect with database.

app.get('/:id', function(req, res){       // When get messages come through 'localhost/(id)' URL
  var id = req.params.id;                 // Get number (id)
  var sql = 'SELECT * FROM fire WHERE id=' + id; // Make query 'SELECT * FROM fire WHERE id=(id)'

  if(id == '*')                           // If id equals '*' then select all data.
  {
    sql = 'SELECT * FROM fire';
  }
  conn.query(sql, function(err, rows, fields){    // request query.
    if(err){
      console.log(err);
      res.send(err);
    }
    else{
      console.log('rows', rows);
      res.send(rows);                             // If error doesn't occur then
    }
  });
});

app.post('/post', function(req, res){      // When post messages come.
  var date = req.body.date;                // parse date.
  var water = req.body.water;              // parse remaining water value.
  console.log(req.body);                   // print log for debugging
  var insert = 'INSERT INTO fire(date, water) VALUES("' + date + '",' + water + ')';    // Make query 'INSERT INTO fire(date, water) VALUES('(date)', (water))'

  conn.query(insert, function(err, rows, fields){   // request query.
    if(err){
      console.log(err);
      res.send(err);
    }
    else{
      console.log(err);
      res.send(rows);
    }
  });
  console.log(insert);
});

app.listen(9000, function(){
  console.log('listen to 9000 port');
});
