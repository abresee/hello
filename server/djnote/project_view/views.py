# Create your views here.
#from django.http import HttpResponse
#from django.template import loader, Context
from django.shortcuts import render, redirect
from django.forms.models import model_to_dict
from project_view.forms import RegisterForm
from project_view.models import Project
from django.contrib.auth.models import User
from django.template import loader, Template, RequestContext
from django.http import HttpResponse

def index(request):
	#template = loader.get_template('project_view/indexx.html')
	#return HttpResponse(template.render)
	return render(request, 'project_view/index.html')
	 
def register(request):
	if request.method == 'POST':
		#form = RegisterForm
		if request.POST.get('password') == request.POST.get('confirm_password'):
			user = User.objects.create_user(request.POST.get('username'),'none', request.POST.get('password'))
			return render(request, 'accounts/profile.html')
		else:
			return render(request, 'registration/register.html')
	else:
		return render(request, 'registration/register.html')
		
def profile(request):
	project_name_list = []
	username = request.user.username
	
	template = loader.get_template('accounts/profile.html')
	
	for object in Project.objects.all():
		project_name_list.append(object.name)
	context = RequestContext(request, {"username": username, "project_name": project_name_list})
	
	return HttpResponse(template.render(context))
	
def projectz_save(request):
	try: 
		p = Project(name=request.POST.get('projectz_name'))
		p.save()
		return redirect('http://127.0.0.1:8000/project_view/')
	except:
		return redirect('http://www.google.com/')
	
