from django.conf.urls import patterns, url
from project_view import views

urlpatterns = patterns('',
	url(r'^$', views.index, name='index'),
	url(r'^register$', views.register, name='register'),
	url(r'^save$', views.projectz_save, name='project_save'),
)
