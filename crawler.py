import requests as rq
from bs4 import BeautifulSoup as bs
import sqlite3 as sql


uniport = "http://www.uniprot.org/uniprot/"
search = "http://www.uniprot.org/uniprot/?query=insecticidal+NOT+crystal+AND+reviewed%3Ayes&sort=length&desc=no"
cookies = {'limit' : 250,}
succeed = None
while not succeed:
    try:
        search_result = rq.get(search, cookies)
    except:
        pass
    else:
        succeed = True
else:
    search_result.close()

search_soup = bs(search_result.text, "lxml")
result_list = [ID.a.text for ID in search_soup.find_all(class_="entry")]


def get_uni_title(soup):
    title = soup.find("div", id="entry-overview").find_all(class_="entry-overview-content")
    protein = title[0].string
    organism = title[2].string
    return protein, organism

def get_feature(soup):
    pep_table = soup.find("table", class_="featureTable", id="peptides_section")
    aam_table = soup.find("table", class_="featureTable", id="aaMod_section")
    sig_pep = False
    propep = False
    disu_bond = 0
    if pep_table:
        if pep_table.find_all(string="Signal peptide"):
            sig_pep = True
        if pep_table.find_all(string="Propeptide"):
            propep = True

    if aam_table:
        disu_bond = len(aam_table.find_all(string="Disulfide bond"))

    return sig_pep, propep, disu_bond

def get_uni_func(soup):
    tag_list = soup.find("div", class_="annotation",
              property="schema:hasPart",
              typeof="schema:creativeWork").span.contents
    func = "".join([tag.string for tag in tag_list if tag.name!='span'])
    return func
def get_uni_AClen(soup):
    length = soup.find("div", class_="sequence-isoform-rightcol").div.span.next_sibling.string
    return length

def link_to_ara(soup):
    url = None
    ara_id = ""
    dblinker = soup.find('table', class_="databaseTable ORGANISM")
    if dblinker:
        if dblinker.span.span.next_sibling == "ArachnoServer":
            url = dblinker.a['href']
            ara_id = dblinker.a.string

    return url, ara_id

def ara_target(soup):
    ctable = soup.find("table", cellpadding="0",cellspacing="0",style="width: 750px",id="cardtable")
    target = []
    if ctable and ctable.find_all(string="Molecular Target"):
        table = ctable.find_all("tr")[1:]
        for row in table:
            target.append(row.td.text)

    return target

def ara_taxon(soup):
    ctable = soup.find("table", cellpadding="0",cellspacing="0",style="width: 650px",id="cardtable")
    tags = []
    info = {}
    if ctable:
        table = ctable.find_all("tr")
        for tag in table[0].find_all("th"):
            info[tag.string] = []
            tags.append(tag.string)

        for row in table[1:]:
            for tag, content in zip(tags, row.find_all("td")):
                info[tag].append(''.join([i for i in content.string if i.isprintable()]))

    return info

def ara_seq(soup):
    source = soup.find("td", class_="carddatafull", style="font-family: courier new").strings
    seq = "".join([s for s in source])
    return seq

def get_info(ID):
    info = {
        "uni-protein":"" ,
        "uni-organism":"" ,
        "uni-function":"" ,
        "uni-amino-acid-len":"",
        "ara-id":"",
        "uni-signal-peptide":False,
        "uni-propetide":False,
        "uni-disulfide-bond":0,
        }
    http = uniport + ID
    succeed = None
    while not succeed:
        try:
            response = rq.get(http)
        except:
            pass
        else:
            succeed = True
    else:
        response.close()

    html = response.text
    soup = bs(html, "lxml")
    info['uni-protein'], info['uni-organism'] = get_uni_title(soup)
    info['uni-function'] = get_uni_func(soup)
    info['uni-amino-acid-len'] = get_uni_AClen(soup)
    info["uni-signal-peptide"], info["uni-propetide"], info["uni-disulfide-bond"] = get_feature(soup)

    ara_url, info['ara-id'] = link_to_ara(soup)
    if ara_url:
        ara_succeed = None
        while not ara_succeed:
            try:
                ara_response = rq.get(ara_url)
            except:
                pass
            else:
                ara_succeed = True
        else:
            ara_response.close()
        
        ara_html = ara_response.text
        ara_soup = bs(ara_html, "lxml")
        taxon = ara_taxon(ara_soup)
        for key in taxon.keys():
            info["ara-"+key] = taxon[key]
        
        target = ara_target(ara_soup)
        if target:
            info["ara_target"] = target

        seq = ara_seq(ara_soup)
        info["ara_sequence"] = seq
        for tag in info.keys():
            if type(info[tag]) == list:
                info[tag] = ",".join(info[tag])

        
    return info

def try_info(ID):
    try:
        get_info(ID)
    except Exception as e:
        print(ID, e)

if __name__ == '__main__':
    TABLE = "'ara-PD50', 'uni-amino-acid-len', 'ara-LD50', 'uni-organism', 'ara-Qualitative Information','uni-propetide', 'ara_sequence', 'uni-function', 'uni-signal-peptide', 'ara-Taxon', 'uni-protein','ara_target', 'ara-ED50', 'uni-disulfide-bond', 'ara-id'"
    mydb = sql.connect("crawler.db")
    c = mydb.cursor()
    c.execute('create table mydb ('+TABLE+')') 

    def info2db(info):
        try:
            ctitle = []
            ccontent = []
            for item in info.items():
                ctitle.append(str(item[0]))
                ccontent.append(str(item[1]))
    
            title = '("'+'", "'.join(ctitle)+'")'
            content = '("'+'", "'.join(ccontent)+'")'
            c.execute('insert into mydb ' + title + ' values ' + content)
            mydb.commit()
        except Exception as e:
            print(title, content, e)

    for ID in result_list:
        info = get_info(ID)
        info2db(info)


    mydb.close()
