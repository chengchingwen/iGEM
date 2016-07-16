import requests
from bs4 import BeautifulSoup

#parsering sequences from uniprot.org
with open("input.txt", "r+") as ifile, open("output.txt", "w+") as ofile:
    for http in ifile:
        html = requests.get(http).text
        soup = BeautifulSoup(html, "html.parser")
        for tex in soup.pre:
            if tex.string:
                if str(tex.string)[0]!=' ':
                    ofile.write(tex.string)
        ofile.write("\n")


