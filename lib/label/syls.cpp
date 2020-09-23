#include "../etana/proof.h"
#include "util.h"


//bool print_syls = false;

struct pattern_struct {
    CFSWString mall;
    INTPTR pos;
};
const INTPTR patterns_size = 18;

// tel-k:i ja kõi-k:e kõlavad halvasti (esisilbi rõhk ja pikk Q teises tasandavad üksteist ära)
// filsime selle rõhutamise käigus, topeldades laat-t:a ja kõik-k:e

pattern_struct patterns[] = {
    L"V",	1, // ja:

    L"VC",	2, // viga FS sõnastiku pärisnimedega (Ant<onio, Nikol<ajeva, päris mitu)

    L"VLh",	3, // 3	monAR-H:i, monARH:
    L"VLQ",	3, // 3	kAR-T:a, kURT: (aga kUR:D)
    L"VCC",	2, // 2	kUL:M, kAS:T, kAL:-Du, jÕh:v

//    L"VLss",	3, // 3	mARS:S
//    L"VLsQ",	3, // 3	kUNS:T (ja kORS:T-Na)
//    L"VLsC",	3, // 3	vELS:-Ker
//    L"VLhv",	3, // 3	vURH:V
//    L"VLhC",	3, // 3	vURH:-Vi
//    L"VLQL",	3, // 3	pILT:-Lik
//    L"VLQC",	3, // 3	lONK:S (ja kANT:-SLer, vINT:SK-Lema)
//    L"VLQQ",	3, // 3	oli lisandusena (pILK:-Lik)

    L"VCCC",	2, // 2	tEK:ST, kAP:S-Lid (ja EK:ST-Ra, gÄN:GST-Rid)

    L"VV",	2, // 2	vÖÖ:
    L"VVV",	2, // 2	[sõi] sAI:A
    L"VVQ",	3, // 3	lAA-T:a, lAAT:
    L"VVC",	2, // 2	vEE:L, krOO:-Ni
    L"VVss",	3, // 3	pOIS:-Se, pOIS:S
    L"VVsQ",	3, // 3	lAA:ST, lAAS:-Tu, sÄÄS:T-Lik	!!! VVsQ# oli 2
    L"VVQs",	3, // 3	lOOT:S, rOOT:S-Lane

    L"VVQL",	3, // 3	kAAT:-Ri
    L"VVQQ",	3, // 3	oli lisandusena, näiteks fOOG:T
    L"VVLQ",	4, // 4	hUULT:, kAAR-T:i (ja kAART:-Lane)
    L"VVCC",	2, // 2	kEE:LD, ja pAA:VST, kEE:L-Du, jUU:RD-Lus, pAA:VST-Lus
    L"VVVC",	2, // 2	lAI:-US
};

Printer PP;

bool can_palat(CFSWString c) {
    if (c.FindOneOf(L"DLNST") > -1) return true;
    return false;
}

INTPTR pattern_lookup(CFSWString s) {
    // üldistus: kõik neljased reeglid v.a VLCL (k<indla, j<ärgmine), mis algavad VL-iga on 3, s.h lONKS
    INTPTR res = 3;
    if (s.GetLength() == 4 && s.Left(2) == L"VL") {
        if (s.GetAt(2) == L'C') return res-1;
        return res;
    }

    res = -1;
    for (INTPTR i = 0; i < patterns_size; i++) {
        if (s == patterns[i].mall) return patterns[i].pos;
    }
    return res;
}

CFSWString tapne_mall(CFSWString s) {
    // varasema shift_pattern asemel

    CFSWString res;
    for (INTPTR i = 0; i < s.GetLength(); i++) {
        CFSWString c = s.GetAt(i);
        if
            (c.FindOneOf(L"jhv") > -1) { res += c; }
        else
            if (c.FindOneOf(L"sS") > -1) res += L's';
        else
            if (c.FindOneOf(L"lmnrLN") > -1) res += L'L';
        else
            if (c.FindOneOf(L"kptfšT") > -1) res += L'Q';
        else
            if (is_consonant(c)) res += L'C';
        else
            // esimene peab, teine võib olla V. pikemad vokaaljärjendid (n<eiu, v<iia jms) on malliga VV
            if (is_vowel(s)) {
                if (i < 2) res += L'V';
                else break;
            }
        else
            // lühem kui 4 või muu vigane jama (<i<i, <i?i vms)
            break;
    }
    return res;
}

CFSWString yldine_mall(CFSWString s) {
    // varasema simplity_pattern asemel (muutusteta)

    CFSWString res;
    for (INTPTR i = 0; i < (s.GetLength()); i++) {
        CFSWString c = s.GetAt(i);
        if (c.FindOneOf(L"jhvsLQ") > -1)
            res += L"C";
        else
            res += c;
    }
    return res;
}


CFSWString sona_foneetiliseks(CFSWString &s) {
    // asendame võõrtähed, soovi korral ć jms, aga vabamorfist tulnud märke ei puudu:
    // ] (palataliseeri), ? (rõhuta) ja < (vältesta) jäävad
    // etapiti, sest varem ei saanud kirjutada näiteks "lic]", sest võrreldi c-d, mitte ts-i

    CFSWString res;
    for (INTPTR i = 0; i < s.GetLength(); i++) {
        CFSWString c = s.GetAt(i);
        if
            ((c == L'<') || (c == L'?') || (c == L']')) res += c;
        else
            // Exxon -> ekson, XXIII -> ksiii (aga ega ksksiii pole parem)
            if ((c == L'x') && (s.GetAt(i + 1) != L'x')) res += L"ks";
        else
            if (c == L'y') res += L"i";
        else
            if (c == L'w') res += L"v";
        else
            // pizza -> pitsa, piza -> pisa
            if (c == L'z') {
                if (s.GetAt(i + 1) == L'z') res += L"t";
                else res += L"s";
            }
        else
            if (c == L'c') {
                // sõltub järgnevast vokaalist, < ei piisa (vrd Wis<consin, A le C<oq jms)
                CFSWString cc = s.GetAt(i + 1);
                if (cc == L'<' || cc == L'?') cc = s.GetAt(i + 2);
                if (cc == L'e' || cc == L'i') {
                    res += L"ts";
                } else
                    res += L"k";
            }
        else
            // müüa -> müia
            if (c == L'ü' && is_vowel(s.GetAt(i + 1)) && s.GetAt(i - 1) == L'ü') res += L"i";
        else
            if (c == L'q') {
                // kv kontroll karmimaks kui enne (lihtsalt vokaal oleks viga Quran -> kvran ja Coqi -> kokv puhul)
                res += L"k";
                if ((s.GetAt(i + 1) == L'u') && is_vowel(s.GetAt(i + 2))) s.SetAt(i + 1, L'v');
        }
        else
            // kustutame kõik muud mittetähed, koos sellega kaob vajadus kutsuda make_char_string
            if (is_char(c)) res += c;
    }
    return res;
}

CFSWString sona_palataliseeri(CFSWString s) {
    // asendame ] konsonandi järel suurtäheliseks ja kaotame ]
    // sama teeme ümbrusega

    CFSWString res;
    INTPTR pos;
    for (INTPTR i = 0; i < s.GetLength(); i++) {
        CFSWString c = s.GetAt(i);
        if (c == L']') {
            // palataliseerime temakese ja selle ette jäävad
            pos = res.GetLength() - 1;
            while (pos >= 0) {
                CFSWString t = CFSWString(res.GetAt(pos)).ToUpper();
                if (can_palat(t)) {
                    res.SetAt(pos, t.GetAt(0));
                    pos--;
                } else break;
            }
            // palataliseerime järgnevad
            pos = i + 1;
            while (pos < s.GetLength()) {
                CFSWString t = CFSWString(s.GetAt(pos)).ToUpper();
                if (can_palat(t)) {
                    s.SetAt(pos, t.GetAt(0));
                    pos++;
                } else break;
            }
        }
        else res += c;
    }
    return res;
}

CFSWString sona_valtesta(CFSWString s) {
    // asendame < vokaali ees :-ga pikendatava hääliku järel

    CFSWString res;
    CFSWString mall;
    INTPTR pos = -1; // sisuliselt boolean

    for (INTPTR i = 0; i < (s.GetLength()); i++) {
        // arvutame uue pos-i
        CFSWString c = s.GetAt(i);
        if (c == L'<') {
//            if (print_syls) fprintf(stderr, "sona: %s\n", ccstr(s));
            // malli pikkus on max 4 märki. edasised ? ja < ei koti, need lõpetavad malli niikuinii
            mall = s.Mid (i+1, 4);
            mall = tapne_mall (mall);
            pos = pattern_lookup(mall);
            // kui ükski spetsiifilisem mall ei klappinud, siis proovime leida V+C+ vaikimisi reeglit
            if (pos < 0) {
                mall = yldine_mall (mall);
                pos = pattern_lookup(mall);
if (pos < 0) {
    fprintf(stderr, "\n###\n\n%s <", ccstr(mall));
    mall = tapne_mall (mall);
    fprintf(stderr, " %s < %s\n\n###\n", ccstr(mall), ccstr(s));
}
            }
            pos = i + pos;
        }
        res += c;
        if (i == pos) res += colon; // t<ikt<ak?
    }
    return res;
}


CFSWString sona_silbita(CFSWString s) {
    // lisame kriipse silbivahedesse (NB < ja ? on endiselt sees ja jäägu, lisaks ka :)

    bool ij = false;
    CFSWString res = s.GetAt(0);
    // hi: alustame ühest, esimese tähe ette niikuinii silbipiiri ei tule
    for (INTPTR i = 1; i < s.GetLength(); i++) {
        CFSWString ees = s.GetAt(i-1);
        if (ees == L':') ees = s.GetAt(i-2);
        CFSWString c = s.GetAt(i);
        CFSWString taga = s.GetAt(i+1);
        if (taga == L':') taga = s.GetAt(i+2);

        if (is_consonant(c)) {
            // ka-na, aka-d<eemia, ra-k?eti
            if (is_vowel(ees) && (is_vowel(taga) || taga == L'<' || taga == '?'))
                res += d;
            // tup-rub, ap-l<omb, aga mitte p-raak
            if ((is_consonant(ees) && (is_vowel(taga) || taga == L'<' || taga == '?')) && has_vowel(res))
                res += d;
        }
        // siia peaks ehk diftongireegleid lisama
        // teeme praegu kolmesed nii, et V-VV siis kui tagumised on samad, muidu VV-V
        else if (is_vowel(ees) && is_vowel(c) && is_vowel(taga)) {
            if (c.ToLower() == taga && ij == false) {
                res += d; // i-ooni, i-Oon, mei-jOos (ij)
            }
            else if (ees.ToLower() == L'ü' && c == L'ü') {
                res += L"i-j"; // püüe
                ij = true;
                continue;
            }
            else if (c == L'i') {
                res += L"i-j"; // saia, sAia
                ij = true;
                continue;
            }
        }
        else if (is_vowel(ees) && is_vowel(c)) ij = false;
        else if ((c == L'<' || c == L'?') && is_vowel(ees))
            res += d;
        res += c;
    }
    return res;
}

bool is_stressed_syl(CFSWString syl) {
    bool res = false;
    for (INTPTR i = 0; i < syl.GetLength(); i++) {
        if ((syl.GetAt(i) == L'<') || (syl.GetAt(i) == L'?') || ((is_vowel(syl.GetAt(i))) && (is_vowel(syl.GetAt(i + 1)))))
            res = true;
    }
    return res;
}




void AddStress(CFSClassArray<TSyl> &sv, INTPTR wp) {
    /* Kõige radikaalsem rõhutus siiani. 
     * wp = kui on liitsõna esimene liige siis on seal pearõhk.
     */
    INTPTR main_stress = 2;
    INTPTR stress = 1;
    // pearõhk saab olla ainult esimesel osisel liitsõnas
    if (wp > 0) main_stress = 1;
    INTPTR size = sv.GetSize();
	
	//p.prni(main_stress);
    
	bool main_applied = false;

    // ühesilbiline
    if (size == 1) {
        sv[0].Stress = main_stress;
        sv[0].Syl.Remove(L'<');
    }
    else {
        // paneme varasema teadmise järgi algväärtused
        // teadmine on silpidesse jäänud < ja ? ja pika vokaali/diftongiga

        // pearõhk on seal, kus ? (ühtlasi koristame)
        for (INTPTR i = 0; i < size; i++) {

            // eksperimendi korras topeldame pikeneva, aga järgmisesse silpi sattunud laa-t:a -> laat-t:ta
            if (i > 0 && sv[i].Syl.GetAt(1) == L':') {
                CFSWString c = sv[i].Syl.GetAt(0);
                sv[i-1].Syl += c;
            }

            if (sv[i].Syl.Find('?') > -1) {
                sv[i].Stress = main_stress;
                main_applied = true;
                sv[i].Syl.Remove(L'?');
            }
        }

        // või siis seal, kus < (ühtlasi koristame)
        for (INTPTR i = 0; i < size; i++) {
            if (sv[i].Stress > 0) continue; // tsivilisatsi?ooni on is_stressed ja saab muidu uue rõhu
            if (is_stressed_syl(sv[i].Syl)) {
                sv[i].Syl.Remove(L'<');
                if (main_applied) {
                    sv[i].Stress = stress;
                }
                else {
                    sv[i].Stress = main_stress;
                    main_applied = true;
                }
            }
        }

        // või siis esisilbil, kui polnud ühtki nähtavalt rõhulist
        if (main_applied == false)
            sv[0].Stress = main_stress;

        // kui sõna algusesse jäi rõhutamata silpe, siis rõhutame paarituid
        for (INTPTR i = 0; i < size-1; i += 2)
            if ((sv[i].Stress == 0) && (sv[i+1].Stress == 0)) {
                sv[i].Stress = stress;
            }
            else break;

        // täidame augud (kolm järjestikust rõhutut silpi)
        for (INTPTR i = 0; i < size-2; i++)
            if ((sv[i].Stress == 0) && (sv[i+1].Stress == 0) && (sv[i+2].Stress == 0)) {
                sv[i+1].Stress = stress;
            }
    }
}



CFSWString word_to_syls(CFSWString word) {
    CFSWString s;

    // varasema chars_to_phones_part_I asendus;
    s = sona_foneetiliseks(word);
    s = sona_palataliseeri(s);

    // varasema the_shift asendus
    s = sona_valtesta(s);

    s = sona_silbita(s);
    return s;
}


void TUtterance::DoSyls(TWord& TW) {
//PP.prnn(TW.TWMInfo.m_szRoot);


    CFSArray<CFSWString> temp_arr, c_words;
	CFSClassArray<TSyl> TSA;
	
	//Kuna siin tulevad DLNST märgentitena siis:
	TW.TWMInfo.m_szRoot = TW.TWMInfo.m_szRoot.ToLower();
	
	explode(TW.TWMInfo.m_szRoot, L"_", c_words);
	CFSWString s;
	for (INTPTR cw = 0; cw < c_words.GetSize(); cw++) {
		s = word_to_syls(c_words[cw]);
		explode(s, d, temp_arr);
		for (INTPTR i = 0; i < temp_arr.GetSize(); i++) {
			TSyl T;
			T.Syl = temp_arr[i];
			T.Stress = 0;
			//T.DoPhones(T);
			TW.TSA.AddItem(T);
			
		}
		AddStress(TW.TSA, cw);
	}
	
	TW.e2 = TW.TSA.GetSize();
	// Välde on ikka silbi, mitte foneemi omadus :)
	
	for (INTPTR i = 0; i < TW.e2; i++) {
		if (TW.TSA[i].Syl.Find(L":", 0) > -1) {
			TW.TSA[i].DoQ  = 1;
			TW.TSA[i].Syl.Remove(L':');
			
		}
		PP.prn(TW.TSA[i].Syl);
                PP.prni(TW.TSA[i].Stress);
                PP.prni(TW.TSA[i].DoQ);
                PP.prnn();
		TW.TSA[i].DoPhones(TW.TSA[i]);
	}

}

// 


