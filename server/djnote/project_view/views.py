import re

from django.shortcuts import render, redirect
from django.db import models
from project_view.forms import RegisterForm
from project_view.models import Project, Track
from django.contrib.auth.models import User
from django.contrib.auth import logout
from django.template import Context, loader, Template, RequestContext
from django.http import HttpResponse

def index(request, usernames, project_name):
	project_list=[]
	template = loader.get_template('project_view/index.html')
	if request.user.is_authenticated():
		for project in request.user.user_projects.all().filter(name=project_name):
			project_list.append(project)
		if len(project_list) > 0:
			context = RequestContext(request, {"username": usernames, "project": project_name})
			return HttpResponse(template.render(context))
		else:
			return redirect('http://127.0.0.1:8000/profile/')	
	else:
		return redirect('http://127.0.0.1:8000/login/')
	 
def register(request):
	if request.method == 'POST':
		if request.POST.get('password') == request.POST.get('confirm_password'):
			user = User.objects.create_user(request.POST.get('username'),'none', request.POST.get('password'))
			return redirect('http://127.0.0.1:8000/login/')
		else:
			return render(request, 'registration/register.html')
	else:
		return render(request, 'registration/register.html')
		
def profile(request, usernames):
	template = loader.get_template('accounts/profile.html')
	if request.user.is_authenticated() and (request.user.username == usernames or usernames=='profile'):
		context = RequestContext(request, {"username": request.user, "projects": request.user.user_projects.all()})
		return HttpResponse(template.render(context))
	else:
		return redirect('http://127.0.0.1:8000/login/')
	
def projectz_save(request, usernames, project_name):
	new_name = request.POST.get('projectz_name')
	p = Project.objects.filter(name=project_name)
	for project in p:
		project.name = new_name
		project.save()
	template = loader.get_template('project_view/index.html')
	context = RequestContext(request, {"username": usernames, "project": new_name})
	return HttpResponse(template.render(context))
	
def create_project(request):
	try:
		t = Track(track_number = 0)
		t.save()
		p = Project(name=request.POST.get('project_name'), tracks=t)
		p.save()
		p.ownerz.add(request.user)
		return redirect('http://127.0.0.1:8000/' + request.user.username)
	except:
		template = loader.get_template('accounts/profile.html')
		context = RequestContext(request, {"username": request.user, "projects": request.user.user_projects.all(), "error": "Project names must be unique! Please choose a different name."})
		return HttpResponse(template.render(context))
	
def project_logout(request):
	if request.user.is_authenticated():
		logout(request)
		return redirect('http://127.0.0.1:8000/login/')
	else:
		return render(request, 'project_view/index.html')
		