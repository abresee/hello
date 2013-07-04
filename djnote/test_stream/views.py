# Create your views here.
from django.http import HttpResponse
from django.template import RequestContext
from django.shortcuts import render_to_response

remember={}

def index(request):
    remember["last"]=request
    return render_to_response("test_stream/test_stream.html",context_instance=RequestContext(request))

def is_ready(request):
    print(remember["last"])
    return HttpResponse("hey hey! ho ho! django development ain't so slow! :)")

