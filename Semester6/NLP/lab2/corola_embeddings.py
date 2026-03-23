from pathlib import Path

from gensim.models import KeyedVectors

MODEL_PATH = "word2vec-corola-300.vec"
OUTPUT = Path(__file__).resolve().parent.parent / "lab2/corola_embeddings_results.txt"

model = KeyedVectors.load_word2vec_format(MODEL_PATH, binary=False)

WORDS = ["carte", "oras", "munte", "prieten", "citi", "scrie", "merge", "frumos", "puternic", "bun"]

ANALOGIES = [
    ("rege", "barbat", "femeie"),
    ("franța", "paris", "italia"),
    ("românia", "bucurești", "germania"),
    ("medic", "spital", "școală"),
    ("pilot", "avion", "barcă"),
    ("poet", "poezie", "tablou"),
    ("casă", "oraș", "sat"),
]

with open(OUTPUT, "w", encoding="utf-8") as out:
    print(f"Vocab: {len(model)}, Dim: {model.vector_size}\n", file=out)

    for w in WORDS:
        if w not in model:
            print(f"'{w}' not in vocab\n", file=out)
            continue
        print(f"'{w}':", file=out)
        for rank, (neighbor, score) in enumerate(model.most_similar(w, topn=10), 1):
            print(f"  {rank:2d}. {neighbor:30s} {score:.4f}", file=out)
        print(file=out)

    for a, b, c in ANALOGIES:
        missing = [w for w in (a, b, c) if w not in model]
        if missing:
            print(f"{a} - {b} + {c} => SKIP (missing: {missing})\n", file=out)
            continue
        results = model.most_similar(positive=[a, c], negative=[b], topn=5)
        print(f"{a} - {b} + {c} = ?", file=out)
        for rank, (w, score) in enumerate(results, 1):
            print(f"  {rank}. {w:30s} {score:.4f}", file=out)
        print(file=out)

print(f"Wrote embeddings output to {OUTPUT}")