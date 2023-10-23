from flask import Blueprint, render_template, request, flash, jsonify
from flask_login import login_required, current_user
from twilio.rest import Client
views = Blueprint('views', __name__)


@views.route('/')
@login_required
def home():
    return render_template("home.html", user=current_user)

@views.route('/main-page')
@login_required
def main_page():
    return render_template("main_page.html", user=current_user)

@views.route('/send-sms', methods=['GET', 'POST'])
@login_required
def sms():
    if request.method == 'POST':
        phone_number = request.form.get('phone_number')
        message = "Help! I don't feel safe" 

        # Initialize Twilio client with your Account SID and Auth Token
        account_sid = 'ACc4cc395fadb3626dd58cdf01d4c38f3c'
        auth_token = 'd7b4df52435c42b39672e31a6c068622'
        client = Client(account_sid, auth_token)

        try:
            # Send SMS
            client.messages.create(
                body=message,
                from_='+15105879524',
                to=phone_number
            )
            flash('SMS sent successfully!', category='success')
        except Exception as e:
            flash(f'Error: {str(e)}', category='error')

    return render_template("sms.html", user=current_user)
