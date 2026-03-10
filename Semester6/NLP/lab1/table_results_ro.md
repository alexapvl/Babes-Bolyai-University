## Romanian Results – Detailed Tables

> **Note on chunking:** Romanian noun phrase "chunks" below are **not** produced by SpaCy's `doc.noun_chunks` (which is not implemented for `ro_core_news_sm`). Instead, they are built with a simple custom heuristic over SpaCy's dependency tree: for each noun or proper noun, the code takes its full dependency subtree (`token.subtree`), sorts the tokens by position, and uses that span as an NP‑like chunk, skipping single‑token spans and avoiding overlaps.

### Sentence 1

**„Ion a mers la bancă să depună bani, apoi s-a așezat pe o bancă în parc.”**

#### Lemmatization

| Token  | Lemma  |
| ------ | ------ |
| Ion    | Ion    |
| a      | avea   |
| mers   | merge  |
| la     | la     |
| bancă  | bancă  |
| să     | să     |
| depună | depune |
| bani   | ban    |
| apoi   | apoi   |
| s-     | sine   |
| a      | avea   |
| așezat | așeza  |
| pe     | pe     |
| o      | un     |
| bancă  | bancă  |
| în     | în     |
| parc   | parc   |

#### POS Tagging

| Token  | POS   | Fine POS        | Explanation |
| ------ | ----- | --------------- | ----------- |
| Ion    | PROPN | Np              |             |
| a      | AUX   | Va--3s          |             |
| mers   | VERB  | Vmp--sm         |             |
| la     | ADP   | Spsa            |             |
| bancă  | NOUN  | Ncfsrn          |             |
| să     | PART  | Qs              |             |
| depună | AUX   | Vmsp3           |             |
| bani   | NOUN  | Ncmp-n          |             |
| ,      | PUNCT | COMMA           |             |
| apoi   | ADV   | Rgp             |             |
| s-     | PRON  | Px3--a--y-----w |             |
| a      | AUX   | Va--3s          |             |
| așezat | VERB  | Vmp--sm         |             |
| pe     | ADP   | Spsa            |             |
| o      | DET   | Tifsr           |             |
| bancă  | NOUN  | Ncfsrn          |             |
| în     | ADP   | Spsa            |             |
| parc   | NOUN  | Ncms-n          |             |
| .      | PUNCT | PERIOD          |             |

#### Dependency Relations

| Token  | Dep     | Head   | Explanation         |
| ------ | ------- | ------ | ------------------- |
| Ion    | nsubj   | mers   | nominal subject     |
| a      | aux     | mers   | auxiliary           |
| mers   | ROOT    | mers   | root                |
| la     | case    | bancă  | case marking        |
| bancă  | obl     | mers   | oblique nominal     |
| să     | mark    | depună | marker              |
| depună | ccomp   | mers   | clausal complement  |
| bani   | obj     | depună | object              |
| ,      | punct   | așezat | punctuation         |
| apoi   | advmod  | așezat | adverbial modifier  |
| s-     | expl:pv | așezat |                     |
| a      | aux     | așezat | auxiliary           |
| așezat | conj    | mers   | conjunct            |
| pe     | case    | bancă  | case marking        |
| o      | det     | bancă  | determiner          |
| bancă  | obl     | așezat | oblique nominal     |
| în     | case    | parc   | case marking        |
| parc   | nmod    | bancă  | modifier of nominal |
| .      | punct   | mers   | punctuation         |

#### Noun Phrase Chunks

| Chunk              | Head  | Dep |
| ------------------ | ----- | --- |
| la bancă           | bancă | obl |
| pe o bancă în parc | bancă | obl |

#### Named Entity Recognition

| Entity | Label | Description                  |
| ------ | ----- | ---------------------------- |
| –      | –     | (no named entities detected) |

---

### Sentence 2

**„Profesorul de la Universitatea Babeș-Bolyai din Cluj-Napoca a publicat o carte nouă.”**

#### Lemmatization

| Token         | Lemma        |
| ------------- | ------------ |
| Profesorul    | profesor     |
| de            | de           |
| la            | la           |
| Universitatea | universitate |
| Babeș-Bolyai  | Babeș-Bolyai |
| din           | din          |
| Cluj-Napoca   | Cluj-Napoca  |
| a             | avea         |
| publicat      | publica      |
| o             | un           |
| carte         | carte        |
| nouă          | nou          |

#### POS Tagging

| Token         | POS   | Fine POS | Explanation |
| ------------- | ----- | -------- | ----------- |
| Profesorul    | NOUN  | Ncmsry   |             |
| de            | ADP   | Spsa     |             |
| la            | ADP   | Spsa     |             |
| Universitatea | NOUN  | Ncfsry   |             |
| Babeș-Bolyai  | PROPN | Np       |             |
| din           | ADP   | Spsa     |             |
| Cluj-Napoca   | PROPN | Np       |             |
| a             | AUX   | Va--3s   |             |
| publicat      | VERB  | Vmp--sm  |             |
| o             | DET   | Tifsr    |             |
| carte         | NOUN  | Ncfsrn   |             |
| nouă          | ADJ   | Afpfsrn  |             |
| .             | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token         | Dep   | Head          | Explanation                |
| ------------- | ----- | ------------- | -------------------------- |
| Profesorul    | nsubj | publicat      | nominal subject            |
| de            | case  | Universitatea | case marking               |
| la            | fixed | de            | fixed multiword expression |
| Universitatea | nmod  | Profesorul    | modifier of nominal        |
| Babeș-Bolyai  | nmod  | Universitatea | modifier of nominal        |
| din           | case  | Cluj-Napoca   | case marking               |
| Cluj-Napoca   | nmod  | Universitatea | modifier of nominal        |
| a             | aux   | publicat      | auxiliary                  |
| publicat      | ROOT  | publicat      | root                       |
| o             | det   | carte         | determiner                 |
| carte         | obj   | publicat      | object                     |
| nouă          | amod  | carte         | adjectival modifier        |
| .             | punct | publicat      | punctuation                |

#### Noun Phrase Chunks

| Chunk                                                       | Head       | Dep   |
| ----------------------------------------------------------- | ---------- | ----- |
| Profesorul de la Universitatea Babeș-Bolyai din Cluj-Napoca | Profesorul | nsubj |
| o carte nouă                                                | carte      | obj   |

#### Named Entity Recognition

| Entity                     | Label        | Description                                   |
| -------------------------- | ------------ | --------------------------------------------- |
| Profesorul                 | PERSON       | People, including fictional                   |
| Universitatea Babeș-Bolyai | ORGANIZATION | ORGANIZATION                                  |
| Cluj-Napoca                | GPE          | Countries, cities, states                     |
| carte                      | PRODUCT      | Objects, vehicles, foods, etc. (not services) |

---

### Sentence 3

**„Ea a blocat ușa cu un bloc de piatră adus din blocul vecin.”**

#### Lemmatization

| Token  | Lemma  |
| ------ | ------ |
| Ea     | el     |
| a      | avea   |
| blocat | bloca  |
| ușa    | ușă    |
| cu     | cu     |
| un     | un     |
| bloc   | bloc   |
| de     | de     |
| piatră | piatră |
| adus   | aduce  |
| din    | din    |
| blocul | bloc   |
| vecin  | vecin  |

#### POS Tagging

| Token  | POS   | Fine POS        | Explanation |
| ------ | ----- | --------------- | ----------- |
| Ea     | PRON  | Pp3fsr--------s |             |
| a      | AUX   | Va--3s          |             |
| blocat | VERB  | Vmp--sm         |             |
| ușa    | NOUN  | Ncfsry          |             |
| cu     | ADP   | Spsa            |             |
| un     | DET   | Timsr           |             |
| bloc   | NOUN  | Ncms-n          |             |
| de     | ADP   | Spsa            |             |
| piatră | NOUN  | Ncfsrn          |             |
| adus   | VERB  | Vmp--sm         |             |
| din    | ADP   | Spsa            |             |
| blocul | NOUN  | Ncmsry          |             |
| vecin  | NOUN  | Ncms-n          |             |
| .      | PUNCT | PERIOD          |             |

#### Dependency Relations

| Token  | Dep   | Head   | Explanation                                  |
| ------ | ----- | ------ | -------------------------------------------- |
| Ea     | nsubj | blocat | nominal subject                              |
| a      | aux   | blocat | auxiliary                                    |
| blocat | ROOT  | blocat | root                                         |
| ușa    | obj   | blocat | object                                       |
| cu     | case  | bloc   | case marking                                 |
| un     | det   | bloc   | determiner                                   |
| bloc   | obl   | blocat | oblique nominal                              |
| de     | case  | piatră | case marking                                 |
| piatră | nmod  | bloc   | modifier of nominal                          |
| adus   | acl   | bloc   | clausal modifier of noun (adjectival clause) |
| din    | case  | blocul | case marking                                 |
| blocul | obl   | adus   | oblique nominal                              |
| vecin  | nmod  | blocul | modifier of nominal                          |
| .      | punct | blocat | punctuation                                  |

#### Noun Phrase Chunks

| Chunk                                      | Head | Dep |
| ------------------------------------------ | ---- | --- |
| cu un bloc de piatră adus din blocul vecin | bloc | obl |

#### Named Entity Recognition

| Entity | Label   | Description                                   |
| ------ | ------- | --------------------------------------------- |
| Ea     | PERSON  | People, including fictional                   |
| ușa    | PRODUCT | Objects, vehicles, foods, etc. (not services) |

---

### Sentence 4

**„Lupul și-a ascuns lupul din priviri lângă Cascada Bigăr din Caraș-Severin.”**

#### Lemmatization

| Token         | Lemma         |
| ------------- | ------------- |
| Lupul         | lup           |
| și-           | sine          |
| a             | avea          |
| ascuns        | ascunde       |
| lupul         | lup           |
| din           | din           |
| priviri       | privire       |
| lângă         | lângă         |
| Cascada       | Cascadă       |
| Bigăr         | Bigăr         |
| din           | din           |
| Caraș-Severin | Caraș-Severin |

#### POS Tagging

| Token         | POS   | Fine POS        | Explanation |
| ------------- | ----- | --------------- | ----------- |
| Lupul         | NOUN  | Ncmsry          |             |
| și-           | PRON  | Px3--d--y-----w |             |
| a             | AUX   | Va--3s          |             |
| ascuns        | VERB  | Vmp--sm         |             |
| lupul         | NOUN  | Ncmsry          |             |
| din           | ADP   | Spsa            |             |
| priviri       | NOUN  | Ncfp-n          |             |
| lângă         | ADP   | Spsa            |             |
| Cascada       | NOUN  | Ncfsry          |             |
| Bigăr         | PROPN | Np              |             |
| din           | ADP   | Spsa            |             |
| Caraș-Severin | PROPN | Np              |             |
| .             | PUNCT | PERIOD          |             |

#### Dependency Relations

| Token         | Dep       | Head          | Explanation               |
| ------------- | --------- | ------------- | ------------------------- |
| Lupul         | nsubj     | ascuns        | nominal subject           |
| și-           | expl:poss | ascuns        |                           |
| a             | aux       | ascuns        | auxiliary                 |
| ascuns        | ROOT      | ascuns        | root                      |
| lupul         | obj       | ascuns        | object                    |
| din           | case      | priviri       | case marking              |
| priviri       | nmod      | lupul         | modifier of nominal       |
| lângă         | case      | Cascada       | case marking              |
| Cascada       | obl       | ascuns        | oblique nominal           |
| Bigăr         | flat      | Cascada       | flat multiword expression |
| din           | case      | Caraș-Severin | case marking              |
| Caraș-Severin | nmod      | Cascada       | modifier of nominal       |
| .             | punct     | ascuns        | punctuation               |

#### Noun Phrase Chunks

| Chunk                                 | Head    | Dep |
| ------------------------------------- | ------- | --- |
| lupul din priviri                     | lupul   | obj |
| lângă Cascada Bigăr din Caraș-Severin | Cascada | obl |

#### Named Entity Recognition

| Entity        | Label | Description                                         |
| ------------- | ----- | --------------------------------------------------- |
| Cascada Bigăr | LOC   | Non-GPE locations, mountain ranges, bodies of water |
| Caraș-Severin | GPE   | Countries, cities, states                           |

---

### Sentence 5

**„Am visat un vis frumos despre Marea Neagră în noaptea de Sfântul Andrei.”**

#### Lemmatization

| Token   | Lemma  |
| ------- | ------ |
| Am      | avea   |
| visat   | visa   |
| un      | un     |
| vis     | vis    |
| frumos  | frumos |
| despre  | despre |
| Marea   | mare   |
| Neagră  | Neagră |
| în      | în     |
| noaptea | noapte |
| de      | de     |
| Sfântul | sfânt  |
| Andrei  | Andrei |

#### POS Tagging

| Token   | POS   | Fine POS | Explanation |
| ------- | ----- | -------- | ----------- |
| Am      | AUX   | Va--1    |             |
| visat   | VERB  | Vmp--sm  |             |
| un      | DET   | Timsr    |             |
| vis     | NOUN  | Ncms-n   |             |
| frumos  | ADJ   | Afpms-n  |             |
| despre  | ADP   | Spsa     |             |
| Marea   | NOUN  | Ncfsry   |             |
| Neagră  | ADJ   | Afpfsrn  |             |
| în      | ADP   | Spsa     |             |
| noaptea | NOUN  | Ncfsry   |             |
| de      | ADP   | Spsa     |             |
| Sfântul | NOUN  | Ncmsry   |             |
| Andrei  | PROPN | Np       |             |
| .       | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token   | Dep   | Head    | Explanation               |
| ------- | ----- | ------- | ------------------------- |
| Am      | aux   | visat   | auxiliary                 |
| visat   | ROOT  | visat   | root                      |
| un      | det   | vis     | determiner                |
| vis     | obj   | visat   | object                    |
| frumos  | amod  | vis     | adjectival modifier       |
| despre  | case  | Marea   | case marking              |
| Marea   | nmod  | vis     | modifier of nominal       |
| Neagră  | flat  | Marea   | flat multiword expression |
| în      | case  | noaptea | case marking              |
| noaptea | obl   | visat   | oblique nominal           |
| de      | case  | Sfântul | case marking              |
| Sfântul | nmod  | noaptea | modifier of nominal       |
| Andrei  | nmod  | Sfântul | modifier of nominal       |
| .       | punct | visat   | punctuation               |

#### Noun Phrase Chunks

| Chunk                             | Head    | Dep |
| --------------------------------- | ------- | --- |
| un vis frumos despre Marea Neagră | vis     | obj |
| în noaptea de Sfântul Andrei      | noaptea | obl |

#### Named Entity Recognition

| Entity       | Label | Description                                         |
| ------------ | ----- | --------------------------------------------------- |
| Marea Neagră | LOC   | Non-GPE locations, mountain ranges, bodies of water |

---

### Sentence 6

**„Poarta cetății din Alba Iulia a fost deschisă de poarta unui jucător celebru.”**

#### Lemmatization

| Token    | Lemma    |
| -------- | -------- |
| Poarta   | poartă   |
| cetății  | cetate   |
| din      | din      |
| Alba     | Alba     |
| Iulia    | Iulia    |
| a        | avea     |
| fost     | fi       |
| deschisă | deschisă |
| de       | de       |
| poarta   | poartă   |
| unui     | un       |
| jucător  | jucător  |
| celebru  | celebru  |

#### POS Tagging

| Token    | POS   | Fine POS | Explanation |
| -------- | ----- | -------- | ----------- |
| Poarta   | NOUN  | Ncfsry   |             |
| cetății  | NOUN  | Ncfsoy   |             |
| din      | ADP   | Spsa     |             |
| Alba     | PROPN | Np       |             |
| Iulia    | PROPN | Np       |             |
| a        | AUX   | Va--3s   |             |
| fost     | AUX   | Vap--sm  |             |
| deschisă | VERB  | Vmp--sf  |             |
| de       | ADP   | Spsa     |             |
| poarta   | NOUN  | Ncfsry   |             |
| unui     | DET   | Timso    |             |
| jucător  | NOUN  | Ncms-n   |             |
| celebru  | ADJ   | Afpms-n  |             |
| .        | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token    | Dep        | Head     | Explanation               |
| -------- | ---------- | -------- | ------------------------- |
| Poarta   | nsubj:pass | deschisă |                           |
| cetății  | nmod       | Poarta   | modifier of nominal       |
| din      | case       | Alba     | case marking              |
| Alba     | nmod       | Poarta   | modifier of nominal       |
| Iulia    | flat       | Alba     | flat multiword expression |
| a        | aux        | deschisă | auxiliary                 |
| fost     | aux:pass   | deschisă |                           |
| deschisă | ROOT       | deschisă | root                      |
| de       | case       | poarta   | case marking              |
| poarta   | obl:agent  | deschisă | oblique agent             |
| unui     | det        | jucător  | determiner                |
| jucător  | nmod       | poarta   | modifier of nominal       |
| celebru  | amod       | jucător  | adjectival modifier       |
| .        | punct      | deschisă | punctuation               |

#### Noun Phrase Chunks

| Chunk                          | Head   | Dep        |
| ------------------------------ | ------ | ---------- |
| Poarta cetății din Alba Iulia  | Poarta | nsubj:pass |
| de poarta unui jucător celebru | poarta | obl:agent  |

#### Named Entity Recognition

| Entity     | Label       | Description                 |
| ---------- | ----------- | --------------------------- |
| Alba Iulia | GPE         | Countries, cities, states   |
| jucător    | PERSON      | People, including fictional |
| celebru    | NAT_REL_POL | NAT_REL_POL                 |

---

### Sentence 7

**„Marcel a tăiat un corn din copac în timp ce mânca un corn cu ciocolată.”**

#### Lemmatization

| Token     | Lemma     |
| --------- | --------- |
| Marcel    | Marcel    |
| a         | avea      |
| tăiat     | tăia      |
| un        | un        |
| corn      | corn      |
| din       | din       |
| copac     | copac     |
| în        | în        |
| timp      | timp      |
| ce        | ce        |
| mânca     | mânca     |
| un        | un        |
| corn      | corn      |
| cu        | cu        |
| ciocolată | ciocolată |

#### POS Tagging

| Token     | POS   | Fine POS | Explanation |
| --------- | ----- | -------- | ----------- |
| Marcel    | PROPN | Np       |             |
| a         | AUX   | Va--3s   |             |
| tăiat     | VERB  | Vmp--sm  |             |
| un        | DET   | Timsr    |             |
| corn      | NOUN  | Ncms-n   |             |
| din       | ADP   | Spsa     |             |
| copac     | NOUN  | Ncms-n   |             |
| în        | ADP   | Spsa     |             |
| timp      | NOUN  | Ncms-n   |             |
| ce        | PRON  | Pw3--r   |             |
| mânca     | AUX   | Vmii3s   |             |
| un        | DET   | Timsr    |             |
| corn      | NOUN  | Ncms-n   |             |
| cu        | ADP   | Spsa     |             |
| ciocolată | NOUN  | Ncfsrn   |             |
| .         | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token     | Dep   | Head      | Explanation                |
| --------- | ----- | --------- | -------------------------- |
| Marcel    | nsubj | tăiat     | nominal subject            |
| a         | aux   | tăiat     | auxiliary                  |
| tăiat     | ROOT  | tăiat     | root                       |
| un        | det   | corn      | determiner                 |
| corn      | obj   | tăiat     | object                     |
| din       | case  | copac     | case marking               |
| copac     | nmod  | corn      | modifier of nominal        |
| în        | mark  | mânca     | marker                     |
| timp      | fixed | în        | fixed multiword expression |
| ce        | fixed | în        | fixed multiword expression |
| mânca     | advcl | tăiat     | adverbial clause modifier  |
| un        | det   | corn      | determiner                 |
| corn      | obj   | mânca     | object                     |
| cu        | case  | ciocolată | case marking               |
| ciocolată | nmod  | corn      | modifier of nominal        |
| .         | punct | tăiat     | punctuation                |

#### Noun Phrase Chunks

| Chunk                | Head | Dep |
| -------------------- | ---- | --- |
| un corn din copac    | corn | obj |
| un corn cu ciocolată | corn | obj |

#### Named Entity Recognition

| Entity | Label | Description                  |
| ------ | ----- | ---------------------------- |
| –      | –     | (no named entities detected) |

---

### Sentence 8

**„Vântul a bătut puternic la Bâlea Lac, iar ceasul a bătut miezul nopții.”**

#### Lemmatization

| Token    | Lemma    |
| -------- | -------- |
| Vântul   | vânt     |
| a        | avea     |
| bătut    | bate     |
| puternic | puternic |
| la       | la       |
| Bâlea    | Bâlea    |
| Lac      | lac      |
| iar      | iar      |
| ceasul   | ceas     |
| a        | avea     |
| bătut    | bate     |
| miezul   | miez     |
| nopții   | noapte   |

#### POS Tagging

| Token    | POS   | Fine POS | Explanation |
| -------- | ----- | -------- | ----------- |
| Vântul   | NOUN  | Ncmsry   |             |
| a        | AUX   | Va--3s   |             |
| bătut    | VERB  | Vmp--sm  |             |
| puternic | ADV   | Rgp      |             |
| la       | ADP   | Spsa     |             |
| Bâlea    | PROPN | Np       |             |
| Lac      | NOUN  | Ncms-n   |             |
| ,        | PUNCT | COMMA    |             |
| iar      | ADV   | Rc       |             |
| ceasul   | NOUN  | Ncmsry   |             |
| a        | AUX   | Va--3s   |             |
| bătut    | VERB  | Vmp--sm  |             |
| miezul   | NOUN  | Ncmsry   |             |
| nopții   | NOUN  | Ncfsoy   |             |
| .        | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token    | Dep    | Head   | Explanation               |
| -------- | ------ | ------ | ------------------------- |
| Vântul   | nsubj  | bătut  | nominal subject           |
| a        | aux    | bătut  | auxiliary                 |
| bătut    | ROOT   | bătut  | root                      |
| puternic | advmod | bătut  | adverbial modifier        |
| la       | case   | Bâlea  | case marking              |
| Bâlea    | obl    | bătut  | oblique nominal           |
| Lac      | flat   | Bâlea  | flat multiword expression |
| ,        | punct  | bătut  | punctuation               |
| iar      | cc     | bătut  | coordinating conjunction  |
| ceasul   | nsubj  | bătut  | nominal subject           |
| a        | aux    | bătut  | auxiliary                 |
| bătut    | conj   | bătut  | conjunct                  |
| miezul   | obj    | bătut  | object                    |
| nopții   | nmod   | miezul | modifier of nominal       |
| .        | punct  | bătut  | punctuation               |

#### Noun Phrase Chunks

| Chunk         | Head   | Dep |
| ------------- | ------ | --- |
| la Bâlea Lac  | Bâlea  | obl |
| miezul nopții | miezul | obj |

#### Named Entity Recognition

| Entity        | Label    | Description                                         |
| ------------- | -------- | --------------------------------------------------- |
| Bâlea Lac     | LOC      | Non-GPE locations, mountain ranges, bodies of water |
| miezul nopții | DATETIME | DATETIME                                            |

---

### Sentence 9

**„Costul costumului popular din Maramureș a costat mai mult decât se aștepta Maria.”**

#### Lemmatization

| Token      | Lemma     |
| ---------- | --------- |
| Costul     | cost      |
| costumului | costum    |
| popular    | popular   |
| din        | din       |
| Maramureș  | Maramureș |
| a          | avea      |
| costat     | costa     |
| mai        | mai       |
| mult       | mult      |
| decât      | decât     |
| se         | sine      |
| aștepta    | aștepta   |
| Maria      | Maria     |

#### POS Tagging

| Token      | POS   | Fine POS        | Explanation |
| ---------- | ----- | --------------- | ----------- |
| Costul     | NOUN  | Ncmsry          |             |
| costumului | NOUN  | Ncmsoy          |             |
| popular    | ADJ   | Afpms-n         |             |
| din        | ADP   | Spsa            |             |
| Maramureș  | PROPN | Np              |             |
| a          | AUX   | Va--3s          |             |
| costat     | VERB  | Vmp--sm         |             |
| mai        | ADV   | Rp              |             |
| mult       | ADV   | Rgp             |             |
| decât      | ADV   | Rc              |             |
| se         | PRON  | Px3--a--------w |             |
| aștepta    | AUX   | Vmii3s          |             |
| Maria      | PROPN | Np              |             |
| .          | PUNCT | PERIOD          |             |

#### Dependency Relations

| Token      | Dep       | Head       | Explanation               |
| ---------- | --------- | ---------- | ------------------------- |
| Costul     | nsubj     | costat     | nominal subject           |
| costumului | nmod      | Costul     | modifier of nominal       |
| popular    | amod      | costumului | adjectival modifier       |
| din        | case      | Maramureș  | case marking              |
| Maramureș  | nmod      | costumului | modifier of nominal       |
| a          | aux       | costat     | auxiliary                 |
| costat     | ROOT      | costat     | root                      |
| mai        | advmod    | mult       | adverbial modifier        |
| mult       | advmod    | costat     | adverbial modifier        |
| decât      | mark      | aștepta    | marker                    |
| se         | expl:pass | aștepta    |                           |
| aștepta    | advcl     | mult       | adverbial clause modifier |
| Maria      | nsubj     | aștepta    | nominal subject           |
| .          | punct     | costat     | punctuation               |

#### Noun Phrase Chunks

| Chunk                                   | Head   | Dep   |
| --------------------------------------- | ------ | ----- |
| Costul costumului popular din Maramureș | Costul | nsubj |

#### Named Entity Recognition

| Entity | Label | Description                  |
| ------ | ----- | ---------------------------- |
| –      | –     | (no named entities detected) |

---

### Sentence 10

**„Tudor a pus capac la oala de pe foc și capac la discuția cu Elena.”**

#### Lemmatization

| Token    | Lemma    |
| -------- | -------- |
| Tudor    | Tudor    |
| a        | avea     |
| pus      | pune     |
| capac    | capac    |
| la       | la       |
| oala     | oală     |
| de       | de       |
| pe       | pe       |
| foc      | foc      |
| și       | și       |
| capac    | capac    |
| la       | la       |
| discuția | discuție |
| cu       | cu       |
| Elena    | Elena    |

#### POS Tagging

| Token    | POS   | Fine POS | Explanation |
| -------- | ----- | -------- | ----------- |
| Tudor    | PROPN | Np       |             |
| a        | AUX   | Va--3s   |             |
| pus      | VERB  | Vmp--sm  |             |
| capac    | NOUN  | Ncms-n   |             |
| la       | ADP   | Spsa     |             |
| oala     | NOUN  | Ncfsry   |             |
| de       | ADP   | Spsa     |             |
| pe       | ADP   | Spsa     |             |
| foc      | NOUN  | Ncms-n   |             |
| și       | CCONJ | Crssp    |             |
| capac    | NOUN  | Ncms-n   |             |
| la       | ADP   | Spsa     |             |
| discuția | NOUN  | Ncfsry   |             |
| cu       | ADP   | Spsa     |             |
| Elena    | PROPN | Np       |             |
| .        | PUNCT | PERIOD   |             |

#### Dependency Relations

| Token    | Dep   | Head     | Explanation                |
| -------- | ----- | -------- | -------------------------- |
| Tudor    | nsubj | pus      | nominal subject            |
| a        | aux   | pus      | auxiliary                  |
| pus      | ROOT  | pus      | root                       |
| capac    | obj   | pus      | object                     |
| la       | case  | oala     | case marking               |
| oala     | obl   | pus      | oblique nominal            |
| de       | case  | foc      | case marking               |
| pe       | fixed | de       | fixed multiword expression |
| foc      | nmod  | oala     | modifier of nominal        |
| și       | cc    | capac    | coordinating conjunction   |
| capac    | conj  | foc      | conjunct                   |
| la       | case  | discuția | case marking               |
| discuția | nmod  | oala     | modifier of nominal        |
| cu       | case  | Elena    | case marking               |
| Elena    | nmod  | discuția | modifier of nominal        |
| .        | punct | pus      | punctuation                |

#### Noun Phrase Chunks

| Chunk                                           | Head | Dep |
| ----------------------------------------------- | ---- | --- |
| la oala de pe foc și capac la discuția cu Elena | oala | obl |

#### Named Entity Recognition

| Entity | Label  | Description                 |
| ------ | ------ | --------------------------- |
| Elena  | PERSON | People, including fictional |
