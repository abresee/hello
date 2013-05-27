from django.utils import simplejson
from dajaxice.decorators import dajaxice_register
from project_view.models import Project, Track
from django.http import HttpResponse

@dajaxice_register
def sayhello(request):
    return simplejson.dumps({'message':'Hello World'})

@dajaxice_register	
def track_save(request):
	p = Track()
	p.track_number = 5
	p.save()
	return HttpResponse()