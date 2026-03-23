from pathlib import Path

import numpy as np
from gensim.models import KeyedVectors

MODELS = [
    ("corola_lemma", "corola_lemma.300.50.vec", "lemma_msd"),
    ("corola_words", "corola_words.300.20.vec", "words"),
]
OUTPUT = Path(__file__).resolve().parent.parent / "lab2/corola_embeddings_results.txt"

# Words are represented as (lemma, POS)
WORDS = [
    ("carte", "NOUN"),
    ("oras", "NOUN"),
    ("munte", "NOUN"),
    ("prieten", "NOUN"),
    ("citi", "VERB"),
    ("scrie", "VERB"),
    ("merge", "VERB"),
    ("frumos", "ADJ"),
    ("puternic", "ADJ"),
    ("bun", "ADJ"),
]

# a - b + c
ANALOGIES = [
    (("rege", "NOUN"), ("barbat", "NOUN"), ("femeie", "NOUN")),
    (("franta", "PROPN"), ("paris", "PROPN"), ("italia", "PROPN")),
    (("romania", "PROPN"), ("bucuresti", "PROPN"), ("germania", "PROPN")),
    (("medic", "NOUN"), ("spital", "NOUN"), ("scoala", "NOUN")),
    (("pilot", "NOUN"), ("avion", "NOUN"), ("corabie", "NOUN")),
    (("poet", "NOUN"), ("poezie", "NOUN"), ("tablou", "NOUN")),
    (("casa", "NOUN"), ("oras", "NOUN"), ("sat", "NOUN")),
]


def strip_diacritics(text: str) -> str:
    table = str.maketrans("ăâîșşțţĂÂÎȘŞȚŢ", "aaissttAAISSTT")
    return text.translate(table)


POS_TO_MSD_PREFIXES = {
    "NOUN": ["Nc", "Np"],
    "PROPN": ["Np", "Nc"],
    "VERB": ["Vm", "Va"],
    "ADJ": ["Af"],
    "ADV": ["Rg"],
}


def lemma_norm(text: str) -> str:
    return strip_diacritics(text.lower())


def build_tokens_by_lemma(model: KeyedVectors) -> dict[str, list[str]]:
    tokens_by_lemma: dict[str, list[str]] = {}
    for token in model.index_to_key:
        if "_" not in token:
            continue
        _, lemma_part = token.split("_", 1)
        key = lemma_norm(lemma_part)
        tokens_by_lemma.setdefault(key, []).append(token)
    return tokens_by_lemma


def sort_by_frequency(model: KeyedVectors, tokens: list[str]) -> list[str]:
    return sorted(tokens, key=lambda t: model.key_to_index[t])


def resolve_token_lemma_msd(
    model: KeyedVectors,
    tokens_by_lemma: dict[str, list[str]],
    lemma: str,
    pos: str,
) -> str | None:
    norm_key = lemma_norm(lemma)
    pos_u = pos.upper()
    allowed_prefixes = POS_TO_MSD_PREFIXES.get(pos_u, [])

    pool = tokens_by_lemma.get(norm_key, [])
    if not pool:
        return None

    # Prefer POS-consistent MSD prefix first.
    pos_matched = [tok for tok in pool if any(tok.startswith(f"{pref}_") for pref in allowed_prefixes)]
    if pos_matched:
        return sort_by_frequency(model, pos_matched)[0]

    # Fallback: choose the most frequent token for this lemma
    # to avoid losing analogy coverage when POS is unavailable/noisy.
    return sort_by_frequency(model, pool)[0]


def resolve_token_words(model: KeyedVectors, lemma: str) -> str | None:
    candidates = [lemma, lemma.lower(), strip_diacritics(lemma.lower())]
    for cand in dict.fromkeys(candidates):
        if cand in model:
            return cand
    return None


def topn_by_cosine(
    model: KeyedVectors,
    query_vec: np.ndarray,
    topn: int,
    excluded: set[str] | None = None,
) -> list[tuple[str, float]]:
    excluded = excluded or set()
    vectors = model.vectors
    keys = model.index_to_key
    norms = np.linalg.norm(vectors, axis=1)
    q_norm = float(np.linalg.norm(query_vec))
    if q_norm == 0.0:
        return []

    sims = (vectors @ query_vec) / (norms * q_norm + 1e-12)
    for token in excluded:
        idx = model.key_to_index.get(token)
        if idx is not None:
            sims[idx] = -np.inf

    k = min(topn, sims.shape[0])
    best_idx = np.argpartition(sims, -k)[-k:]
    best_idx = best_idx[np.argsort(sims[best_idx])[::-1]]
    return [(keys[i], float(sims[i])) for i in best_idx]


def compare_line(left: str, right: str, width: int = 68) -> str:
    return f"{left:<{width}} | {right}"


def model_summary(
    model: KeyedVectors,
    resolver_name: str,
    words: list[tuple[str, str]],
    analogies: list[tuple[tuple[str, str], tuple[str, str], tuple[str, str]]],
) -> tuple[dict[str, str], dict[str, str], int]:
    word_rows: dict[str, str] = {}
    analogy_rows: dict[str, str] = {}
    analogy_skips = 0

    tokens_by_lemma = build_tokens_by_lemma(model) if resolver_name == "lemma_msd" else {}

    def resolve(lemma: str, pos: str) -> str | None:
        if resolver_name == "lemma_msd":
            return resolve_token_lemma_msd(model, tokens_by_lemma, lemma, pos)
        return resolve_token_words(model, lemma)

    for lemma, pos in words:
        token = resolve(lemma, pos)
        key = f"{lemma}/{pos}"
        if token is None:
            word_rows[key] = "MISSING"
            continue

        neighbors = topn_by_cosine(model, model[token], topn=3, excluded={token})
        if neighbors:
            n_text = ", ".join(f"{n} ({s:.3f})" for n, s in neighbors)
            word_rows[key] = f"token={token}; top3={n_text}"
        else:
            word_rows[key] = f"token={token}; top3=none"

    for (a_lemma, a_pos), (b_lemma, b_pos), (c_lemma, c_pos) in analogies:
        key = f"{a_lemma}/{a_pos} - {b_lemma}/{b_pos} + {c_lemma}/{c_pos}"
        a_token = resolve(a_lemma, a_pos)
        b_token = resolve(b_lemma, b_pos)
        c_token = resolve(c_lemma, c_pos)
        if None in (a_token, b_token, c_token):
            analogy_rows[key] = "SKIP (missing tokens)"
            analogy_skips += 1
            continue

        analogy_vec = model[a_token] - model[b_token] + model[c_token]
        results = topn_by_cosine(model, analogy_vec, topn=3, excluded={a_token, b_token, c_token})
        if results:
            r_text = ", ".join(f"{w} ({s:.3f})" for w, s in results)
            analogy_rows[key] = f"{r_text}"
        else:
            analogy_rows[key] = "no results"

    return word_rows, analogy_rows, analogy_skips


with open(OUTPUT, "w", encoding="utf-8") as out:
    loaded = []
    for name, path, resolver in MODELS:
        model = KeyedVectors.load_word2vec_format(path, binary=False)
        loaded.append((name, model, resolver))
        print(f"Loaded {name}: path={path}, vocab={len(model)}, dim={model.vector_size}", file=out)
    print(file=out)

    summaries = {}
    for name, model, resolver in loaded:
        summaries[name] = model_summary(model, resolver, WORDS, ANALOGIES)

    left_name = MODELS[0][0]
    right_name = MODELS[1][0]
    left_words, left_analogies, left_skips = summaries[left_name]
    right_words, right_analogies, right_skips = summaries[right_name]

    print("=" * 150, file=out)
    print(compare_line(f"WORD NEIGHBORS ({left_name})", f"WORD NEIGHBORS ({right_name})"), file=out)
    print("=" * 150, file=out)
    for lemma, pos in WORDS:
        key = f"{lemma}/{pos}"
        print(compare_line(key, key), file=out)
        print(compare_line(left_words.get(key, "-"), right_words.get(key, "-")), file=out)
        print("-" * 150, file=out)

    print("\n" + "=" * 150, file=out)
    print(compare_line(f"ANALOGIES ({left_name})", f"ANALOGIES ({right_name})"), file=out)
    print("=" * 150, file=out)
    for (a_lemma, a_pos), (b_lemma, b_pos), (c_lemma, c_pos) in ANALOGIES:
        key = f"{a_lemma}/{a_pos} - {b_lemma}/{b_pos} + {c_lemma}/{c_pos}"
        print(compare_line(key, key), file=out)
        print(compare_line(left_analogies.get(key, "-"), right_analogies.get(key, "-")), file=out)
        print("-" * 150, file=out)

    print("\n" + "=" * 150, file=out)
    print("COMPARISON SUMMARY", file=out)
    print("=" * 150, file=out)
    print(f"{left_name}: analogy SKIP count = {left_skips}/{len(ANALOGIES)}", file=out)
    print(f"{right_name}: analogy SKIP count = {right_skips}/{len(ANALOGIES)}", file=out)

print(f"Wrote embeddings comparison output to {OUTPUT}")