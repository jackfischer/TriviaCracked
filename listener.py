#-*- coding:utf-8 -*-
from flask import Flask, make_response, request, current_app
from functools import update_wrapper
import json, pprint
from flask.ext.headers import headers

app = Flask(__name__)

#Placeholder question
globalAnswers = '[{"question": {"category": "HISTORY", "text": "What two U.S. Presidents are on two forms of U.S. currency?", "answers": ["Jackson & Hamilton", "Ben Franklin & John F. Kennedy", "Lincoln  & George Washington", "Grant & Roosevelt"], "correct_answer": 2}}]'


@app.route("/poll")
@headers({'Access-Control-Allow-Origin':'*', 'Content-Type':'application/json'})
def answer():
  global globalAnswers
  return globalAnswers

@app.route("/", methods=['GET', 'POST'])
def hello():
  global globalAnswers
  print "\n\nInside flask, got Post... "
  data = json.loads(request.data)

  #check if crown case
#  qType = "NORMAL"
#  if (data["spins_data"]["spins"][0]["type"] is "CROWN"):
#    qType = "CROWN"

  print data
  #narrow json down to just the questions core
  questions = data["spins_data"]
  questions = questions["spins"]
  questions = questions[0]["questions"]

  #clean up powerups, extra baggage
  for i in range (len(questions)):
    del questions[i]["powerup_question"]
    del questions[i]["question"]["author"]
    del questions[i]["question"]["id"]
    del questions[i]["question"]["media_type"]

#  print questions
  #add the nice "type" flag for chris
 # api["type"] = qType
 # api["questions"] = questions

  #globalAnswers = json.dumps(questions)
  globalAnswers = json.dumps(questions)
  print(json.dumps(questions))
  return json.dumps(questions)


  ###################
  #Old model, works

  #question = data["spins_data"]["spins"][0]["questions"][0]["question"]
  #answerIndex = question["correct_answer"]
  #answerContent = question["answers"][int(answerIndex)]

  #print("Correct answer's index: ")
  #print(answerIndex)
  #print(answerContent)
  #return request.data

  #pp = pprint.PrettyPrinter(indent=4)
  #pp.pprint(answerIndex)
  ###################



if __name__ == "__main__":
  app.run(host='0.0.0.0', port=800, debug=True)

