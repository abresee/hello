from django.conf.urls import patterns, url

from project_view import views

urlpatterns = patterns('',
	url(r'^$', views.index, name='index')
)
