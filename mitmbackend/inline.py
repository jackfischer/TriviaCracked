
#runs integrated with mitmdump via the -s flag.
#parses around all the crap traffic for whats going to preguntados.

import requests

def response(context, flow):
  #print "Inside python"
  #if flow.request.pretty_host(hostheader=True).contains("api.preguntados.com"):

  #ignore requests to analytics services and native background stuff
  if ("api.preguntados.com" in flow.request.pretty_host(hostheader=True)):
    print flow.response.content #see everything going to preguntados
    #ignore requests for profile data, etc
    if ("answer" in flow.request.path or "games" in flow.request.path):       
      #post request to LOCAL flask listener.
      r = requests.post("http://localhost:800", data=flow.response.content)


