from django.db import models
from django.contrib.auth.models import User
from django.core.exceptions import ValidationError
import core
import re

class Project(models.Model):
    name = models.CharField(max_length=40)
    owners = models.ManyToManyField(User)

class EventWindow(models.Model):
    position_left = models.IntegerField()
    position_top = models.IntegerField()
    window_length = models.IntegerField()
    id_number = models.IntegerField()
    project = models.ForeignKey('Project')
    track = models.ForeignKey('Track')
   
class Track(models.Model):
    track_number = models.IntegerField()
    project = models.ForeignKey('Project')

class BeatField(models.CharField, metaclass=models.SubfieldBase):

    description = "A rational valued duration in 'beat' units. Useful for time points and time intervals" 

    def to_python(self,value):
        print(value)
        if isinstance(value, core.Beat):
            return value
        pattern = r'^(\d+),(\d+)$'
        m = re.match(pattern,value)
        if not m:
            raise ValidationError('Invalid value: {0}'.format(value))
        return core.Beat(int(m.group(1)),int(m.group(2)))

    def get_prep_value(self,value):
        return "{0},{1}".format(value.num,value.denom)
                

class Note(models.Model):
    pitch_class = models.IntegerField()
    octave = models.IntegerField()
    intensity = models.IntegerField()
    position = BeatField(max_length=200)
    duration = BeatField(max_length=200)
    id_number = models.IntegerField()
    event_window = models.ForeignKey('EventWindow')
    project = models.ForeignKey('Project')
