import sqlite3 as sql

TABLE = "'Protein_name', 'Molecular_target', 'Function', 'Taxon', 'ED50', 'LD50', 'PD50', 'Qualitative_Information', 'aa_length', 'number_of_disulfide_bonds', 'propeptide', 'signal_peptide', 'Protein_sequence_Uni', 'Protein_sequence_Ara', 'Chain', 'Organism', 'Uniprot_entry', 'Arachnoserver_id'"
final = sql.connect("crawler-v.2-de-space.db")
db2 = sql.connect("new_crawler.db")
c = final.cursor()
cur2 = db2.cursor()
g = cur2.execute("select entry, Chain from mydb2")

def update(entry, chain):
    c.execute('update mydb set Chain = "' + chain + '" where Uniprot_entry = "' + entry + '"')
    final.commit()

for i in g:
    update(*i)

    
c.close()
cur2.close()
db2.close()
final.close()
