#-*- coding:utf-8 -*-
from datetime import timedelta  
from flask import Flask, make_response, request, current_app
from functools import update_wrapper
import json, pprint

app = Flask(__name__)


def crossdomain(origin=None, methods=None, headers=None, max_age=21600, attach_to_all=True, automatic_options=True):  
  if methods is not None:
    methods = ', '.join(sorted(x.upper() for x in methods))
    if headers is not None and not isinstance(headers, basestring):
      headers = ', '.join(x.upper() for x in headers)
    if not isinstance(origin, basestring):
      origin = ', '.join(origin)
    if isinstance(max_age, timedelta):
      max_age = max_age.total_seconds()

    def get_methods():
      if methods is not None:
        return methods

      options_resp = current_app.make_default_options_response()
      return options_resp.headers['allow']

    def decorator(f):
      def wrapped_function(*args, **kwargs):
        if automatic_options and request.method == 'OPTIONS':
          resp = current_app.make_default_options_response()
        else:
          resp = make_response(f(*args, **kwargs))
          if not attach_to_all and request.method != 'OPTIONS':
            return resp

            h = resp.headers

            h['Access-Control-Allow-Origin'] = origin
            h['Access-Control-Allow-Methods'] = get_methods()
            h['Access-Control-Max-Age'] = str(max_age)
            if headers is not None:
              h['Access-Control-Allow-Headers'] = headers
            return resp

        f.provide_automatic_options = False
        return update_wrapper(wrapped_function, f)
    return decorator



globalAnswers = '[{"question": {"category": "HISTORY", "text": "What two U.S. Presidents are on two forms of U.S. currency?", "answers": ["Jackson & Hamilton", "Ben Franklin & John F. Kennedy", "Lincoln  & George Washington", "Grant & Roosevelt"], "correct_answer": 2}}]'

@app.route("/poll")
@crossdomain(origin='*')
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
  globalAnswers = questions
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
  app.run(host='0.0.0.0', port=800)

