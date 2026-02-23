import argparse
import warnings
warnings.filterwarnings("ignore", category=UserWarning)

import spacy
from spacy import displacy

# ──────────────────────────────────────────────────────────────
#  Language models (loaded on demand)
# ──────────────────────────────────────────────────────────────
nlp_en = None
nlp_ro = None


# ──────────────────────────────────────────────────────────────
#  Challenging sentences
#  - polysemy / homonymy (bank, bat, spring, lead, bark, light)
#  - idioms, passive voice, complex syntax
#  - named entities mixed with common nouns
# ──────────────────────────────────────────────────────────────
english_sentences = [
    "The bank by the river bank approved my loan in the spring of last spring.",
    "Apple's CEO Tim Cook said he can't bear the bear market any longer.",
    "I saw her duck under the table and grab the roasted duck.",
    "The lead singer refused to lead because he found lead in the water.",
    "She left her left shoe on the right side of the left turn.",
    "They were fired after they fired the cannon at the fire station.",
    "The bass player caught a bass while playing bass-heavy music on the boat.",
    "Will Will Smith smith a new sword for the Renaissance fair in May?",
    "The bark of the old oak made the dog bark louder at night.",
    "Can you book a flight to Reading while reading this booking confirmation?",
]

romanian_sentences = [
    "Ion a mers la bancă să depună bani, apoi s-a așezat pe o bancă în parc.",
    "Profesorul de la Universitatea Babeș-Bolyai din Cluj-Napoca a publicat o carte nouă.",
    "Ea a blocat ușa cu un bloc de piatră adus din blocul vecin.",
    "Lupul și-a ascuns lupul din priviri lângă Cascada Bigăr din Caraș-Severin.",
    "Am visat un vis frumos despre Marea Neagră în noaptea de Sfântul Andrei.",
    "Poarta cetății din Alba Iulia a fost deschisă de poarta unui jucător celebru.",
    "Marcel a tăiat un corn din copac în timp ce mânca un corn cu ciocolată.",
    "Vântul a bătut puternic la Bâlea Lac, iar ceasul a bătut miezul nopții.",
    "Costul costumului popular din Maramureș a costat mai mult decât se aștepta Maria.",
    "Tudor a pus capac la oala de pe foc și capac la discuția cu Elena.",
]

SEPARATOR = "=" * 90


def print_header(title: str):
    print(f"\n{SEPARATOR}")
    print(f"  {title}")
    print(SEPARATOR)


def process_sentence(doc, sentence_num: int, lang: str):
    """Run every NLP operation on a single spaCy Doc."""

    print(f"\n{'─' * 80}")
    print(f"  [{lang.upper()}] Sentence {sentence_num}: \"{doc.text}\"")
    print(f"{'─' * 80}")

    # 1. Tokenization
    print("\n► Tokenization")
    tokens = [token.text for token in doc]
    print(f"  Tokens ({len(tokens)}): {tokens}")

    # 2. Lemmatization
    print("\n► Lemmatization")
    for token in doc:
        if token.is_punct or token.is_space:
            continue
        print(f"  {token.text:20s} → {token.lemma_}")

    # 3. POS Tagging
    print("\n► POS Tagging")
    print(f"  {'Token':20s} {'POS':8s} {'Fine POS':10s} {'Explanation'}")
    print(f"  {'─'*20} {'─'*8} {'─'*10} {'─'*30}")
    for token in doc:
        if token.is_space:
            continue
        print(f"  {token.text:20s} {token.pos_:8s} {token.tag_:10s} {spacy.explain(token.tag_) or ''}")

    # 4. Dependency Relations
    print("\n► Dependency Relations")
    print(f"  {'Token':20s} {'Dep':12s} {'Head':20s} {'Explanation'}")
    print(f"  {'─'*20} {'─'*12} {'─'*20} {'─'*30}")
    for token in doc:
        if token.is_space:
            continue
        print(f"  {token.text:20s} {token.dep_:12s} {token.head.text:20s} {spacy.explain(token.dep_) or ''}")

    # 5. Dependency Parse Tree (text-based)
    print("\n► Dependency Parse Tree")
    for sent in doc.sents:
        _print_dep_tree(sent.root, indent=0)
        print()

    # 6. Noun Phrase Chunking
    print("► Noun Phrase Chunks")
    try:
        chunks = list(doc.noun_chunks)
        if chunks:
            for chunk in chunks:
                print(f"  \"{chunk.text}\"  (root: {chunk.root.text}, dep: {chunk.root.dep_})")
        else:
            print("  (no noun chunks detected)")
    except NotImplementedError:
        print(f"  (noun_chunks not supported for '{doc.lang_}' — extracting NP-like subtrees instead)")
        _fallback_noun_chunks(doc)

    # 7. Named Entity Recognition
    print("\n► Named Entity Recognition (NER)")
    ents = list(doc.ents)
    if ents:
        for ent in ents:
            print(f"  \"{ent.text}\"  →  {ent.label_} ({spacy.explain(ent.label_) or ent.label_})")
    else:
        print("  (no named entities detected)")


def _print_dep_tree(token, indent: int):
    """Recursively print a dependency tree from a root token."""
    prefix = "  " + "│ " * indent
    print(f"{prefix}{'└─' if indent > 0 else ''}{token.text} [{token.dep_}]")
    for child in token.children:
        _print_dep_tree(child, indent + 1)


def _fallback_noun_chunks(doc):
    """Extract noun-headed subtrees when noun_chunks is not available."""
    noun_pos = {"NOUN", "PROPN"}
    seen = set()
    for token in doc:
        if token.pos_ in noun_pos and token.i not in seen:
            subtree_tokens = sorted(token.subtree, key=lambda t: t.i)
            span_text = " ".join(t.text for t in subtree_tokens)
            if len(subtree_tokens) > 1:
                for t in subtree_tokens:
                    seen.add(t.i)
                print(f"  \"{span_text}\"  (head: {token.text}, dep: {token.dep_})")


def generate_dep_html(docs, lang: str):
    """Generate an HTML file with dependency parse visualizations."""
    html = displacy.render(docs, style="dep", page=True, options={"compact": True})
    filename = f"dependency_graphs_{lang}.html"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)
    print(f"\n  ✓ Dependency graphs saved to {filename}")


def generate_ner_html(docs, lang: str):
    """Generate an HTML file with NER visualizations."""
    html = displacy.render(docs, style="ent", page=True)
    filename = f"ner_visualization_{lang}.html"
    with open(filename, "w", encoding="utf-8") as f:
        f.write(html)
    print(f"\n  ✓ NER visualization saved to {filename}")


def main():
    parser = argparse.ArgumentParser(description="NLP preprocessing with SpaCy")
    parser.add_argument("--en", action="store_true", help="Run English sentences only")
    parser.add_argument("--ro", action="store_true", help="Run Romanian sentences only")
    args = parser.parse_args()

    run_en = args.en or not (args.en or args.ro)
    run_ro = args.ro or not (args.en or args.ro)

    global nlp_en, nlp_ro
    if run_en:
        nlp_en = spacy.load("en_core_web_sm")
    if run_ro:
        nlp_ro = spacy.load("ro_core_news_sm")

    print_header("NLP Preprocessing Lab 1 — SpaCy")
    print("  Operations: Tokenization, Lemmatization, POS Tagging,")
    print("              Dependency Parsing, Chunking, NER")
    if run_en:
        print(f"  English model : {nlp_en.meta['name']}")
    if run_ro:
        print(f"  Romanian model: {nlp_ro.meta['name']}")

    # ── English ──────────────────────────────────────────────
    if run_en:
        print_header("ENGLISH SENTENCES")
        en_docs = []
        for i, sent in enumerate(english_sentences, 1):
            doc = nlp_en(sent)
            en_docs.append(doc)
            process_sentence(doc, i, "EN")
        generate_dep_html(en_docs, "en")
        generate_ner_html(en_docs, "en")

    # ── Romanian ─────────────────────────────────────────────
    if run_ro:
        print_header("ROMANIAN SENTENCES")
        ro_docs = []
        for i, sent in enumerate(romanian_sentences, 1):
            doc = nlp_ro(sent)
            ro_docs.append(doc)
            process_sentence(doc, i, "RO")
        generate_dep_html(ro_docs, "ro")
        generate_ner_html(ro_docs, "ro")

    print_header("DONE")
    print("  HTML visualizations generated:")
    if run_en:
        print("    open dependency_graphs_en.html")
        print("    open ner_visualization_en.html")
    if run_ro:
        print("    open dependency_graphs_ro.html")
        print("    open ner_visualization_ro.html")
    print()


if __name__ == "__main__":
    main()
