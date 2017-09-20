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

router.get('/schwifty.html', function(req,res,next){ //Works only when you click AndrewBraunEE
	res.sendFile('schwifty.html', {root : __dirname});
});

router.get('/schwifty.mp4', function(req,res,next){ //Works to stream a mp4 file.
  const path = 'public/videos/schwifty.mp4'
  const stat = fs.statSync(path)
  const fileSize = stat.size
  const range = req.headers.range
  if (range) {
    const parts = range.replace(/bytes=/, "").split("-")
    const start = parseInt(parts[0], 10)
    const end = parts[1] 
      ? parseInt(parts[1], 10)
      : fileSize-1
    const chunksize = (end-start)+1
    const file = fs.createReadStream(path, {start, end})
    const head = {
      'Content-Range': `bytes ${start}-${end}/${fileSize}`,
      'Accept-Ranges': 'bytes',
      'Content-Length': chunksize,
      'Content-Type': 'video/mp4',
    }
    res.writeHead(206, head);
    file.pipe(res);
  } else {
    const head = {
      'Content-Length': fileSize,
      'Content-Type': 'video/mp4',
    }
    res.writeHead(200, head)
    fs.createReadStream(path).pipe(res)
  }
});

module.exports = router;
