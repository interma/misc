import sys
import bs4 #BeautifulSoup4

html_file = sys.argv[1]
data = open(html_file).read()
soup = bs4.BeautifulSoup(data, "html.parser")

#intro
path = "div.im-description"
items = soup.select(path)
print items[0].text.strip()

#license
path = "span.lic"
items = soup.select(path)
print items[0].text.strip()

#homepage
path = "td > a"
items = soup.select(path)
print items[0]["href"].strip()

#download page
path = "a.vbtn"
items = soup.select(path)
print items[0]["href"].strip()

