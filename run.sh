#!/bin/bash

#   -f      [sisendtekst utf8-s] 
#   -o      [väljund-wav] 
#   -lex    [analüüsi sõnastik] 
#   -lexd   [ühestaja sõnastik] 
#   -m      [hääle nimi, vt kataloogi htsvoices/]
#   -r      [kõnetempo, double, 0.01-2.76] 

bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_eva1100.wav -f in.txt -m htsvoices/eki_et_eva1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_lee1100.wav -f in.txt -m htsvoices/eki_et_lee1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_lsi1100.wav -f in.txt -m htsvoices/eki_et_lsi1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_luu1100.wav -f in.txt -m htsvoices/eki_et_luu1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_lvk1100.wav -f in.txt -m htsvoices/eki_et_lvk1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_mls1101.wav -f in.txt -m htsvoices/eki_et_mls1101.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_rna1100.wav -f in.txt -m htsvoices/eki_et_rna1100.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_tnu1100.wav -f in.txt -m htsvoices/eki_et_tnu1100.htsvoice -r 1.1

