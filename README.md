# cs50
Harvard's intro to computer science course.

# Excercises completed

## Pset1
1. [Hello](https://docs.cs50.net/2018/x/psets/1/hello/hello.html): print "hello, world."
2. [Mario (more comfortable)](https://docs.cs50.net/2018/x/psets/1/mario/more/mario.html): print hashtags in shapes that look like the pyramids in Mario.
3. [Credit](https://docs.cs50.net/2018/x/psets/1/credit/credit.html): check if given card numbers are valid.


## Pset2
1. [Caesar](https://docs.cs50.net/2018/x/psets/2/caesar/caesar.html): encoded a string with a Caesar cipher (shift letters by a given amount).
2. [Crack](https://docs.cs50.net/2018/x/psets/2/crack/crack.html): crack hashed passwords up to 5 characters long.


## Pset3 (2018)
1. [Music](https://docs.cs50.net/2018/x/psets/3/music/music.html) (from 2018 course): modify a file distribution to add functionality to output the frequency of musical notes (eg A4 = 440Hz).


##### Note at this point the course was updated from its 2018 to 2019 versions. The test scores for the above Psets were carried over, except for Music which isn't on the course anymore.


## Pset3 (2019)
1. [Whodunit](https://lab.cs50.io/cs50/labs/2019/x/whodunit/): modify the bytes of a bitmap image file to change the colour of red pixels to reveal a message hidden in the noise.
2. [Resize (more comfortable)](https://docs.cs50.net/2019/x/psets/3/resize/more/resize.html): modify the headers and pixels of a bitmap file to increase or decrease its size.
3. [Recover](https://docs.cs50.net/2019/x/psets/3/recover/recover.html): find 50 JPEG files on a memory card by searching for the starting bytes of JPEG images.


## Pset4
1. [Speller (trie implementation)](https://docs.cs50.net/2019/x/psets/4/speller/trie/speller.html): store the words of a dictionary in a trie and then search the trie to see if passed words are spelt right. Also created functions to recursively count the words in the trie and recursively free the memory used by it at the end.


## Pset5
1. [Homepage](https://docs.cs50.net/2019/x/psets/5/homepage/homepage.html): create a small website with 4 pages. (Please don't judge me on this, I thoroughly half arsed it and just met the minimum requirements.)


## Pset6
1. [Hello](https://docs.cs50.net/2019/x/psets/6/sentimental/hello/hello.html): redo Hello from pset1 in python.
2. [Mario (more comfortable)](https://docs.cs50.net/2019/x/psets/6/sentimental/mario/more/mario.html): redo Mario from pset1 in python.
3. [Credit](https://docs.cs50.net/2019/x/psets/6/sentimental/credit/credit.html): redo Credit from pset1 in python.
4. [Crack](https://docs.cs50.net/2019/x/psets/6/sentimental/crack/crack.html): redo Crack from pset2 in python.
5. [Bleep](https://docs.cs50.net/2019/x/psets/6/bleep/bleep.html): read in a text file of banned words then censor those words with \*s if they appear in a users input string.


## Pset7
1. [Similarities](https://docs.cs50.net/2019/x/psets/7/similarities/similarities.html): create a form that could accept file inputs from user and write functions for splitting the text in them into lines, sentences or ngrams.
2. [Survey](https://docs.cs50.net/2019/x/psets/7/survey/survey.html): With Flask for Python make a site that can accept user inputs from a validated form, save them on the server in a csv then show the csv contents to the user in a table.

## Pset8
1. [Finance](https://docs.cs50.net/2019/x/psets/8/finance/finance.html): build out a website with Flask, Bootstrap and SQLite that allows users sign up, log in and then buy or sell stocks on multiple forms and webpages.

## Final Project
Stock market gamification. [Repo here](https://github.com/MarkMacArdle/stock_game) and [playable version here](http://ec2co-ecsel-5zub5659xyt4-1820632623.us-east-1.elb.amazonaws.com:5000/).

Built off the pset8 to make a game from the stock market data pulled from IEX's api. Logos from companies on the Nasdaq 100 move with the stock price movements from the last full trading day. You play a little spaceman trying to jump up these stocks and sit on rising stocks to make money.

The game front end was made with the Phaser 3 Javascript library and it runs on a Flask server. I ran that in an Alpine Linux Docker container and hosted it on AWS ECS. The highscores are kept in a MongoDB on Mongo Atlas.
