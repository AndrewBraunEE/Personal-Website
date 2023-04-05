var express = require('express');
var router = express.Router();
var fs = require("fs"),
    http = require("http"),
    url = require("url"),
    path = require("path");
var getelement = require("get-element");
var request = require("request"),
    cheerio = require("cheerio")
    //processclass = require("./libs/build/Release/classes.node");


//Example: https://sa.ucla.edu/ro/Public/SOC/Results?t=17F&sBy=subject&sName=Aerospace+Studies+%28AERO+ST%29&subj=AERO+ST&crsCatlg=Enter+a+Catalog+Number+or+Class+Title+%28Optional%29&catlg=&cls_no=&btnIsInIndex=btn_inIndex
/*
class search_result{
	constructor(term, classname, classcode){ //Term = 17F , classname = Aerospace+Studies , classcode = "AERO+ST"
		this.classname = classname;
		this.classcode = classcode;
		this.string = "https://sa.ucla.edu/ro/Public/SOC/Results?t=" + term + "&sBy=subject&sName=" + classname + "%28" + classcode + "%29&subj=" + classcode + "&crsCatlg=Enter+a+Catalog+Number+or+Class+Title+%28Option%29&catlg=&cls_no=&btnIsInIndex=btn_inIndex";
	};
};
*/ /*
function genClasses(url){
  this.url = url;
  this.docname = "ucla_register.html"
  request(url,function(error, response, html){ //gets html page, saves it to docname.
    if(!error){
      var $ = cheerio.load(html);
     // console.log(html); 
      var rawsubjects = $("body").find("subjects").text().trim();  
      console.log(rawsubjects);
      
    }
    else if(error){
    console.log(error);
  }
  }).pipe(fs.createWriteStream(this.docname));
  /*this.generateData = function generateData(){
    if(this.html != undefined){//Algorithm here
      console.log(html);
    }
    console.log("HTML File could not be found")
  };*/
//}
/*
var classgenerator = new genClasses("https://sa.ucla.edu/ro/Public/SOC/Results?t=17F&sBy=subject&sName=Aerospace+Studies+%28AERO+ST%29&subj=AERO+ST&crsCatlg=Enter+a+Catalog+Number+or+Class+Title+%28Optional%29&catlg=&cls_no=&btnIsInIndex=btn_inIndex");
console.log(classgenerator.HTML);

function getStatus(in_search){
	//getelement.
};
*/

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