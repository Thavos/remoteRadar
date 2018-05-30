from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
@app.route('/index')
def homepage():
    return render_template("index.html")

@app.route('/problems')
def problems ():
    return render_template("problems.html")

@app.route('/contact')
def contact ():
    return render_template("contact.html")

if __name__== '__main__':
app.run(port=8000)

