from django.db import models
from django.contrib.auth.models import User

class Project(models.Model):
    name = models.CharField(max_length=40)
    ownerz = models.ManyToManyField(User)

class Event_Window(models.Model):
    position_left = models.IntegerField()
    position_top = models.IntegerField()
    window_length = models.IntegerField()
    id_number = models.IntegerField()
    project = models.ForeignKey('Project')
    track = models.ForeignKey('Track')
    
   
class Track(models.Model):
    track_number = models.IntegerField()
    project = models.ForeignKey('Project')
    
class Note(models.Model):
    pitch_class = models.IntegerField()
    octave = models.IntegerField()
    intensity = models.IntegerField()
    position = models.IntegerField()
    duration = models.IntegerField()
    id_number = models.IntegerField()
    event_window = models.ForeignKey('Event_Window')
    project = models.ForeignKey('Project')

    