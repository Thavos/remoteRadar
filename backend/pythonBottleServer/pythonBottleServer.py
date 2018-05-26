#   
#    Python REST API service
#    

from bottle import route, run, request

@route('/api/v1', method='POST')
def index():
    for l in request.body:
        print(l)
    #print(request.body.readlines())

#run(host='192.168.12.57', port=8888, debug=True)
run(host='localhost', port=8888, debug=True)

