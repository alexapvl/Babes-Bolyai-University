## English Results – Detailed Tables

### Sentence 1  
**"The bank by the river bank approved my loan in the spring of last spring."**

#### Lemmatization

| Token | Lemma   |
|-------|--------|
| The   | the    |
| bank  | bank   |
| by    | by     |
| the   | the    |
| river | river  |
| bank  | bank   |
| approved | approve |
| my    | my     |
| loan  | loan   |
| in    | in     |
| the   | the    |
| spring| spring |
| of    | of     |
| last  | last   |
| spring| spring |

#### POS Tagging

| Token   | POS  | Fine POS | Explanation                                              |
|---------|------|----------|----------------------------------------------------------|
| The     | DET  | DT       | determiner                                               |
| bank    | NOUN | NN       | noun, singular or mass                                   |
| by      | ADP  | IN       | conjunction, subordinating or preposition               |
| the     | DET  | DT       | determiner                                               |
| river   | PROPN| NNP      | noun, proper singular                                    |
| bank    | PROPN| NNP      | noun, proper singular                                    |
| approved| VERB | VBD      | verb, past tense                                         |
| my      | PRON| PRP$      | pronoun, possessive                                      |
| loan    | NOUN | NN       | noun, singular or mass                                   |
| in      | ADP  | IN       | conjunction, subordinating or preposition               |
| the     | DET  | DT       | determiner                                               |
| spring  | NOUN | NN       | noun, singular or mass                                   |
| of      | ADP  | IN       | conjunction, subordinating or preposition               |
| last    | ADJ  | JJ       | adjective, other noun-modifier                          |
| spring  | NOUN | NN       | noun, singular or mass                                   |
| .       | PUNCT| .        | punctuation mark, sentence closer                        |

#### Dependency Relations

| Token   | Dep   | Head     | Explanation                       |
|---------|-------|----------|-----------------------------------|
| The     | det   | bank     | determiner                        |
| bank    | nsubj | approved | nominal subject                   |
| by      | prep  | bank     | prepositional modifier            |
| the     | det   | bank     | determiner                        |
| river   | compound | bank  | compound                          |
| bank    | pobj  | by       | object of preposition             |
| approved| ROOT  | approved | root                              |
| my      | poss  | loan     | possession modifier               |
| loan    | dobj  | approved | direct object                     |
| in      | prep  | approved | prepositional modifier            |
| the     | det   | spring   | determiner                        |
| spring  | pobj  | in       | object of preposition             |
| of      | prep  | spring   | prepositional modifier            |
| last    | amod  | spring   | adjectival modifier               |
| spring  | pobj  | of       | object of preposition             |
| .       | punct | approved | punctuation                       |

#### Noun Phrase Chunks

| Chunk             | Root  | Dep  |
|-------------------|-------|------|
| The bank          | bank  | nsubj|
| the river bank    | bank  | pobj |
| my loan           | loan  | dobj |
| the spring        | spring| pobj |
| last spring       | spring| pobj |

#### Named Entity Recognition

| Entity                           | Label | Description                             |
|----------------------------------|-------|-----------------------------------------|
| the spring of last spring        | DATE  | Absolute or relative dates or periods   |

---

### Sentence 2  
**"Apple's CEO Tim Cook said he can't bear the bear market any longer."**

#### Lemmatization

| Token  | Lemma   |
|--------|---------|
| Apple  | Apple   |
| 's     | 's      |
| CEO    | CEO     |
| Tim    | Tim     |
| Cook   | Cook    |
| said   | say     |
| he     | he      |
| ca     | can     |
| n't    | not     |
| bear   | bear    |
| the    | the     |
| bear   | bear    |
| market | market  |
| any    | any     |
| longer | long    |

#### POS Tagging

| Token  | POS   | Fine POS | Explanation                                  |
|--------|-------|----------|----------------------------------------------|
| Apple  | PROPN | NNP      | noun, proper singular                         |
| 's     | PART  | POS      | possessive ending                             |
| CEO    | PROPN | NNP      | noun, proper singular                         |
| Tim    | PROPN | NNP      | noun, proper singular                         |
| Cook   | PROPN | NNP      | noun, proper singular                         |
| said   | VERB  | VBD      | verb, past tense                              |
| he     | PRON  | PRP      | pronoun, personal                             |
| ca     | AUX   | MD       | verb, modal auxiliary                         |
| n't    | PART  | RB       | adverb                                       |
| bear   | VERB  | VB       | verb, base form                               |
| the    | DET   | DT       | determiner                                    |
| bear   | NOUN  | NN       | noun, singular or mass                        |
| market | NOUN  | NN       | noun, singular or mass                        |
| any    | ADV   | RB       | adverb                                       |
| longer | ADV   | RBR      | adverb, comparative                           |
| .      | PUNCT | .        | punctuation mark, sentence closer             |

#### Dependency Relations

| Token  | Dep   | Head   | Explanation                    |
|--------|-------|--------|--------------------------------|
| Apple  | poss  | CEO    | possession modifier            |
| 's     | case  | Apple  | case marking                   |
| CEO    | nsubj | said   | nominal subject                |
| Tim    | compound | Cook| compound                       |
| Cook   | appos | CEO    | appositional modifier          |
| said   | ROOT  | said   | root                           |
| he     | nsubj | bear   | nominal subject                |
| ca     | aux   | bear   | auxiliary                      |
| n't    | neg   | bear   | negation modifier              |
| bear   | ccomp | said   | clausal complement             |
| the    | det   | market | determiner                     |
| bear   | compound | market | compound                    |
| market | dobj  | bear   | direct object                  |
| any    | advmod| longer | adverbial modifier             |
| longer | advmod| bear   | adverbial modifier             |
| .      | punct | said   | punctuation                    |

#### Noun Phrase Chunks

| Chunk             | Root  | Dep  |
|-------------------|-------|------|
| Apple's CEO       | CEO   | nsubj|
| Tim Cook          | Cook  | appos|
| he                | he    | nsubj|
| the bear market   | market| dobj |

#### Named Entity Recognition

| Entity   | Label | Description                                 |
|----------|-------|---------------------------------------------|
| Apple    | ORG   | Companies, agencies, institutions, etc.     |
| Tim Cook | PERSON| People, including fictional                 |

---

### Sentence 3  
**"I saw her duck under the table and grab the roasted duck."**

#### Lemmatization

| Token   | Lemma   |
|---------|---------|
| I       | I       |
| saw     | see     |
| her     | her     |
| duck    | duck    |
| under   | under   |
| the     | the     |
| table   | table   |
| and     | and     |
| grab    | grab    |
| the     | the     |
| roasted | roasted |
| duck    | duck    |

#### POS Tagging

| Token   | POS   | Fine POS | Explanation                        |
|---------|-------|----------|------------------------------------|
| I       | PRON  | PRP      | pronoun, personal                  |
| saw     | VERB  | VBD      | verb, past tense                   |
| her     | PRON  | PRP$     | pronoun, possessive                |
| duck    | NOUN  | NN       | noun, singular or mass             |
| under   | ADP   | IN       | conjunction, subordinating or preposition |
| the     | DET   | DT       | determiner                         |
| table   | NOUN  | NN       | noun, singular or mass             |
| and     | CCONJ | CC       | conjunction, coordinating          |
| grab    | VERB  | VB       | verb, base form                    |
| the     | DET   | DT       | determiner                         |
| roasted | ADJ   | JJ       | adjective                          |
| duck    | NOUN  | NN       | noun, singular or mass             |
| .       | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token   | Dep   | Head   | Explanation                    |
|---------|-------|--------|--------------------------------|
| I       | nsubj | saw    | nominal subject                |
| saw     | ROOT  | saw    | root                           |
| her     | poss  | duck   | possession modifier            |
| duck    | dobj  | saw    | direct object                  |
| under   | prep  | saw    | prepositional modifier         |
| the     | det   | table  | determiner                     |
| table   | pobj  | under  | object of preposition          |
| and     | cc    | saw    | coordinating conjunction       |
| grab    | conj  | saw    | conjunct                       |
| the     | det   | duck   | determiner                     |
| roasted | amod  | duck   | adjectival modifier            |
| duck    | dobj  | grab   | direct object                  |
| .       | punct | saw    | punctuation                    |

#### Noun Phrase Chunks

| Chunk             | Root  | Dep  |
|-------------------|-------|------|
| I                 | I     | nsubj|
| her duck          | duck  | dobj |
| the table         | table | pobj |
| the roasted duck  | duck  | dobj |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

---

### Sentence 4  
**"The lead singer refused to lead because he found lead in the water."**

#### Lemmatization

| Token  | Lemma   |
|--------|---------|
| The    | the     |
| lead   | lead    |
| singer | singer  |
| refused| refuse  |
| to     | to      |
| lead   | lead    |
| because| because |
| he     | he      |
| found  | find    |
| lead   | lead    |
| in     | in      |
| the    | the     |
| water  | water   |

#### POS Tagging

| Token  | POS   | Fine POS | Explanation                        |
|--------|-------|----------|------------------------------------|
| The    | DET   | DT       | determiner                         |
| lead   | NOUN  | NN       | noun, singular or mass             |
| singer | NOUN  | NN       | noun, singular or mass             |
| refused| VERB  | VBD      | verb, past tense                   |
| to     | PART  | TO       | infinitival "to"                   |
| lead   | VERB  | VB       | verb, base form                    |
| because| SCONJ | IN       | conjunction, subordinating or preposition |
| he     | PRON  | PRP      | pronoun, personal                  |
| found  | VERB  | VBD      | verb, past tense                   |
| lead   | NOUN  | NN       | noun, singular or mass             |
| in     | ADP   | IN       | conjunction, subordinating or preposition |
| the    | DET   | DT       | determiner                         |
| water  | NOUN  | NN       | noun, singular or mass             |
| .      | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token  | Dep   | Head    | Explanation                    |
|--------|-------|---------|--------------------------------|
| The    | det   | singer  | determiner                     |
| lead   | compound | singer | compound                    |
| singer | nsubj | refused | nominal subject                |
| refused| ROOT  | refused | root                           |
| to     | aux   | lead    | auxiliary                      |
| lead   | xcomp | refused | open clausal complement        |
| because| mark  | found   | marker                         |
| he     | nsubj | found   | nominal subject                |
| found  | advcl | refused | adverbial clause modifier      |
| lead   | dobj  | found   | direct object                  |
| in     | prep  | lead    | prepositional modifier         |
| the    | det   | water   | determiner                     |
| water  | pobj  | in      | object of preposition          |
| .      | punct | refused | punctuation                    |

#### Noun Phrase Chunks

| Chunk            | Root   | Dep  |
|------------------|--------|------|
| The lead singer  | singer | nsubj|
| he               | he     | nsubj|
| lead             | lead   | dobj |
| the water        | water  | pobj |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

---

### Sentence 5  
**"She left her left shoe on the right side of the left turn."**

#### Lemmatization

| Token | Lemma |
|-------|-------|
| She   | she   |
| left  | leave |
| her   | she   |
| left  | left  |
| shoe  | shoe  |
| on    | on    |
| the   | the   |
| right | right |
| side  | side  |
| of    | of    |
| the   | the   |
| left  | left  |
| turn  | turn  |

#### POS Tagging

| Token | POS   | Fine POS | Explanation                        |
|-------|-------|----------|------------------------------------|
| She   | PRON  | PRP      | pronoun, personal                  |
| left  | VERB  | VBD      | verb, past tense                   |
| her   | PRON  | PRP      | pronoun, personal                  |
| left  | ADJ   | JJ       | adjective                          |
| shoe  | NOUN  | NN       | noun, singular or mass             |
| on    | ADP   | IN       | conjunction, subordinating or preposition |
| the   | DET   | DT       | determiner                         |
| right | ADJ   | JJ       | adjective                          |
| side  | NOUN  | NN       | noun, singular or mass             |
| of    | ADP   | IN       | conjunction, subordinating or preposition |
| the   | DET   | DT       | determiner                         |
| left  | ADJ   | JJ       | adjective                          |
| turn  | NOUN  | NN       | noun, singular or mass             |
| .     | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token | Dep   | Head | Explanation                    |
|-------|-------|------|--------------------------------|
| She   | nsubj | left | nominal subject                |
| left  | ROOT  | left | root                           |
| her   | poss  | shoe | possession modifier            |
| left  | amod  | shoe | adjectival modifier            |
| shoe  | dobj  | left | direct object                  |
| on    | prep  | left | prepositional modifier         |
| the   | det   | side | determiner                     |
| right | amod  | side | adjectival modifier            |
| side  | pobj  | on   | object of preposition          |
| of    | prep  | side | prepositional modifier         |
| the   | det   | turn | determiner                     |
| left  | amod  | turn | adjectival modifier            |
| turn  | pobj  | of   | object of preposition          |
| .     | punct | left | punctuation                    |

#### Noun Phrase Chunks

| Chunk           | Root | Dep  |
|-----------------|------|------|
| She             | She  | nsubj|
| her left shoe   | shoe | dobj |
| the right side  | side | pobj |
| the left turn   | turn | pobj |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

---

### Sentence 6  
**"They were fired after they fired the cannon at the fire station."**

#### Lemmatization

| Token  | Lemma    |
|--------|----------|
| They   | they     |
| were   | be       |
| fired  | fire     |
| after  | after    |
| they   | they     |
| fired  | fire     |
| the    | the      |
| cannon | cannon   |
| at     | at       |
| the    | the      |
| fire   | fire     |
| station| station  |

#### POS Tagging

| Token  | POS   | Fine POS | Explanation                        |
|--------|-------|----------|------------------------------------|
| They   | PRON  | PRP      | pronoun, personal                  |
| were   | AUX   | VBD      | verb, past tense                   |
| fired  | VERB  | VBN      | verb, past participle              |
| after  | SCONJ | IN       | conjunction, subordinating or preposition |
| they   | PRON  | PRP      | pronoun, personal                  |
| fired  | VERB  | VBD      | verb, past tense                   |
| the    | DET   | DT       | determiner                         |
| cannon | NOUN  | NN       | noun, singular or mass             |
| at     | ADP   | IN       | conjunction, subordinating or preposition |
| the    | DET   | DT       | determiner                         |
| fire   | NOUN  | NN       | noun, singular or mass             |
| station| NOUN  | NN       | noun, singular or mass             |
| .      | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token  | Dep      | Head   | Explanation                            |
|--------|----------|--------|----------------------------------------|
| They   | nsubjpass| fired  | nominal subject (passive)             |
| were   | auxpass  | fired  | auxiliary (passive)                   |
| fired  | ROOT     | fired  | root                                   |
| after  | mark     | fired  | marker                                 |
| they   | nsubj    | fired  | nominal subject                        |
| fired  | advcl    | fired  | adverbial clause modifier              |
| the    | det      | cannon | determiner                             |
| cannon | dobj     | fired  | direct object                          |
| at     | prep     | fired  | prepositional modifier                 |
| the    | det      | station| determiner                             |
| fire   | compound | station| compound                               |
| station| pobj     | at     | object of preposition                  |
| .      | punct    | fired  | punctuation                            |

#### Noun Phrase Chunks

| Chunk            | Root    | Dep       |
|------------------|---------|-----------|
| They             | They    | nsubjpass |
| they             | they    | nsubj     |
| the cannon       | cannon  | dobj      |
| the fire station | station | pobj      |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

---

### Sentence 7  
**"The bass player caught a bass while playing bass-heavy music on the boat."**

#### Lemmatization

| Token | Lemma  |
|-------|--------|
| The   | the    |
| bass  | bass   |
| player| player |
| caught| catch  |
| a     | a      |
| bass  | bass   |
| while | while  |
| playing| play  |
| bass  | bass   |
| heavy | heavy  |
| music | music  |
| on    | on     |
| the   | the    |
| boat  | boat   |

#### POS Tagging

| Token  | POS   | Fine POS | Explanation                        |
|--------|-------|----------|------------------------------------|
| The    | DET   | DT       | determiner                         |
| bass   | NOUN  | NN       | noun, singular or mass             |
| player | NOUN  | NN       | noun, singular or mass             |
| caught | VERB  | VBD      | verb, past tense                   |
| a      | DET   | DT       | determiner                         |
| bass   | NOUN  | NN       | noun, singular or mass             |
| while  | SCONJ | IN       | conjunction, subordinating or preposition |
| playing| VERB  | VBG      | verb, gerund or present participle |
| bass   | NOUN  | NN       | noun, singular or mass             |
| -      | PUNCT | HYPH     | punctuation mark, hyphen           |
| heavy  | ADJ   | JJ       | adjective                          |
| music  | NOUN  | NN       | noun, singular or mass             |
| on     | ADP   | IN       | conjunction, subordinating or preposition |
| the    | DET   | DT       | determiner                         |
| boat   | NOUN  | NN       | noun, singular or mass             |
| .      | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token  | Dep      | Head    | Explanation                            |
|--------|----------|---------|----------------------------------------|
| The    | det      | player  | determiner                             |
| bass   | compound | player  | compound                               |
| player | nsubj    | caught  | nominal subject                        |
| caught | ROOT     | caught  | root                                   |
| a      | det      | bass    | determiner                             |
| bass   | dobj     | caught  | direct object                          |
| while  | mark     | playing | marker                                 |
| playing| advcl    | caught  | adverbial clause modifier              |
| bass   | npadvmod | heavy   | noun phrase as adverbial modifier      |
| -      | punct    | heavy   | punctuation                            |
| heavy  | amod     | music   | adjectival modifier                    |
| music  | dobj     | playing | direct object                          |
| on     | prep     | playing | prepositional modifier                 |
| the    | det      | boat    | determiner                             |
| boat   | pobj     | on      | object of preposition                  |
| .      | punct    | caught  | punctuation                            |

#### Noun Phrase Chunks

| Chunk             | Root  | Dep  |
|-------------------|-------|------|
| The bass player   | player| nsubj|
| a bass            | bass  | dobj |
| bass-heavy music  | music | dobj |
| the boat          | boat  | pobj |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

---

### Sentence 8  
**"Will Will Smith smith a new sword for the Renaissance fair in May?"**

#### Lemmatization

| Token | Lemma       |
|-------|-------------|
| Will  | will        |
| Will  | will        |
| Smith | Smith       |
| smith | smith       |
| a     | a           |
| new   | new         |
| sword | sword       |
| for   | for         |
| the   | the         |
| Renaissance | Renaissance |
| fair  | fair        |
| in    | in          |
| May   | May         |

#### POS Tagging

| Token       | POS   | Fine POS | Explanation                        |
|-------------|-------|----------|------------------------------------|
| Will        | AUX   | MD       | verb, modal auxiliary              |
| Will        | AUX   | NNP      | noun, proper singular              |
| Smith       | PROPN | NNP      | noun, proper singular              |
| smith       | VERB  | VB       | verb, base form                    |
| a           | DET   | DT       | determiner                         |
| new         | ADJ   | JJ       | adjective                          |
| sword       | NOUN  | NN       | noun, singular or mass             |
| for         | ADP   | IN       | conjunction, subordinating or preposition |
| the         | DET   | DT       | determiner                         |
| Renaissance | PROPN | NNP      | noun, proper singular              |
| fair        | ADJ   | JJ       | adjective                          |
| in          | ADP   | IN       | conjunction, subordinating or preposition |
| May         | PROPN | NNP      | noun, proper singular              |
| ?           | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token       | Dep   | Head   | Explanation                    |
|-------------|-------|--------|--------------------------------|
| Will        | aux   | smith  | auxiliary                      |
| Will        | aux   | smith  | auxiliary                      |
| Smith       | nsubj | smith  | nominal subject                |
| smith       | ROOT  | smith  | root                           |
| a           | det   | sword  | determiner                     |
| new         | amod  | sword  | adjectival modifier            |
| sword       | dobj  | smith  | direct object                  |
| for         | prep  | sword  | prepositional modifier         |
| the         | det   | Renaissance | determiner               |
| Renaissance | pobj  | for    | object of preposition          |
| fair        | amod  | sword  | adjectival modifier            |
| in          | prep  | smith  | prepositional modifier         |
| May         | pobj  | in     | object of preposition          |
| ?           | punct | smith  | punctuation                    |

#### Noun Phrase Chunks

| Chunk          | Root   | Dep  |
|----------------|--------|------|
| Smith          | Smith  | nsubj|
| a new sword    | sword  | dobj |
| the Renaissance| Renaissance | pobj |
| May            | May    | pobj |

#### Named Entity Recognition

| Entity          | Label | Description                                 |
|-----------------|-------|---------------------------------------------|
| Will Will Smith | PERSON| People, including fictional                 |
| Renaissance     | ORG   | Companies, agencies, institutions, etc.     |
| May             | DATE  | Absolute or relative dates or periods       |

---

### Sentence 9  
**"The bark of the old oak made the dog bark louder at night."**

#### Lemmatization

| Token | Lemma  |
|-------|--------|
| The   | the    |
| bark  | bark   |
| of    | of     |
| the   | the    |
| old   | old    |
| oak   | oak    |
| made  | make   |
| the   | the    |
| dog   | dog    |
| bark  | bark   |
| louder| louder |
| at    | at     |
| night | night  |

#### POS Tagging

| Token | POS   | Fine POS | Explanation                        |
|-------|-------|----------|------------------------------------|
| The   | DET   | DT       | determiner                         |
| bark  | NOUN  | NN       | noun, singular or mass             |
| of    | ADP   | IN       | conjunction, subordinating or preposition |
| the   | DET   | DT       | determiner                         |
| old   | ADJ   | JJ       | adjective                          |
| oak   | NOUN  | NN       | noun, singular or mass             |
| made  | VERB  | VBD      | verb, past tense                   |
| the   | DET   | DT       | determiner                         |
| dog   | NOUN  | NN       | noun, singular or mass             |
| bark  | NOUN  | NN       | noun, singular or mass             |
| louder| ADV   | RBR      | adverb, comparative                |
| at    | ADP   | IN       | conjunction, subordinating or preposition |
| night | NOUN  | NN       | noun, singular or mass             |
| .     | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token | Dep   | Head  | Explanation                    |
|-------|-------|-------|--------------------------------|
| The   | det   | bark  | determiner                     |
| bark  | nsubj | made  | nominal subject                |
| of    | prep  | bark  | prepositional modifier         |
| the   | det   | oak   | determiner                     |
| old   | amod  | oak   | adjectival modifier            |
| oak   | pobj  | of    | object of preposition          |
| made  | ROOT  | made  | root                           |
| the   | det   | bark  | determiner                     |
| dog   | compound | bark | compound                     |
| bark  | dobj  | made  | direct object                  |
| louder| advmod| made  | adverbial modifier             |
| at    | prep  | made  | prepositional modifier         |
| night | pobj  | at    | object of preposition          |
| .     | punct | made  | punctuation                    |

#### Noun Phrase Chunks

| Chunk        | Root  | Dep  |
|--------------|-------|------|
| The bark     | bark  | nsubj|
| the old oak  | oak   | pobj |
| the dog bark | bark  | dobj |
| night        | night | pobj |

#### Named Entity Recognition

| Entity | Label | Description                       |
|--------|-------|-----------------------------------|
| night  | TIME  | Times smaller than a day          |

---

### Sentence 10  
**"Can you book a flight to Reading while reading this booking confirmation?"**

#### Lemmatization

| Token        | Lemma        |
|--------------|--------------|
| Can          | can          |
| you          | you          |
| book         | book         |
| a            | a            |
| flight       | flight       |
| to           | to           |
| Reading      | read         |
| while        | while        |
| reading      | read         |
| this         | this         |
| booking      | booking      |
| confirmation | confirmation |

#### POS Tagging

| Token        | POS   | Fine POS | Explanation                        |
|--------------|-------|----------|------------------------------------|
| Can          | AUX   | MD       | verb, modal auxiliary              |
| you          | PRON  | PRP      | pronoun, personal                  |
| book         | VERB  | VB       | verb, base form                    |
| a            | DET   | DT       | determiner                         |
| flight       | NOUN  | NN       | noun, singular or mass             |
| to           | ADP   | IN       | conjunction, subordinating or preposition |
| Reading      | VERB  | VBG      | verb, gerund or present participle |
| while        | SCONJ | IN       | conjunction, subordinating or preposition |
| reading      | VERB  | VBG      | verb, gerund or present participle |
| this         | DET   | DT       | determiner                         |
| booking      | NOUN  | NN       | noun, singular or mass             |
| confirmation | NOUN  | NN       | noun, singular or mass             |
| ?            | PUNCT | .        | punctuation mark, sentence closer  |

#### Dependency Relations

| Token        | Dep   | Head  | Explanation                    |
|--------------|-------|-------|--------------------------------|
| Can          | aux   | book  | auxiliary                      |
| you          | nsubj | book  | nominal subject                |
| book         | ROOT  | book  | root                           |
| a            | det   | flight| determiner                     |
| flight       | dobj  | book  | direct object                  |
| to           | prep  | flight| prepositional modifier         |
| Reading      | pobj  | to    | object of preposition          |
| while        | mark  | reading | marker                       |
| reading      | advcl | book  | adverbial clause modifier      |
| this         | det   | confirmation | determiner             |
| booking      | compound | confirmation | compound            |
| confirmation | dobj  | reading | direct object               |
| ?            | punct | book  | punctuation                    |

#### Noun Phrase Chunks

| Chunk                     | Root         | Dep  |
|---------------------------|--------------|------|
| you                       | you          | nsubj|
| a flight                  | flight       | dobj |
| this booking confirmation | confirmation | dobj |

#### Named Entity Recognition

| Entity | Label | Description |
|--------|-------|-------------|
| –      | –     | (no named entities detected) |

