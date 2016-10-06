import crawler as cr

#f = open("input.csv", "r+")
#ID_list = [ Id[:-1] for Id in f ][1:]


def get_info(ID):
    info = {
        "entry":"",
        "Protein_name":"" ,
        "signal_peptide":False,
        "propetide":False,
        "Protein_sequence_Uni":"",
        "Chain":"",
        }
    http = cr.uniport + ID
    succeed = None
    while not succeed:
        try:
            response = cr.rq.get(http)
        except:
            pass
        else:
            succeed = True
    else:
        response.close()

    html = response.text
    soup = cr.bs(html, "lxml")
    info["entry"] = ID
    info["Protein_name"] = cr.get_uni_title(soup)[0]
    info["signal_peptide"], info["propetide"], _  = cr.get_feature(soup)
    info["Protein_sequence_Uni"] = cr.get_uni_seq(http)
    
    if info["signal_peptide"] or info["propetide"]:
        info["Chain"] = cr.blast_query(ID , *cr.blast_info(soup))
    
    return info


if __name__ == '__main__':
    cr.monkey.patch_all()
    TABLE = "'entry', 'Chain', 'Protein_sequence_Uni','propetide', 'signal_peptide', 'Protein_name'"
    mydb = cr.sql.connect("new_crawler.db")
    c = mydb.cursor()
    c.execute('create table mydb2 ('+TABLE+')') 

    def info2db(info):
        try:
            ctitle = []
            ccontent = []
            for item in info.items():
                ctitle.append(str(item[0]))
                ccontent.append(str(item[1]))
            
            title = '("'+'", "'.join(ctitle)+'")'
            content = '("'+'", "'.join(ccontent)+'")'
            c.execute('insert into mydb2 ' + title + ' values ' + content)
            mydb.commit()
        except Exception as e:
            print(title, content, e)
    
    def work(ID):
        info2db(get_info(ID))

    
    jobs = [cr.gevent.spawn(work, ID) for ID in cr.result_list ]
    cr.gevent.joinall(jobs)
    
    '''
    for ID in ID_list:
        work(ID)
    '''    

    c.close()
    mydb.close()
