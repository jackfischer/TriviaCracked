#-*- coding:utf-8 -*-
from flask import Flask
from flask import request
import json, pprint

app = Flask(__name__)

@app.route("/", methods=['GET', 'POST'])
def hello():
  print "\nInside flask, got Post... "
  data = json.loads(request.data)
  #question = data["spins_data"]["spins"][0]["questions"][0]["question"]["correct_answer"]
  question = data["spins_data"]["spins"][0]["questions"][0]["question"]
  answerIndex = question["correct_answer"]
  answerContent = question["answers"][int(answerIndex)]

  #pp = pprint.PrettyPrinter(indent=4)
  #pp.pprint(answerIndex)

  print("Correct answer's index: ")
  print(answerIndex)
  print(answerContent)
  #print "Answer index: " + answerIndex
  #print "CORRECT ANSWER: " + question["answers"][answerIndex]
  return request.data

if __name__ == "__main__":
  app.run()

