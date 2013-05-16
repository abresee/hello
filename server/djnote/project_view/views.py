# Create your views here.
#from django.http import HttpResponse
#from django.template import loader, Context
from django.shortcuts import render

def index(request):
	#template = loader.get_template('project_view/indexx.html')
	#return HttpResponse(template.render)
	return render(request, 'project_view/index.html')
