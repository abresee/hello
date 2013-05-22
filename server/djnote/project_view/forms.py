from django import forms

class RegisterForm(forms.Form):
	username = forms.CharField(max_length=15, min_length=5)
	password = forms.CharField(max_length=15, min_length=5)
	password_confirm = forms.CharField(max_length=15, min_length=5)
	