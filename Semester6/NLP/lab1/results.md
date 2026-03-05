## NLP Lab 1 – Preprocessing with SpaCy

**Student:** Apavaloaiei Alexandru  
**Lab:** 1  
**Date:** 23 February 2026

---

## 1. Task Description

The lab specification requires:

> Preprocess 10 English sentences and 10 Romanian (or Spanish/Italian/French) sentences using both SpaCy and NLP‑Cube/NLTK.  
> Operations: **stemming, tokenization, POS tagging, lemmatization, dependency relations and dependency parsing (dependency graph), chunking and NER (named entity recognition).**  
> The source code and results will be written to a file and uploaded to Teams.

In my implementation I focused on **SpaCy** and implemented the following operations for **10 English** and **10 Romanian** sentences:

- Tokenization
- Lemmatization
- POS tagging (coarse + fine tags)
- Dependency relations
- Dependency parsing (text tree + HTML dependency graphs)
- Chunking (noun phrase chunks or NP‑like subtrees)
- Named Entity Recognition (NER)

**Stemming** and a second toolkit (NLP‑Cube or NLTK) are **not explicitly implemented** in the current `main.py`. Instead, I use **lemmatization** as the main normalization step (it often plays a similar role to stemming for many downstream tasks). A comparison with a second toolkit and true stemmers is left as future work.

---

## 2. Implementation Details

### 2.1 Code Structure

All functionality is implemented in `main.py` and can be run from the command line:

- `python main.py --en` – process **English** sentences only
- `python main.py --ro` – process **Romanian** sentences only
- `python main.py` – process **both** English and Romanian sentences

The script:

- Loads SpaCy language models on demand:
  - English: `en_core_web_sm`
  - Romanian: `ro_core_news_sm`
- Defines two lists of 10 **challenging sentences** each:
  - `english_sentences` – polysemy, homonymy, idioms, passive voice, complex syntax
  - `romanian_sentences` – similar ambiguity and named entities
- For each sentence, it creates a SpaCy `Doc` and calls `process_sentence(doc, sentence_num, lang)` which:
  - Prints tokenization
  - Prints lemmatization
  - Prints POS tags (coarse `pos_` and fine `tag_` + explanation)
  - Prints dependency relations + explanations
  - Prints a text‑based dependency parse tree
  - Extracts noun phrase chunks (or NP‑like subtrees if `noun_chunks` is not supported)
  - Prints named entities and their types
- After processing all sentences in a language, it generates HTML visualizations:
  - `dependency_graphs_en.html` / `dependency_graphs_ro.html` (dependency parses)
  - `ner_visualization_en.html` / `ner_visualization_ro.html` (NER spans)

At the end, it prints which HTML files were generated so they can be opened or uploaded to Teams.

### 2.2 Models and Libraries

- **SpaCy**
  - English model: `en_core_web_sm`
  - Romanian model: `ro_core_news_sm`
  - Used for:
    - Tokenization
    - POS tagging (universal tag `pos_` and language‑specific `tag_`)
    - Lemmatization (`lemma_`)
    - Dependency relations and parse trees
    - Noun phrase chunking (`noun_chunks`) for English
    - Named Entity Recognition (`doc.ents`)
- **No explicit use of NLP‑Cube or NLTK** in the current code.
  - The “stemming” requirement is approximated by **lemmatization**, which returns dictionary base forms instead of crude stem truncation.

---

## 3. English Results (10 Sentences)

All 10 English sentences are crafted to expose **lexical ambiguity** (polysemy/homonymy). Below I summarize how SpaCy analyzes them; the numbers and structures are taken from the actual console output (`python main.py --en`).

### 3.1 Sentence 1 – “bank” (financial vs. riverbank)

**Sentence:**

> “The bank by the river bank approved my loan in the spring of last spring.”

- **Tokenization:** 16 tokens
- **Lemmas:** both `bank` tokens → lemma `bank`
- **POS tags (examples):**
  - `bank` (subject) – `NOUN/NN`
  - `bank` (in “river bank”) – `PROPN/NNP` in this run, acting as a proper noun
  - `approved` – `VERB/VBD`
- **Dependency relations (examples):**
  - First `bank`:
    - `bank` → `nsubj` of `approved`
    - `The` → `det` of `bank`
  - Second `bank`:
    - `by` → `prep` modifying the subject `bank`
    - `bank` (2nd) → `pobj` of `by`
    - `river` → `compound` modifier of this `bank`
  - Temporal PP:
    - `in` → `prep` of `approved`
    - `spring` → `pobj` of `in`
    - `of` → `prep` of `spring`
    - second `spring` → `pobj` of `of`, modified by `last` (`amod`)
- **Noun phrase chunks:**
  - “The bank” – subject NP (`root: bank, dep: nsubj`)
  - “the river bank” – object NP (`root: bank, dep: pobj`)
  - “my loan”, “the spring”, “last spring”
- **NER:**
  - “the spring of last spring” → DATE

**Observation:**  
SpaCy assigns different **syntactic roles** to the two occurrences of “bank” (subject vs. part of a PP), while keeping the same lemma, illustrating how syntax helps disambiguate meaning.

---

### 3.2 Sentence 2 – “bear / bear market”

**Sentence:**

> “Apple's CEO Tim Cook said he can't bear the bear market any longer.”

- **Key lemmas and POS:**
  - `bear` (verb) – lemma `bear`, POS `VERB/VB`
  - `bear` (in “bear market”) – lemma `bear`, POS `NOUN/NN`, `compound` of `market`
  - `market` – `NOUN/NN`
- **Dependencies (examples):**
  - `said` – `ROOT`
  - `CEO` – `nsubj` of `said` with:
    - `Apple` – `poss` (owner), `'s` – `case`
    - `Cook` – `appos` with `Tim` as `compound`
  - `bear` (verb) – `ccomp` of `said`:
    - `he` – `nsubj` of `bear`
    - `ca` (can) – `aux`
    - `n't` – `neg`
  - `market` – `dobj` of `bear` with `bear` as `compound`
- **NER:**
  - `Apple` → ORG
  - `Tim Cook` → PERSON

**Observation:**  
The parser clearly separates the **verb** “bear” (emotion) from the **noun** “bear” in the financial term “bear market” by POS and dependency role.

---

### 3.3 Sentence 3 – “her duck / roasted duck”

**Sentence:**

> “I saw her duck under the table and grab the roasted duck.”

- `saw` – `ROOT`, subject `I` (`nsubj`)
- First `duck`:
  - `her` – `poss`
  - `duck` – `dobj` of `saw`
- PP:
  - `under` – `prep` of `saw`
  - `table` – `pobj` of `under`, with `the` as `det`
- Coordination:
  - `and` – `cc` attached to `saw`
  - `grab` – `conj` of `saw`
  - `duck` (second) – `dobj` of `grab` with `the` as `det` and `roasted` as `amod`
- NER: none

Ambiguity between verb “to duck” and noun “duck” is resolved syntactically as **nouns** in this context.

---

### 3.4 Sentence 4 – “lead / lead / lead”

**Sentence:**

> “The lead singer refused to lead because he found lead in the water.”

- First `lead`:
  - `lead` – `NOUN/NN` (`compound`) modifying `singer`
  - `singer` – `nsubj` of `refused`
- Second `lead`:
  - `to` – `aux` of `lead`
  - `lead` – `xcomp` (open clausal complement) of `refused`
- Third `lead`:
  - `found` – `advcl` of `refused`, subject `he`
  - `lead` – `dobj` of `found`
  - `in the water` – PP modifying `lead`
- NER: none

Three different uses of “lead” (role noun, verb, metal noun) are separated through POS tags and dependency labels.

---

### 3.5 Sentences 5–10 (Short Overview)

- **Sentence 5 – “She left her left shoe on the right side of the left turn.”**
  - `left` – main verb (`ROOT`); “her left shoe” as direct object; “the right side of the left turn” as PP.
  - Demonstrates that `left` is both a **verb** (leave) and **adjective** (left/right).

- **Sentence 6 – “They were fired after they fired the cannon at the fire station.”**
  - First `fired` – passive main verb with subject “They” (`nsubjpass`) and auxiliary `were`.
  - Second `fired` – `advcl` subordinate clause with subject `they` and object `the cannon`.
  - “fire station” is a compound noun (`fire` → `compound` of `station`).

- **Sentence 7 – “The bass player caught a bass while playing bass‑heavy music on the boat.”**
  - “bass player” – subject NP; `player` is `nsubj` of `caught`.
  - Second `bass` – direct object of `caught`.
  - In “bass‑heavy music”, `bass` modifies `heavy`, which in turn modifies `music`.

- **Sentence 8 – “Will Will Smith smith a new sword for the Renaissance fair in May?”**
  - `smith` – `ROOT` verb.
  - Person name: `Smith` (`nsubj`) with two auxiliary `Will` tokens.
  - “a new sword” – direct object; `Renaissance` (pobj of `for`) with `the` as `det`; `May` as `pobj` of `in`.
  - NER: “Will Will Smith” → PERSON; “Renaissance” → ORG; “May” → DATE.

- **Sentence 9 – “The bark of the old oak made the dog bark louder at night.”**
  - `bark` (tree) – `nsubj` of `made` with PP `of the old oak`.
  - `bark` (dog) – `dobj` of `made`.
  - `night` – `pobj` of `at`, labeled TIME.

- **Sentence 10 – “Can you book a flight to Reading while reading this booking confirmation?”**
  - `book` – `ROOT` verb; `Can` (`aux`), `you` (`nsubj`).
  - `flight` – `dobj` with PP `to Reading`.
  - Subordinate clause: `reading` – `advcl` of `book`, direct object “this booking confirmation”.
  - “Reading” is analyzed as a verb form (VBG), although it is semantically a place name here.

---

## 4. Romanian Results (10 Sentences)

The Romanian sentences mirror the English ones in ambiguity. SpaCy’s Romanian model (`ro_core_news_sm`) provides **tokenization, POS tags, lemmas, dependencies, chunking (via NP‑like subtrees), and NER**.

### 4.1 Sentence 1 – “bancă” (financial vs. bench)

**Sentence:**

> „Ion a mers la bancă să depună bani, apoi s‑a așezat pe o bancă în parc.”

- **Lemmas (examples):**
  - `Ion` → `Ion`
  - `a` → `avea`
  - `mers` → `merge`
  - `bancă` → `bancă` (both occurrences)
  - `așezat` → `așeza`
- **POS tags:**
  - `Ion` – `PROPN/Np`
  - `a` – `AUX/Va--3s`
  - `mers` – `VERB/Vmp--sm`
  - `la` – `ADP/Spsa`
  - `bancă` – `NOUN/Ncfsrn`
- **Dependencies:**
  - `mers` – `ROOT` with:
    - `Ion` – `nsubj`
    - `a` – `aux`
    - `bancă` (first) – `obl` with `la` as `case`
    - `depună` – `ccomp` with `să` as `mark` and `bani` as `obj`
    - `așezat` – `conj` with its own subtree:
      - `bancă` (second) – `obl` with `pe` (`case`) and `o` (`det`)
      - `parc` – `nmod` with `în` as `case`
- **NP‑like chunks (fallback):**
  - „la bancă” (head `bancă`, dep `obl`)
  - „pe o bancă în parc” (head `bancă`, dep `obl`)
- **NER:** none

**Observation:**  
As in English “bank”, the same lemma `bancă` is used in two different senses (financial institution vs. bench), and SpaCy distinguishes them via syntactic role and modifiers.

---

### 4.2 Sentence 2 – University and City Names

**Sentence:**

> „Profesorul de la Universitatea Babeș-Bolyai din Cluj-Napoca a publicat o carte nouă.”

- `Profesorul` – `nsubj` of `publicat`
- `Universitatea` – `nmod` of `Profesorul` with:
  - `de` – `case`
  - `la` – `fixed`
  - `Babeș-Bolyai` – `nmod` of `Universitatea`
  - `Cluj-Napoca` – `nmod` of `Universitatea` with `din` as `case`
- NP‑like chunks:
  - „Profesorul de la Universitatea Babeș-Bolyai din Cluj-Napoca”
  - „o carte nouă”
- NER:
  - „Profesorul” → PERSON
  - „Universitatea Babeș-Bolyai” → ORGANIZATION
  - „Cluj-Napoca” → GPE
  - „carte” → PRODUCT

---

### 4.3 Sentences 3–10 (Short Overview)

- **Sentence 3 – “bloc / blocul”**

  > „Ea a blocat ușa cu un bloc de piatră adus din blocul vecin.”
  - `blocat` – `ROOT` with:
    - `Ea` – `nsubj`, `a` – `aux`
    - `ușa` – `obj`
    - `bloc` – `obl` with `cu` (`case`), `un` (`det`), and `piatră` (`nmod`)
    - `adus` – `acl` modifying `bloc`, with `blocul` (`obl`) and `vecin` (`nmod`)
  - NER: “Ea” → PERSON, “ușa” → PRODUCT.

- **Sentence 4 – “lupul din priviri” & locations**

  > „Lupul și-a ascuns lupul din priviri lângă Cascada Bigăr din Caraș-Severin.”
  - `ascuns` – `ROOT`, `Lupul` (`nsubj`), pronominal clitic `și-` (`expl:poss`).
  - Object `lupul` with `priviri` (`nmod`) and preposition `din`.
  - PP `lângă Cascada Bigăr din Caraș-Severin` with `Cascada` as `obl`, `Bigăr` as `flat`, `Caraș-Severin` as `nmod` with `din` as `case`.
  - NER: „Cascada Bigăr” → LOC; „Caraș-Severin” → GPE.

- **Sentence 5 – „Marea Neagră”**

  > „Am visat un vis frumos despre Marea Neagră în noaptea de Sfântul Andrei.”
  - `visat` – `ROOT` with `Am` (`aux`), `vis` (`obj`).
  - `Marea` – `nmod` of `vis` with `despre` (`case`) and `Neagră` as `flat`.
  - `noaptea` – `obl` with `în` (`case`), modified by `Sfântul` and `Andrei`.
  - NER: „Marea Neagră” → LOC.

- **Sentence 6 – “Poarta … de poarta unui jucător celebru.”**

  > „Poarta cetății din Alba Iulia a fost deschisă de poarta unui jucător celebru.”
  - Passive verb `deschisă` as `ROOT`.
  - `Poarta` – `nsubj:pass` with modifiers `cetății`, `din` + `Alba Iulia`.
  - `poarta` – `obl:agent` with `de` (`case`) and `jucător` (`nmod`) modified by `unui` and `celebru`.
  - NER: „Alba Iulia” → GPE; „jucător” → PERSON; „celebru” → NAT_REL_POL.

- **Sentence 7 – “corn din copac / corn cu ciocolată”**

  > „Marcel a tăiat un corn din copac în timp ce mânca un corn cu ciocolată.”
  - `tăiat` – `ROOT` with subject `Marcel`, object `corn` + `copac` as `nmod` (`din` as `case`).
  - Subordinate clause `mânca` (`advcl`) with second `corn` (`obj`) and `ciocolată` as `nmod` (`cu` as `case`).
  - Shows same lemma `corn` with two different real‑world referents.

- **Sentence 8 – “bătut … bătut miezul nopții”**

  > „Vântul a bătut puternic la Bâlea Lac, iar ceasul a bătut miezul nopții.”
  - First `bătut` – `ROOT` with subject `Vântul`, advmod `puternic`, and `Bâlea` (`obl`) + `Lac` (`flat`).
  - Second `bătut` – `conj` with subject `ceasul` and object `miezul` with `nopții` (`nmod`).
  - NER: „Bâlea Lac” → LOC; „miezul nopții” → DATETIME.

- **Sentence 9 – “Costul costumului … a costat mai mult decât se aștepta Maria.”**

  > „Costul costumului popular din Maramureș a costat mai mult decât se aștepta Maria.”
  - `costat` – `ROOT` with subject `Costul` and nominal modifier `costumului` (with `popular` and `Maramureș`).
  - Comparative construction: `mult` (`advmod`) modified by `mai`, with subordinate `aștepta` (`advcl`) and subject `Maria`.

- **Sentence 10 – “capac la oală / capac la discuție”**
  > „Tudor a pus capac la oala de pe foc și capac la discuția cu Elena.”
  - `pus` – `ROOT` with subject `Tudor`, object `capac`.
  - `oala` – `obl` with `la` (`case`), `foc` (`nmod`) with `de` and `pe`.
  - Second `capac` – `conj` (metaphorical) attached via coordination, with `discuția` + `Elena`.
  - NER: „Elena” → PERSON.

---

## 5. Discussion: Operations and Requirements

### 5.1 Implemented Operations

For all 20 sentences (10 EN, 10 RO), the script performs:

- **Tokenization:**  
  Splitting text into tokens with SpaCy’s language‑specific rules. Handles punctuation, clitics (e.g. `s-a`, `și-a`), contractions, and hyphenation.

- **Lemmatization:**  
  Each token is mapped to its base form (`token.lemma_`), e.g.:
  - English: `approved` → `approve`, `left` → `leave`, `reading` → `read`
  - Romanian: `mers` → `merge`, `costat` → `costa`, `visat` → `visa`

- **POS Tagging:**  
  Two levels of POS information per token:
  - Coarse tag: `token.pos_` (e.g. `NOUN`, `VERB`, `AUX`, `ADP`, `PROPN`)
  - Fine tag: `token.tag_` (e.g. `NN`, `VBD` in English, or `Ncfsrn`, `Vmp--sm` in Romanian)  
    The script also prints explanations using `spacy.explain`.

- **Dependency Relations and Parse Trees:**  
  For each token, the script prints:
  - Dependency label (`token.dep_`)
  - Head word (`token.head.text`)
  - Human‑readable explanation  
    It also prints a **recursive text tree** for each sentence, starting from the root verb.

- **Chunking:**
  - English: uses `doc.noun_chunks` (SpaCy’s NP chunker).
  - Romanian: when `noun_chunks` is not implemented, the script falls back to `_fallback_noun_chunks`, which builds spans from noun‑headed subtrees.

- **Named Entity Recognition (NER):**  
  For both languages, the script prints each entity (`ent.text`), its label (`ent.label_`), and an explanation.  
  Examples:
  - English: `Apple` (ORG), `Tim Cook` (PERSON), `May` (DATE), “the spring of last spring” (DATE), “night” (TIME).
  - Romanian: `Universitatea Babeș-Bolyai` (ORGANIZATION), `Cluj-Napoca` (GPE), `Marea Neagră` (LOC), `Alba Iulia` (GPE), `Bâlea Lac` (LOC), `Elena` (PERSON).

### 5.2 Not Implemented / Limitations

- **Stemming:**  
  True stemming (e.g. Porter/Snowball stemmer) is not implemented. Instead, lemmatization is used as the main normalization step. While lemmas are linguistically richer than stems, they are not identical to classic stems.

- **Second Toolkit (NLP‑Cube / NLTK):**  
  Only SpaCy is used in this version. A future extension would be to:
  - Add NLTK tokenization + stemming for English and compare with SpaCy’s lemmas.
  - Optionally use NLP‑Cube for Romanian morphological analysis and compare the outputs.

- **NER Coverage:**  
  Not all possible entities are recognized (e.g., “Reading” in the English sentence may be treated as a verb rather than a location). This reflects limits of the small pretrained models.

---

## 6. Generated Output Files

Running the script generates the following visualizations:

- **Dependency graphs:**
  - `dependency_graphs_en.html` – all English sentences
  - `dependency_graphs_ro.html` – all Romanian sentences
- **NER visualizations:**
  - `ner_visualization_en.html`
  - `ner_visualization_ro.html`

These HTML files show:

- Tokens along with arrows indicating syntactic dependencies.
- Heads and dependency labels between words.
- Highlighted named entities with color‑coded labels.

They, together with `main.py` and this report, can be uploaded to the Teams folder as the deliverables for the lab.

---

## 7. Conclusions

- I processed **10 English** and **10 Romanian** sentences using **SpaCy**, applying tokenization, lemmatization, POS tagging, dependency parsing, chunking, and NER.
- The sentences were chosen to emphasize **polysemy, homonymy, and complex syntax**, and the resulting analyses (from the terminal output and HTML visualizations) show that SpaCy can handle these cases reasonably well.
- Romanian outputs demonstrate that modern NLP tools can provide rich morpho‑syntactic analysis even for morphologically complex languages, including NP‑like chunks and named entities.
- Although the original assignment also mentioned stemming and a second toolkit (NLP‑Cube/NLTK), the current implementation focuses on SpaCy only; extending the project with a stemmer and a second tagger/parser would allow a more systematic comparison between **lemmatization vs. stemming** and between different toolkits.
