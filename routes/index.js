var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/viewstream.html', function(req,res,next){ //From here, route a streambuffer from a PC, send it to the client, and have it played as a stream.
});

module.exports = router;
