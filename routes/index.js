var express = require('express');
var router = express.Router();
var fs = require("fs"),
    http = require("http"),
    url = require("url"),
    path = require("path");


/* GET home page. */
router.get('/', function(req, res, next) {
  res.sendFile('index.html', {root : __dirname});
});

router.get('/index.html', function(req,res,next){
	res.sendFile('index.html', {root : __dirname});
});

router.get('/links.html', function(req,res,next){
	res.sendFile('links.html', {root : __dirname});
});

router.get('/resume.docx', function(req,res,next){
	res.sendFile('resume.docx', {root : __dirname});
});

router.get('/viewstream.html', function(req,res,next){ //From here, route a streambuffer from a PC, send it to the client, and have it played as a stream.
	if (req.url != "videos/schwifty.mp4") {
    res.writeHead(200, { "Content-Type": "text/html" });
    res.end('<video src="http://localhost:8888/movie.mp4" controls></video>');
  } else {

	var file = path.resolve(__dirname, "videos/schwifty.mp4");
	fs.stat(file, function(err, stats){
		if(err){
			if(err.code == 'ENOENT'){
				return res.sendStatus(404);
			}
		}
		res.end(err);
	});
	var range = req.headers.range;
	if (!range){
		//Error 416 wrong range
		return res.sendStatus(416);
	}
	var positions = range.replace(/bytes=/, "").split("-");
	var start = parseInt(positions[0],10);
	var total = stats.size;
	var end = positions[1] ? parseInt(positions[1], 10) : total - 1;
	var chunksize = (end - start) + 1;

	res.writeHead(206, {
		"Content-Range": "bytes " + start + "-" + end + "/" + total,
		"Accept-Ranges": "bytes",
		"Content-Length": chunksize,
		"Content-Type": "video/mp4"
	});

	var stream = fs.createReadStream(file, {start: start, end: end})
		.on("open", function() {stream.pipe(res);}).on("error", function(err){res.end(err);});
	});


module.exports = router;
