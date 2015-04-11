import requests

def response(context, flow):
  print "Inside python"
  #if flow.request.pretty_host(hostheader=True).contains("api.preguntados.com"):
  if ("api.preguntados.com" in flow.request.pretty_host(hostheader=True)):
    r = requests.post("http://localhost:5000", data=flow.response.content)
    print flow.response.content

