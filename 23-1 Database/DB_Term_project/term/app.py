import psycopg2
from flask import Flask, render_template, request, redirect, url_for, session, abort
app = Flask(__name__)
connect = psycopg2.connect("dbname=term user=postgres password=0000")
cur = connect.cursor()  # create cursor

@app.route("/")
def index():
    user_id = session.get('user_id', None)
    print(user_id)
    if user_id is None:
        return redirect(url_for('login'))
    else:
        cur.execute("SELECT id, credit, rating, role, welcome_coupon FROM account WHERE id=(%s)", (user_id,))
        account = cur.fetchone()

        cur.execute("SELECT code, name, price, tutor FROM lecture")
        lectures = cur.fetchall()

        cur.execute("SELECT tutor FROM enrollment GROUP BY tutor ORDER BY count(tutor) DESC LIMIT 1")
        popular_tutor = cur.fetchone()[0]
        cur.execute("SELECT lecture_name FROM enrollment GROUP BY lecture_name ORDER BY count(lecture_name) DESC LIMIT 1")
        popular_lecture = cur.fetchone()[0]
        cur.execute("SELECT subject_name FROM subject WHERE code = (SELECT code FROM enrollment GROUP BY code ORDER BY count(code) DESC LIMIT 1)")
        popular_subject = cur.fetchone()[0]

        popular_info = {
            "subject": popular_subject,
            "lecture": popular_lecture,
            "tutor": popular_tutor
        }

        return render_template('index.html', popular_info=popular_info, account=account, lectures=lectures)

"""
Authentication
"""
@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    elif request.method == 'POST':
        id = request.form["id"]
        password = request.form["password"]
        send = request.form["send"]

        try:
            cur.execute(f"SELECT id, password FROM users WHERE id=(%s) and password=(%s)", (id, password))
            user = cur.fetchone()
        except psycopg2.Error as e:
            print(e)
            connect.rollback()
            return render_template("login_fail.html")

        if not user:
            return render_template("login_fail.html")
        
        print(user)
        session['user_id'] = user[0]
        return redirect(url_for("index"))

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == 'GET':
        return render_template('signup.html')
    elif request.method == 'POST':
        id = request.form["id"]
        password = request.form["password"]
        role = request.form["role"]

    try:
        cur.execute("INSERT INTO users VALUES (%s, %s)", (id, password))
        cur.execute("INSERT INTO account(id, credit, rating, role) VALUES (%s, %s, %s, %s)",
                    (id, 10000, 'welcome', role))
        connect.commit()
        return render_template("signup_success.html")
    except psycopg2.IntegrityError as e:
        print(e)
        connect.rollback()
        return render_template("signup_fail.html")

@app.route("/logout")
def logout():
    session.clear()
    return redirect(url_for("index"))


@app.route("/user", methods=["GET"])
def get_users():
    if session.get("user_id", None) != "admin":
        abort(403)
    
    cur.execute("SELECT id, password FROM users")
    users = cur.fetchall()

    return render_template("print_users.html", users=users)

@app.route("/enrollment", methods=["GET", "POST"])
def enrollment():
    if request.method == "GET":
        if session.get("user_id", None) != "admin":
            abort(403)
        
        cur.execute("SELECT tutee, tutor, code, lecture_name, lecture_price FROM enrollment")
        enrollments = cur.fetchall()

        return render_template("print_enrollments.html", enrollments=enrollments)
    
    elif request.method == "POST":
        tutee = request.form["tutee"]
        tutor = request.form["tutor"]
        code = request.form["code"]
        name = request.form["name"]
        price = int(request.form["price"])
        discount = int(request.form["discount"])
        discounted_price = price - discount

        if tutee == tutor:
            error_message = "You cannot take the lecture you teach."
            return render_template("enroll_fail.html", error_message=error_message)

        cur.execute("SELECT id, credit, rating, role FROM account WHERE id=(%s)", (tutee,))
        tutee_account = cur.fetchone()
        cur.execute("SELECT id, credit, rating, role FROM account WHERE id=(%s)", (tutor,))
        tutor_account = cur.fetchone()

        if "welcome_coupon" not in request.form and tutee_account[1] < discounted_price:
            error_message = "Not enough credit."
            return render_template("enroll_fail.html", error_message=error_message)
        
        cur.execute("SELECT tutee, tutor, code, lecture_name, lecture_price FROM enrollment \
                     WHERE tutee=(%s) AND tutor=(%s) AND code=(%s) AND lecture_name=(%s) and lecture_price=(%s)", (tutee, tutor, code, name, price))
        duplicate_enrollment = cur.fetchone()

        if duplicate_enrollment is not None:
            error_message = "Duplicate enrollment"
            return render_template("enroll_fail.html", error_message=error_message)

        try:
            # Update credit
            cur.execute("INSERT INTO enrollment VALUES (%s, %s, %s, %s, %s)", (tutee, tutor, code, name, price))
            tutee_credit = tutee_account[1] - discounted_price if "welcome_coupon" not in request.form else tutee_account[1]
            print(f"tutee_credit: {tutee_credit}")
            cur.execute("UPDATE account SET credit=(%s) WHERE id=(%s)", (tutee_credit, tutee_account[0]))
            tutor_credit = tutor_account[1] + price
            cur.execute("UPDATE account SET credit=(%s) WHERE id=(%s)", (tutor_credit, tutor_account[0]))

            # Update ratings
            cur.execute("SELECT rating, condition, discount FROM rating_info ORDER BY condition DESC")
            rating_conditions = cur.fetchall()
            for rating, condition, discount in rating_conditions:
                if tutee_credit >= condition:
                    cur.execute("UPDATE account SET rating=(%s) WHERE id=(%s)", (rating, tutee_account[0]))
                    break
            for rating, condition, discount in rating_conditions:
                if tutor_credit >= condition:
                    cur.execute("UPDATE account SET rating=(%s) WHERE id=(%s)", (rating, tutor_account[0]))
                    break
            
            # Update welcome coupon
            cur.execute("UPDATE account SET welcome_coupon=false WHERE id=(%s)", (tutee_account[0],))

            # cur.execute("UPDATE rati")
            connect.commit()
            return redirect(url_for("my_enrollment"))
        except psycopg2.Error as e:
            print(e)
            connect.rollback()
            error_message = "Database error"
            return render_template("enroll_fail.html", error_message=error_message)

@app.route("/enrollment/my", methods=["GET"])
def my_enrollment():
    user_id = session.get("user_id")
    cur.execute("SELECT tutee, tutor, subject_name, lecture_name, lecture_price FROM enrollment NATURAL JOIN subject WHERE tutee = (%s)", (user_id, ))
    enrollments = cur.fetchall()

    return render_template("my_enrollment.html", enrollments=enrollments)

"""
Lecture
"""
@app.route("/lecture/my", methods=["GET"])
def my_lecture():
    user_id = session.get("user_id", None)
    if user_id is None:
        abort(404)

    cur.execute("SELECT id, credit, rating, role FROM account WHERE id=(%s)", (user_id,))
    account = cur.fetchone()
    
    if account[3] == 'tutor':
        cur.execute("SELECT subject_name, lecture_name, tutee, lecture_price FROM enrollment NATURAL JOIN subject WHERE tutor=(%s)", (account[0],))
        my_lectures = cur.fetchall()

        cur.execute("SELECT subject_name, lecture_name, tutor, lecture_price FROM enrollment NATURAL JOIN subject WHERE tutee=(%s)", (account[0],))
        registered_lectures = cur.fetchall()

        return render_template("my_lecture.html", my_lectures=my_lectures, registered_lectures=registered_lectures, role=account[3])
    else:
        cur.execute("SELECT subject_name, lecture_name, tutor, lecture_price FROM enrollment NATURAL JOIN subject WHERE tutee=(%s)", (account[0],))
        my_lectures = cur.fetchall()

        return render_template("my_lecture.html", my_lectures=my_lectures, role=account[3])

@app.route("/lecture/new", methods=["GET"])
def lecture_form():
    user_id = session.get("user_id", None)
    if user_id is None:
        abort(403)
    
    cur.execute("SELECT code, subject_name FROM subject")
    subjects = cur.fetchall()
    return render_template("lecture_form.html", subjects=subjects)

@app.route("/lecture", methods=["POST"])
def add_lecture():
    code = request.form["code"]
    name = request.form["name"]
    price = request.form["price"]
    
    user_id = session.get("user_id")
    try:
        cur.execute("INSERT INTO lecture VALUES (%s, %s, %s, %s)", (code, name, price, user_id))
        connect.commit()
    except psycopg2.Error as e:
        print(e)
        connect.rollback()
        return render_template("add_lecture_fail.html")
    
    return redirect(url_for("index"))

@app.route("/lecture/register", methods=["POST"])
def register_lecture():
    lecture = request.form.to_dict()

    user_id = session.get("user_id")
    cur.execute("SELECT id, credit, rating, role, welcome_coupon FROM account WHERE id=(%s)", (user_id,))
    account = cur.fetchone()

    cur.execute("SELECT rating, condition, discount FROM rating_info WHERE rating=(%s)", (account[2],))
    rating_info = cur.fetchone()
    discount_rate = rating_info[2]
    
    lecture["price"] = int(lecture["price"])
    discount = int(lecture["price"] * (discount_rate / 100))
    return render_template("register_lecture.html", lecture=lecture, account=account, discount=discount)

@app.route("/credit/gift", methods=["GET", "POST"])
def credit_gift():
    if request.method == "GET":
        return render_template("credit_gift.html")
    else:
        reciever_id = request.form["reciever_id"]
        credit = int(request.form["credit"])
        sender_id = session.get("user_id")

        cur.execute("SELECT id, credit FROM account WHERE id=(%s)", (reciever_id,))
        reciever_account = cur.fetchone()
        if reciever_account is None or credit < 0 or sender_id == reciever_id:
            return render_template("credit_gift_fail.html")
        
        cur.execute("SELECT id, credit FROM account WHERE id=(%s)", (sender_id,))
        sender_account = cur.fetchone()
        
        try:
            sender_credit = sender_account[1] - credit
            reciever_credit = reciever_account[1] + credit
            # Update credit
            cur.execute("UPDATE account SET credit=(%s) WHERE id=(%s)", (sender_credit, sender_id))
            cur.execute("UPDATE account SET credit=(%s) WHERE id=(%s)", (reciever_credit, reciever_id))
            
            # Update ratings
            cur.execute("SELECT rating, condition, discount FROM rating_info ORDER BY condition DESC")
            rating_conditions = cur.fetchall()
            for rating, condition, discount in rating_conditions:
                if sender_credit >= condition:
                    cur.execute("UPDATE account SET rating=(%s) WHERE id=(%s)", (rating, sender_account[0]))
                    break
            for rating, condition, discount in rating_conditions:
                if reciever_credit >= condition:
                    cur.execute("UPDATE account SET rating=(%s) WHERE id=(%s)", (rating, reciever_account[0]))
                    break
                
            connect.commit()
            return render_template("credit_gift_success.html")
        except psycopg2.Error as e:
            print(e)
            connect.rollback()
            return render_template("credit_gift_fail.html")

if __name__ == '__main__':
    app.config['SECRET_KEY'] = '2023COSE371'

    app.run(host='127.0.0.1', port=5000, debug=True)
