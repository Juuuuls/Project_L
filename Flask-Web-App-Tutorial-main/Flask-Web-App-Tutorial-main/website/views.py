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
        message = request.form.get('message')

        # Initialize Twilio client with your Account SID and Auth Token
        account_sid = 'your_account_sid'
        auth_token = 'your_auth_token'
        client = Client(account_sid, auth_token)

        try:
            # Send SMS
            client.messages.create(
                body=message,
                from_='your_twilio_phone_number',
                to=phone_number
            )
            flash('SMS sent successfully!', category='success')
        except Exception as e:
            flash(f'Error: {str(e)}', category='error')

    return render_template("sms.html", user=current_user)