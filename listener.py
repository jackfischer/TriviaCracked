#-*- coding:utf-8 -*-
from flask import Flask
from flask import request
import json, pprint

app = Flask(__name__)

@app.route("/", methods=['GET', 'POST'])
def hello():
  print "\n\nInside flask, got Post... "
  data = json.loads(request.data)

  #check if crown case
  #qType = "NORMAL"
  #if (data["spins_data"]["spins"][0]["type"] is "CROWN"):
  #  qType = "CROWN"

  #narrow json down to just the questions core
  questions = data["spins_data"]["spins"][0]["questions"]

  #clean up powerups, extra baggage
  for i in range (0,5):
    del questions[i]["powerup_question"]
    del questions[i]["question"]["author"]
    del questions[i]["question"]["id"]
    del questions[i]["question"]["media_type"]

  #add the nice "type" flag for chris
  #api["type"] = qType
  #api["questions"] = questions

  print(json.dumps(questions))


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
  app.run()

