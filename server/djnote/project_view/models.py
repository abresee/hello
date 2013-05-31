from django.db import models
from django.contrib.auth.models import User

class Project(models.Model):
	name = models.CharField(max_length=40)
	ownerz = models.ManyToManyField(User, related_name='user_projects')

class Event_Window(models.Model):
	window_position = models.IntegerField()
	window_length = models.IntegerField()
	window_track = models.IntegerField()

class Track(models.Model):
	track_number = models.IntegerField()
	project = models.ForeignKey('Project', related_name="project_name")

	
