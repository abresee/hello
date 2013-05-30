from django.conf.urls import patterns, url
from project_view import views



urlpatterns = patterns('',
	url(r'^(?P<project_name>[\w\s]+)/$', views.index, name='index'),
	url(r'^(?P<project_name>[\w\s]+)/save$', views.projectz_save, name='project_save'),	
)
