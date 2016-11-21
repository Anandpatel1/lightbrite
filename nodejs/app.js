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

	console.log('mode = ', mode);
	switch(mode) {
		case 'stop':
			exec('pidof python', function(stderr, out, err) {
				console.log('err = ', err);
				console.log('out = ', out);
				console.log('stderr = ', stderr);
				var pid = out;
				console.log('pid = ', pid);
				var cmd = 'true';
				if(pid && pid != '') {
					console.log('Killing process');
					cmd = 'kill -9 ' + pid;
				} else {
					cmd = 'true';
				}
				exec(cmd, function(e, o, er) {
					exec('/root/mode/black.py', function(error, stdout, stderr) {
						if (error) {
							console.error('exec error: ', error);
							return;
						}
						console.log('stdout:', stdout);
						console.log('stderr:', stderr);
					});
				});
			});

			break;
		case 'lightbrite':
			exec('pidof python', function(err, out, err) {
				var pid = out;
				console.log('pid = ', pid);
				var cmd = 'true';
				if(pid && pid != '') {
					cmd = 'kill -9 ' + pid;
					console.log('Killing process');
				} else {
					cmd = 'true';
				}
				exec(cmd, function(e, o, er) {
					exec('/root/mode/light-brite.py', function(error, stdout, stderr) {
						if (error) {
							console.error('exec error: ', error);
							return;
						}
						console.log('stdout:', stdout);
						console.log('stderr:', stderr);
					});
				});
			});

			break;
		case 'twinkle':
			exec('pidof python', function(err, out, err) {
				var pid = out;
				console.log('pid = ', pid);
				var cmd = 'true';
				if(pid && pid != '') {
					cmd = 'kill -9 ' + pid;
					console.log('Killing process');
				} else {
					cmd = 'true';
				}
				exec(cmd, function(e, o, er) {
					exec('/root/mode/twinkle.py', function(error, stdout, stderr) {
						if (error) {
							console.error('exec error: ', error);
							return;
						}
						console.log('stdout:', stdout);
						console.log('stderr:', stderr);
					});
				});
			});
			break;
		case 'draw':
			exec('pidof python', function(err, out, err) {
				var pid = out;
				console.log('pid = ', pid);
				var cmd = 'true';
				if(pid && pid != '') {
					cmd = 'kill -9 ' + pid;
					console.log('Killing process');
				} else {
					cmd = 'true';
				}
				exec(cmd, function(e, o, er) {
					exec('/root/mode/draw.py', function(error, stdout, stderr) {
						if (error) {
							console.error('exec error: ', error);
							return;
						}
						console.log('stdout:', stdout);
						console.log('stderr:', stderr);
					});
				});
			});
			break;
	}

	message = 'Light Brite is in "' + mode + '" mode.';
	res.redirect('/home');

})

app.listen(80, function () {
  console.log('Example app listening on port 3000!');
});
