import re

from django.conf.urls import patterns, include, url
from django.contrib import admin
admin.autodiscover()
from dajaxice.core import dajaxice_autodiscover, dajaxice_config
dajaxice_autodiscover()
from django.contrib.staticfiles.urls import staticfiles_urlpatterns

urlpatterns = patterns('',

    url(r'^admin/', include(admin.site.urls)),
	url(r'^register/$', 'project_view.views.register'),
	url(r'^login/$', 'django.contrib.auth.views.login'),
	url(r'^logout$', 'project_view.views.project_logout'),
	url(r'^create_project$', 'project_view.views.create_project'),
	url(r'^(?P<usernames>\w+)/$', 'project_view.views.profile'),
	url(r'^(?P<usernames>\w+)/', include('project_view.urls')),
	url(dajaxice_config.dajaxice_url, include('dajaxice.urls')),

)

urlpatterns += staticfiles_urlpatterns()
