from pathlib import Path

import rowordnet as rwn
from rowordnet import Synset

OUTPUT = Path(__file__).resolve().parent / "rowordnet_synsets_results.txt"

wn = rwn.RoWordNet()

WORDS = {
    Synset.Pos.NOUN:      ["carte", "floare", "oraș"],
    Synset.Pos.VERB:      ["scrie", "alerga", "citi"],
    Synset.Pos.ADJECTIVE: ["rapid", "blând"],
}

def unique_synsets(literal, pos):
    seen = set()
    return [s for s in wn.synsets(literal=literal) if s not in seen and not seen.add(s) and wn.synset(s).pos == pos]


def synset_label(ss, fallback_id: str | None = None) -> str:
    if ss.literals:
        return ss.literals[0]
    return fallback_id or "(no literal)"

with open(OUTPUT, "w", encoding="utf-8") as out:
    for pos, words in WORDS.items():
        for word in words:
            sids = unique_synsets(word, pos)
            print(f"\n{'='*60}\n{word} ({pos}) — {len(sids)} senses\n{'='*60}", file=out)

            for i, sid in enumerate(sids, 1):
                s = wn.synset(sid)
                pno = s.sentiwn or [0, 0, 0]
                print(f"\n  Sense {i}: {sid}", file=out)
                print(f"    Definition  : {s.definition}", file=out)
                print(f"    Literals    : {s.literals}", file=out)
                print(f"    Lit. senses : {s.literals_senses}", file=out)
                print(f"    PNO         : P={pno[0]:.2f} N={pno[1]:.2f} O={pno[2]:.2f}", file=out)

                for target, rel in wn.outbound_relations(sid):
                    t = wn.synset(target)
                    print(f"    {rel:25s} -> {synset_label(t, target)} ({target})", file=out)

                path = wn.synset_to_hypernym_root(sid)
                labels = [synset_label(wn.synset(p), p) for p in path]
                print(f"    Path to root: {' -> '.join(labels)}", file=out)

print(f"Wrote RoWordNet output to {OUTPUT}")