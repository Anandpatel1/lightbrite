var express = require('express');
var jade = require('jade');
var logger = require('morgan');
var bodyParser = require('body-parser');
var session = require('cookie-session');
const exec = require('child_process').exec;

var app = express();

app.set('views', __dirname + '/views');
app.set('view engine', 'jade')
app.use(logger('common'));

app.use(express.static(__dirname + '/public')); //setup static public directory

var message = '';

app.get('/', function (req, res) {
  res.redirect('/home');
});

app.get('/home', function(req, res) {
	res.render('home.jade', {message: message})
});

app.get('/mode/:mode', function(req, res) {
	var mode = req.params.mode;

	switch(mode) {
		case 'stop':
			exec('kill -9 `pidof python` && /root/mode/black.py', (error, stdout, stderr) => {
				if (error) {
					console.error(`exec error: ${error}`);
					return;
				}
				console.log(`stdout: ${stdout}`);
				console.log(`stderr: ${stderr}`);
			});

			break;
		case 'lightbrite':
			exec('kill -9 `pidof python` && /root/mode/light-brite.py', (error, stdout, stderr) => {
				if (error) {
					console.error(`exec error: ${error}`);
					return;
				}
				console.log(`stdout: ${stdout}`);
				console.log(`stderr: ${stderr}`);
			});

			break;
		case 'twinkle':
			exec('kill -9 `pidof python` && /root/mode/twinkle.py', (error, stdout, stderr) => {
				if (error) {
					console.error(`exec error: ${error}`);
					return;
				}
				console.log(`stdout: ${stdout}`);
				console.log(`stderr: ${stderr}`);
			});

			break;
		case 'draw':
			exec('kill -9 `pidof python` && /root/mode/draw.py', (error, stdout, stderr) => {
				if (error) {
					console.error(`exec error: ${error}`);
					return;
				}
				console.log(`stdout: ${stdout}`);
				console.log(`stderr: ${stderr}`);
			});

			break;
	}

	message = 'Light Brite is in "' + mode + '" mode.';
	res.redirect('/home');

})

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});