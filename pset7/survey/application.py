import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


def check_and_add_missing_values(missing_fields_list, field, field_description):
    if not field:
        missing_fields_list.append(field_description)
    return missing_fields_list


@app.route("/form", methods=["POST"])
def post_form():
    missing_fields = []

    first_name = request.form.get("first_name")
    missing_fields = check_and_add_missing_values(missing_fields, first_name, "first name")

    last_name = request.form.get("last_name")
    missing_fields = check_and_add_missing_values(missing_fields, last_name, "last name")

    fruit = request.form.get("fruit")
    missing_fields = check_and_add_missing_values(missing_fields, fruit, "fruit")

    seat = request.form.get("seat")
    missing_fields = check_and_add_missing_values(missing_fields, seat, "seat")

    if missing_fields:
        return render_template("error.html", missing_fields=missing_fields)

    # If all fields are here write them to database and send user to the table of entries.
    with open("survey.csv", "a") as file:
        writer = csv.writer(file)
        writer.writerow((first_name, last_name, fruit, seat))

    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    with open('survey.csv', 'r') as f:
        reader = csv.reader(f)
        table_rows = list(reader)

    return render_template("sheet.html", table_rows=table_rows)
