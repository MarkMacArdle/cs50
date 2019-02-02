import os
import sys
import time
import inspect

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get users current stocks
    users_stocks = db.execute('''SELECT symbol, SUM(shares) AS total_shares
                              FROM transactions
                              WHERE id = :user_id AND symbol != "Cash Deposit"
                              GROUP BY symbol
                              HAVING total_shares > 0
                              ORDER BY symbol''', user_id=session.get("user_id"))

    # Get users current cash balance
    user_cash = db.execute("SELECT cash FROM users WHERE id=:user_id",
                           user_id=session.get("user_id"))[0]["cash"]

    grand_total = user_cash
    for row in users_stocks:
        quote = lookup(row["symbol"])
        row["name"] = quote["name"]
        row["price"] = usd(quote["price"])
        row["total"] = usd(quote["price"] * row["total_shares"])
        grand_total += quote["price"] * row["total_shares"]

    return render_template("index.html",
                           table_type="Portfolio",
                           table_rows=users_stocks,
                           user_cash=usd(user_cash),
                           grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        try_quote = get_quote(request.form, inspect.stack()[0][3])
        if try_quote[0]:
            quote = try_quote[1]
            quantity = try_quote[2]
        else:
            return apology(try_quote[1])

        # Check user has enough money
        user_cur_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session.get("user_id"))[0]["cash"]
        user_new_cash = user_cur_cash - (quote["price"] * quantity)
        if user_new_cash < 0:
            return apology("not enough funds")

        # Update database tables
        process_transaction(session.get("user_id"),
                            quote["symbol"],
                            quantity,
                            quote["price"],
                            user_new_cash)

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("buy_or_sell.html", buy_or_sell="buy")


@app.route("/add_funds", methods=["GET", "POST"])
@login_required
def add_funds():
    '''Allow user add cash to account'''

    if request.method == "POST":
        if not request.form.get("amount"):
            return apology("must provide amount")

        try:
            amount = float(request.form.get("amount"))
        except ValueError:
            return apology("must provide valid number")

        if amount <= 0:
            return apology("must provide positive number")

        user_cur_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session.get("user_id"))[0]["cash"]

        process_transaction(session.get("user_id"),
                            "Cash Deposit",
                            1,  # Recording this as buying 1 share
                            amount,  # Share price is same as purchase amount
                            user_cur_cash + amount)

        return redirect("/")
    else:
        return render_template("add_funds.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available and 1 character or more, else false, in JSON format"""

    if len(request.args.get("username")) > 0 and username_available(request.args.get("username")):
        return jsonify(True)
    else:
        return jsonify(False)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get users history
    users_transactions = db.execute('''SELECT symbol, shares, price, transaction_time
                                    FROM transactions
                                    WHERE id = :user_id
                                    ORDER BY transaction_time DESC''',
                                    user_id=session.get("user_id"))

    for row in users_transactions:
        if row["symbol"] == "Cash Deposit":
            row["name"] = "N/A"
            row["price"] = usd(row["price"])
        else:
            quote = lookup(row["symbol"])
            row["name"] = quote["name"]
            row["price"] = usd(quote["price"])

    return render_template("index.html",
                           table_type="History",
                           table_rows=users_transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
        try_quote = get_quote(request.form, inspect.stack()[0][3])
        if try_quote[0]:
            quote = try_quote[1]
        else:
            return apology(try_quote[1])

        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure no blanks in form data and that passwords matched
        if not request.form.get("username"):
            return apology("must provide username")
        elif not request.form.get("password"):
            return apology("must provide password")
        elif not request.form.get("confirmation"):
            return apology("must confirm password")
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords didn't match")

        # Check if username already in database
        if not username_available(request.form.get("username")):
            return apology("username already taken")

        # Add new user to database
        # ID auto increments
        db.execute('INSERT INTO "users" ("id","username","hash") VALUES (NULL, :username, :hashed_pword)',
                   username=request.form.get("username"),
                   hashed_pword=generate_password_hash(request.form.get("password")))

        # Query database to get users id
        rows = db.execute("SELECT id FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        try_quote = get_quote(request.form, inspect.stack()[0][3])
        if try_quote[0]:
            quote = try_quote[1]
            quantity = try_quote[2]
        else:
            return apology(try_quote[1])

        # Check user has enough stocks to sell
        users_shares_of_stock = db.execute("SELECT SUM(shares) AS total_shares FROM transactions WHERE id = :user_id AND symbol = :symbol",
                                           user_id=session.get("user_id"),
                                           symbol=quote["symbol"])[0]["total_shares"]
        if quantity > users_shares_of_stock:
            return apology(f"not enough shares to sell, you only have {users_shares_of_stock}")

        # Get new cash position
        user_cur_cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                                   user_id=session.get("user_id"))[0]["cash"]
        user_new_cash = user_cur_cash + (quote["price"] * quantity)

        # Update database tables
        process_transaction(session.get("user_id"),
                            quote["symbol"],
                            -quantity,
                            quote["price"],
                            user_new_cash)

        # Redirect user to home page
        return redirect("/")

    else:
        # Get users current stocks
        users_stocks = db.execute('''SELECT symbol
                                     FROM transactions
                                     WHERE id = :user_id AND symbol != "Cash Deposit"
                                     GROUP BY symbol
                                     HAVING SUM(shares) > 0
                                     ORDER BY symbol''', user_id=session.get("user_id"))

        return render_template("buy_or_sell.html", buy_or_sell="sell", users_stocks=users_stocks)


def get_quote(form, calling_func_name):
    '''Returns true if form is okay, otherwise the apology string'''

    if not form.get("symbol"):
        return False, "must provide stock symbol"

    # Try getting quote for stock
    quote = lookup(form.get("symbol"))
    if not quote:
        return False, f'unable to get a quote for {form.get("symbol")}'

    if calling_func_name == "quote":
        return True, quote
    else:  # For buy and sell functions
        if not form.get("shares"):
            return False, "must provide shares amount"
        if not form.get("shares").isdigit() or form.get("shares") == "0":
            return False, "shares must be positive whole number"

        quantity = int(form.get("shares"))

        return True, quote, quantity


def process_transaction(user_id, symbol, quantity, price, user_new_cash):
    # Record transaction
    db.execute('''INSERT INTO transactions (id, symbol, shares, transaction_time, price)
                  VALUES (:user_id, :symbol, :quantity, :transaction_time, :price)''',
               user_id=user_id,
               symbol=symbol,
               quantity=quantity,
               transaction_time=time.strftime('%Y-%m-%d %H:%M:%S'),
               price=price)

    # Update users cash amount
    db.execute("UPDATE users SET cash = :user_new_cash WHERE id = :user_id",
               user_new_cash=user_new_cash,
               user_id=user_id)

    return


def username_available(username):
    '''Return true if username is available, false if not'''
    return len(db.execute("SELECT username FROM users WHERE username = :username", username=username)) == 0


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
