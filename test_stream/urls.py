from django.conf.urls import patterns, url

urlpatterns = patterns('test_stream.views',
        url(r'^$', 'index'),
        url(r'^is_ready/$', 'is_ready')
)
