from django.conf.urls import patterns, include, url
from django.contrib import admin

admin.autodiscover()

urlpatterns = patterns('',
	url(r'^admin/', include(admin.site.urls)),
	url(r'^register/$', 'project_view.views.register'),
	url(r'^login/$', 'django.contrib.auth.views.login'),
	url(r'^logout/$', 'project_view.views.project_logout'),
	url(r'^create_project/$', 'project_view.views.create_project'),
	url(r'^(?P<usernames>\w+)$', 'project_view.views.profile'),
	url(r'^(?P<usernames>\w+)/', include('project_view.urls')),
    url(r'^test_stream/', include('test_stream.urls')),
)
