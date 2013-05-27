import re

from django.shortcuts import render, redirect
from project_view.forms import RegisterForm
from project_view.models import Project, Track
from django.contrib.auth.models import User
from django.contrib.auth import logout
from django.template import Context, loader, Template, RequestContext
from django.http import HttpResponse

def index(request):
	if request.user.is_authenticated():
		return render(request, 'project_view/index.html')
	else:
		return redirect('http://127.0.0.1:8000/login/')
	 
def register(request):
	if request.method == 'POST':
		if request.POST.get('password') == request.POST.get('confirm_password'):
			user = User.objects.create_user(request.POST.get('username'),'none', request.POST.get('password'))
			return render(request, 'accounts/profile.html')
		else:
			return render(request, 'registration/register.html')
	else:
		return render(request, 'registration/register.html')
		
def profile(request):
	project_list = []
	template = loader.get_template('accounts/profile.html')
	if request.user.is_authenticated():
		projects = request.user.user_projects.all()
		for project in projects:
			project_list.append(project)
		context = RequestContext(request, {"username": request.user, "projects": request.user.user_projects.all()})
		return HttpResponse(template.render(context))
	else:
		return redirect('http://127.0.0.1:8000/login/')
	
def projectz_save(request):
	match = re.search(r'[A-Za-z0-9]', request.POST.get('projectz_name'))
	if match:
		t = Track(track_number = int(request.POST.get('trackss')))
		t.save()
		p = Project(name=request.POST.get('projectz_name'), tracks=t)
		p.save()
	return render(request, 'project_view/index.html')	
	
def project_logout(request):
	if request.user.is_authenticated():
		logout(request)
		return redirect('http://127.0.0.1:8000/login/')
	else:
		return render(request, 'project_view/index.html')
		