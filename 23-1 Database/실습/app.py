import psycopg2
from flask import Flask, render_template, request

app = Flask(__name__)
connect = psycopg2.connect("dbname=tutorial user=postgres password=0000")
cur = connect.cursor()  # create cursor


@app.route('/')
def main():
    return render_template("main.html")


@app.route('/return', methods=['post'])
def re_turn():
    return render_template("main.html")


@app.route('/print_table', methods=['post'])
def print_table():
    cur.execute("SELECT * FROM users;")
    result = cur.fetchall()

    return render_template("print_table.html", users=result)


@app.route('/register', methods=['get', 'post'])
def register():
    if request.method == "GET":
        print("!@#!@#!@#!@#")
    id = request.form["id"]
    password = request.form["password"]
    send = request.form["send"]

    if send == 'sign up':
        try:
            cur.execute(f"INSERT INTO users values(%s, %s)", (id, password))
            connect.commit()
            return render_template("login_success.html")
        except psycopg2.IntegrityError as e:
            print(e)
            connect.rollback()
            return render_template("ID_collision.html")

    elif send == 'login':
        try:
            cur.execute(f"SELECT id, password FROM users WHERE id=(%s) and password=(%s)", (id, password))
            user = cur.fetchone()
        except psycopg2.Error as e:
            print(e)
            connect.rollback()
            return render_template("login_fail.html")
        
        print(user)
        if not user:
            return render_template("login_fail.html")
        
        return render_template("login_success.html")

    return id + " " + password + " " + send


if __name__ == '__main__':
    app.run()
