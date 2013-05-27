from django.conf.urls import patterns, include, url
from django.contrib import admin
admin.autodiscover()
from dajaxice.core import dajaxice_autodiscover, dajaxice_config
dajaxice_autodiscover()
from django.contrib.staticfiles.urls import staticfiles_urlpatterns

urlpatterns = patterns('',

    url(r'^admin/', include(admin.site.urls)),
    url(r'^project_view/', include('project_view.urls')),
	url(r'^login/', 'django.contrib.auth.views.login',),
	url(r'^profile/', 'project_view.views.profile'),
	url(dajaxice_config.dajaxice_url, include('dajaxice.urls')),

)

urlpatterns += staticfiles_urlpatterns()
