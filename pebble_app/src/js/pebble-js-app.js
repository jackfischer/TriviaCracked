var checkMe = null;
var answerList = []
var categoryList = []
var questionList = []

function sendSpecificQuestion(index) {
		var id = Pebble.sendAppMessage({ '3': categoryList[index], '2': answerList[index], '1': questionList[index] },
				function(e) {
					console.log('Successfully delivered app message');
				},
				function(e) {
					console.log('Unable to deliver app message');
				});
}

function buildAndSendAppMessage() {
	if (categoryList.length == 1) {
		var id = Pebble.sendAppMessage({ '3': categoryList[0], '2': answerList[0], '1': questionList[0] },
				function(e) {
					console.log('Successfully delivered app message');
				},
				function(e) {
					console.log('Unable to deliver app message');
				});
	} else {
		delimitedCategoryList = [];
		for(var i = 0; i < categoryList.length; i++) {
			delimitedCategoryList.push(categoryList[i]);
			delimitedCategoryList.push('|');
		}
		var id = Pebble.sendAppMessage({ '4': categoryList.length, '3': delimitedCategoryList, '0': 1 },
				function(e) {
					console.log('Successfully delivered app message');
				},
				function(e) {
					console.log('Unable to deliver app message');
				});
	}
}

function pollPage() {
	var req = new XMLHttpRequest();
	req.open('GET', 'http://triviacracked.trump6.com:800/poll', true);
	req.onload = function(e) {
		if (req.readyState == 4 && req.status == 200) {
			var response = JSON.parse(req.responseText);
			if (response[0].question.text !== checkMe) {
				checkMe = response[0].question.text;
				answerList.length = 0;
				categoryList = []
				answerList = []
				questionList = []
				for (var i = 0; i < response.length; i++) {
					categoryList.push(response[i].question.category);
					answerList.push(response[i].question.answers[response[i].question.correct_answer]);
					questionList.push(response[i].question.text);
				}
				buildAndSendAppMessage();
			}
		}
	}
	req.send();
}

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
	pollPage();
});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage", function(e) {
	if ("REFRESH" in e.payload) {
		pollPage();
		console.log("Refresh request received");
	}

	if ("REQUEST" in e.payload) {
		sendSpecificQuestion(e.payload.REQUEST);
	}
});
