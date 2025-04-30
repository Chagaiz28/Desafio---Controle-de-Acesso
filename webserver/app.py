from flask import Flask, render_template
import sqlite3
import os

# Determine the absolute path to the templates directory
template_dir = os.path.abspath("./template")

app = Flask(__name__, template_folder=template_dir)
DB_PATH = "/home/henrique/meus-projetos/projeto/Desafio---Controle-de-Acesso/access_control.db"

def get_events():
    print("🔍 get_events() function called")  # Add this line
    try:
        with sqlite3.connect(DB_PATH) as conn:
            print("🔍 Database connection established")  # Add this line
            cursor = conn.cursor()
            cursor.execute("SELECT id, user_name, door_id, timestamp FROM events ORDER BY timestamp DESC;")
            events = cursor.fetchall()
            print("🔍 Eventos encontrados:", events)
            return events
    except sqlite3.Error as e:
        print(f"Database error: {e}")
        return []
    finally:
        print("🔍 get_events() function finished") # Add this line

@app.route("/")
def index():
    return render_template("events.html")

@app.route("/events")
def events():
    return render_template("events.html", events=get_events())

if __name__ == "__main__":
    app.run(debug=True)