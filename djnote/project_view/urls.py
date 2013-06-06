from django.conf.urls import patterns, url
from project_view import views



urlpatterns = patterns('',
	url(r'^(?P<project_name>[\w\s]+)/$', views.index, name='index'),
	url(r'^(?P<project_name>[\w\s]+)/create_track/$', views.track_creation, name='track_creation'),
	url(r'^(?P<project_name>[\w\s]+)/delete_track/$', views.track_deletion, name='track_deletion'),
    url(r'^(?P<project_name>[\w\s]+)/create_event_window/$', views.event_container_creation, name='event_window'),
    url(r'^(?P<project_name>[\w\s]+)/event_window_dragstop/$', views.event_container_dragstop, name='event_window_dragstop'),
    url(r'^(?P<project_name>[\w\s]+)/event_window_resizestop/$', views.event_container_resizestop, name='event_window_resizestop'),
    url(r'^(?P<project_name>[\w\s]+)/event_window_delete/$', views.event_window_delete, name='event_window_delete'),
    url(r'^(?P<project_name>[\w\s]+)/create_note/$', views.note_creation, name='note_creation'),
	url(r'^(?P<project_name>[\w\s]+)/save/$', views.projectz_save, name='project_save'),		
)
