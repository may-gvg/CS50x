import datetime
import os
from tempfile import mkdtemp

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


def process_stock(symbol, number):
    row = db.execute("SELECT * from stocks WHERE user_id = ? and symbol = ?", session['user_id'], symbol)
    if row:
        db.execute("UPDATE stocks set number = number + ? WHERE symbol = ? and user_id = ?", number, symbol,
                   session['user_id'])
    else:
        db.execute("INSERT INTO stocks (symbol, number, user_id) values (?, ?, ?)", symbol, number, session['user_id'])


def process_trasaction(operation, total, symbol, number):
    if operation == 'buy':
        operation = 0
    else:
        operation = 1
    db.execute(
        "INSERT INTO transactions (operation, symbol, price, number, datetime, user_id) VALUES (?, ?, ?, ?, ?, ? )",
        operation, symbol, total, number, datetime.datetime.now(), session['user_id'])
    pass


def process_cash(total):
    db.execute("UPDATE users set cash = cash + ? where id = ?", total, session['user_id'])


def is_positive_int(num):
    try:
        i = int(num)
    except Exception as e:
        return False
    s = str(i)
    if s == num:
        if i > 0:
            return True
    return False


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    if request.method == "POST":
        oldpassword = request.form.get("oldpassword")
        password = request.form.get("password")
        password2 = request.form.get("password2")

        rows = db.execute("SELECT * FROM users WHERE id = ?", session['user_id'])

        ret = check_password_hash(rows[0]["hash"], oldpassword)
        if not ret:
            return apology("invalid username and/or password", 403)
        if password != password2:
            return apology("Passwords do nto match")

        hash = generate_password_hash(password)
        db.execute("update users set hash = ? where id = ?", hash, session['user_id'])
        return redirect("/")

    return render_template("password.html")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    table = []
    stockvalue = 0
    rows = db.execute("SELECT * FROM stocks where user_id = ? order by symbol", session['user_id'])
    for row in rows:
        r = {}
        r['symbol'] = row['symbol']
        r['number'] = row['number']
        quote = lookup(row['symbol'])
        price = quote['price']
        r['price'] = price
        value = price * row['number']
        r['value'] = value
        stockvalue += value
        table.append(r)

    rows = db.execute("SELECT * FROM users where id = ?", session['user_id'])
    cash_balance = rows[0]['cash']
    total_value = cash_balance + stockvalue
    flash(f"{usd(cash_balance)} {usd(total_value)}")
    return render_template("index.html", table=table, cash_balance=usd(cash_balance), total_value=usd(total_value))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        number = request.form.get("shares")

        if not symbol:
            return apology("Symbol hot provided")

        if not is_positive_int(number):
            return apology("You need to provide positive number of shares")

        number = int(number)

        quote = lookup(symbol)

        if not quote:
            return apology("Wrong stock provided")

        rows = db.execute("SELECT * from users where id = ?", session["user_id"])
        cash = rows[0]['cash']
        total = quote['price'] * number

        if total > cash:
            return apology("You don't have this much cash in account")

        process_stock(symbol, number)
        process_trasaction("buy", total, symbol, number)
        process_cash(-total)
        flash(usd(quote['price']) + " " + usd(total))
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    table = []
    rows = db.execute("SELECT * FROM transactions where user_id = ? order by id", session['user_id'])
    for row in rows:
        r = {}
        r['symbol'] = row['symbol']
        r['number'] = row['number']
        r['price'] = row['price']
        op = row['operation']
        if op == 1:
            r['operation'] = 'sell'
        else:
            r['operation'] = 'buy'
        r['datetime'] = row['datetime']
        table.append(r)

    return render_template("history.html", table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote:
            quoted = {}
            quoted['name'] = quote['name']
            quoted['price'] = quote['price']
            quoted['symbol'] = quote['symbol']
            return render_template("quoted.html", quoted=quoted)
        else:
            return apology("Symbol does not exist")

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password2 = request.form.get("confirmation")

        if not username or not password or not password2:
            return apology("please fill all fields")

        if password != password2:
            return apology("Passwords do not match")

        row = db.execute("select * from users where username = ?", username)
        print("XXXX")
        print(row)
        if row:
            return apology("Username already exists")

        hash = generate_password_hash(password)
        id = db.execute("insert into users (username, hash) values (?, ?)", username, hash)
        session['user_id'] = id

        return redirect("/")
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        rows = db.execute("SELECT * FROM stocks where user_id = ? order by symbol", session['user_id'])
        stocks = []
        for row in rows:
            stocks.append(row['symbol'])
        return render_template("sell.html", stocks=stocks)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        number = request.form.get("shares")

        if not symbol:
            return apology("Symbol hot provided")

        if not is_positive_int(number):
            return apology("You need to provide positive number of shares")

        number = int(number)

        quote = lookup(symbol)

        if not quote:
            return apology("Wrong stock provided")

        rows = db.execute("SELECT * from stocks where user_id = ?", session["user_id"])
        current_number = rows[0]['number']
        total = quote['price'] * number

        if number > current_number:
            return apology("You don't have this much number of shares")

        process_stock(symbol, -number)
        process_trasaction("sell", -total, symbol, -number)
        process_cash(+total)

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
