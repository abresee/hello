import re
from math import floor

from django.shortcuts import render, redirect
from django.db import models
from project_view.forms import RegisterForm
from project_view.models import Project, Track, EventWindow, Note
from django.contrib.auth.models import User
from django.contrib.auth import logout
from django.template import Context, loader, Template, RequestContext
from django.http import HttpResponse

def index(request, usernames, project_name):
    project_list=[]
    track_list=[]
    p = Project.objects.get(name=project_name)
    template = loader.get_template('project_view/index.html')
    if request.user.is_authenticated():
        for project in request.user.project_set.all().filter(name=project_name):
            project_list.append(project)
        if len(project_list) > 0:
            tracks = Track.objects.filter(project=p)
            for track in tracks:
                track_list.append(track)
            context = RequestContext(request, {"username": usernames, "project": project_name, "track_number": track_list, "event_window": p.event_window_set.all(), "notes": p.note_set.all()})
            return HttpResponse(template.render(context))
        else:
            return redirect('http://127.0.0.1:8000/profile/')   
    else:
        return redirect('http://127.0.0.1:8000/login/')
     
def register(request):
    template = loader.get_template('registration/register.html')
    try:
        if request.method == 'POST':
            if request.POST.get('password') == request.POST.get('confirm_password'):
                user = User.objects.create_user(request.POST.get('username'),'none', request.POST.get('password'))
                return redirect('http://127.0.0.1:8000/login/')
            else:
                return render(request, 'registration/register.html')
        else:
            return render(request, 'registration/register.html')
    except:
        context = RequestContext(request, {"error": "Username already exists!"})
        return HttpResponse(template.render(context))
        
def profile(request, usernames):
    template = loader.get_template('accounts/profile.html')
    if request.user.is_authenticated() and (request.user.username == usernames or usernames=='profile'):
        context = RequestContext(request, {"username": request.user, "projects": request.user.project_set.all()})
        return HttpResponse(template.render(context))
    else:
        return redirect('http://127.0.0.1:8000/login/')
    
def projectz_save(request, usernames, project_name):
    if request.is_ajax():
        text=request.POST.getlist('track[]')
        for i in text:
            print(i)
        return HttpResponse(text)
    else:
        flag = True
        projects = request.user.project_set.all()
        for project in projects:
            if project.name == request.POST.get('projectz_name'):
                flag = False

        p = Project.objects.get(name=project_name)
        new_name = request.POST.get('projectz_name')
        if flag:
            project.name = new_name
            project.save()

            template = loader.get_template('project_view/index.html')
            context = RequestContext(request, {"username": usernames, "project": new_name})
            return HttpResponse(template.render(context))
        else:
            template = loader.get_template('project_view/index.html')
            context = RequestContext(request, {"username": usernames, "project": project_name, "error": "You already have a project with this name!"})
            return HttpResponse(template.render(context))
        
def create_project(request):
    flag = True
    projects = request.user.project_set.all()
    for project in projects:
        if project.name == request.POST.get('project_name'):
            flag = False
    
    if flag:
        p = Project(name=request.POST.get('project_name'))
        p.save()
        p.owners.add(request.user)
        return redirect('http://127.0.0.1:8000/' + request.user.username)
    else:
        template = loader.get_template('accounts/profile.html')
        context = RequestContext(request, {"username": request.user, "projects": request.user.project_set.all(), "error": "Project names must be unique! Please choose a different name."})
        return HttpResponse(template.render(context))
    
def project_logout(request):
    if request.user.is_authenticated():
        logout(request)
        return redirect('http://127.0.0.1:8000/login/')
    else:
        return render(request, 'project_view/index.html')
        
def track_creation(request, project_name, usernames):
    if request.is_ajax():
        number_of_tracks=str(int(request.POST['track']) - 1)
        p = Project.objects.get(name=project_name)
        t = Track(track_number=number_of_tracks, project=p)
        t.save()
        track_number = t.track_number
        return HttpResponse(track_number)
        
def track_deletion(request, project_name, usernames):
    track_number_list = []
    remaining_windows_list = []
    if request.is_ajax():
        tracks_deleted= request.POST.getlist('track_number[]')
        remaining_windows = request.POST.getlist('window_positions[]')
        
        for i in remaining_windows:
            remaining_windows_list.append(i)
        print(remaining_windows_list)
        
        p = Project.objects.get(name=project_name)
        for i in tracks_deleted:
            t = Track.objects.get(track_number=i, project=p)
            t.delete()
            e = EventWindow.objects.filter(track=i, project=p)
            for ee in e:
                ee.delete()
            
        tracks = Track.objects.filter(project=p)
        for track in tracks:
            track_number_list.append(track.track_number)
        
        count = 1
        for track_numberz in track_number_list:
            tt = Track.objects.get(track_number = track_numberz, project=p)
            tt.track_number = count
            tt.save()
            count += 1
        
        e = EventWindow.objects.filter(project=p)
        count = 0
        for ee in e:
            ee.position_top = int(remaining_windows_list[count])
            ee.save()
            count += 1
            
        return HttpResponse(tracks_deleted)
        
def event_container_creation(request, project_name, usernames):
    if request.is_ajax():
        p = Project.objects.get(name=project_name)
        t = Track.objects.get(track_number=request.POST['track'], project=p)
        e = EventWindow(position_left=request.POST['position_left'], position_top=request.POST['position_top'], window_length=request.POST['length'], id_number=request.POST['id'], track=t, project=p)
        e.save()
        return HttpResponse('event_window create saved')
    
def event_container_dragstop(request, project_name, usernames):
    if request.is_ajax():
        p = Project.objects.get(name=project_name)
        t = Track.objects.get(track_number=request.POST['track'], project=p)
        e = EventWindow.objects.get(id_number=request.POST['id'], project=p)
        e.track = t
        e.position_left = request.POST['position_left']
        e.position_top = request.POST['position_top']
        e.save()
        
        print("HAHAHAHAHAHA", request.POST)
        position_list = []
        positions = request.POST.getlist('note_position[]')
        for position in positions:
            position_list.append(position)
        
        count = 0
        notes = e.note_set.all()
        for note in notes:
            note.position = int(position_list[count])
            note.save()
            count += 1
        
        return HttpResponse('drag_stop saved')
        
def event_container_resizestop(request, project_name, usernames):
    if request.is_ajax():
        p = Project.objects.get(name=project_name)
        t = Track.objects.get(track_number=request.POST['track'], project=p)
        e = EventWindow.objects.get(project=p, id_number=request.POST['id'])
        e.window_length = request.POST['length']
        e.save()
        return HttpResponse('window resizestop saved')
        
def event_window_delete(request, project_name, usernames):
    if request.is_ajax():
        p = Project.objects.get(name=project_name)
        e = EventWindow.objects.get(project=p, id_number=request.POST['id'])
        e.delete()
        return HttpResponse('window delete saved')
        
def note_creation(request, project_name, usernames):
    if request.is_ajax():
        print(request.POST)
        p = Project.objects.get(name=project_name, owners=request.user)
        match = re.search(r'container_num_(?P<window_number>[0-9]+$)', request.POST['window'])
        e = EventWindow.objects.get(project=p, id_number=int(match.group('window_number')))
        n = Note(pitch_class=request.POST['pitch_class'], octave=request.POST['octave'], intensity=0, position=request.POST['position'], duration=request.POST['duration'], id_number=request.POST['id'], event_window=e, project=p)
        n.save()
        return HttpResponse('note creation saved')

def note_dragstop(request, project_name, usernames):
    if request.is_ajax():
        print(request.POST)
        
        pitch_class_list = []
        octave_list = []
        
        p = Project.objects.get(name=project_name, owners=request.user)
        ids = request.POST.getlist('id[]')
        pos_lefts = request.POST.getlist('position_left[]')
        pos_tops = request.POST.getlist('position_top[]')
        for pos_top in pos_tops:
            pitch_class_list.append(floor((int(pos_top) / 25) % 12))
            octave_list.append(floor(int(pos_top) / (25 * 12)))
        
        count = 0
        for id in ids:
            match = re.search(r'id_num_(?P<note_id>[0-9]+$)', id)
            n = Note.objects.get(project=p, id_number=int(match.group('note_id')))
            n.pitch_class = pitch_class_list[count]
            n.octave = octave_list[count]
            n.position = pos_lefts[count]
            n.save()
            count += 1
        return HttpResponse('note drag saved')
        
def note_resize(request, project_name, usernames):
    if request.is_ajax():
        print(request.POST)  
        note_widths = []
        p = Project.objects.get(name=project_name, owners=request.user)
        ids = request.POST.getlist('id[]')
        widths = request.POST.getlist('width[]')
        for width in widths:
            note_widths.append(int(width))
        
        count = 0
        for id in ids:
            match = re.search(r'id_num_(?P<note_id>[0-9]+$)', id)
            n = Note.objects.get(project=p, id_number=int(match.group('note_id')))
            n.duration = note_widths[count]
            n.save()
            count += 1
        return HttpResponse('note resize saved')

def note_delete(request, project_name, usernames):
    if request.is_ajax():
        print(request.POST)  
        note_widths = []
        p = Project.objects.get(name=project_name, owners=request.user)
        ids = request.POST.getlist('id[]')
        
        count = 0
        for id in ids:
            match = re.search(r'id_num_(?P<note_id>[0-9]+$)', id)
            n = Note.objects.get(project=p, id_number=int(match.group('note_id')))
            n.delete()
            count += 1
        return HttpResponse('note delete saved')
    
        
        
    
        
        
